#include "cpu.h"
#include "cloud.c"
#include "I2C_core.h"
#include "terasic_includes.h"
#include "mipi_camera_config.h"
#include "mipi_bridge_config.h"
#include "auto_focus.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
/**
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
**/

void initBridge();
unsigned char getUart (volatile unsigned char * uart);
unsigned char checkTouchUartEmpty (volatile unsigned char * uart);
void getTouchscreenCoords (volatile unsigned char * uart, unsigned short * x, unsigned short * y);
void translateTouchscreenVGACoords (unsigned short * x, unsigned short * new_x, unsigned short * y, unsigned short * new_y);
void clearVGA (volatile unsigned short * vga_base, unsigned short colour);
void paintPixel (volatile unsigned short * vga_base, unsigned short x, unsigned short y, unsigned short colour);
int paintVGAPicture (const char * file, volatile unsigned short * vga_base);
void paintCloudPicture (const char * file, volatile unsigned short * vga_base);
void getCloudPicture (const char * file);
unsigned short convertHex(unsigned short num);
int waitRoomID();
unsigned short getSWColour (volatile unsigned short * switches); 
unsigned short getKeyAH (volatile unsigned short * keys);
void getCameraPicture(const char * path);

#define RESET_SLEEP 2000 // Need to wait for reset on FPGA side

#define TOUCHSCREEN_PEN_DOWN (0x81)
#define TOUCHSCREEN_X (4095)
#define TOUCHSCREEN_Y (4095)
#define VGA_X (640)
#define VGA_Y (480)

#define COLOUR_WHITE (-1)
#define COLOUR_BLACK (0)
#define COLOUR_BLUE (0x1F)
#define COLOUR_GREEN (0x7E0)
#define COLOUR_RED (0xF800)
#define BLUE_MASK (1)
#define GREEN_MASK (2)
#define RED_MASK   (4)

#define KEY0_MASK (1)
#define KEY1_MASK (2)
#define KEY2_MASK (4)
#define KEY3_MASK (8)

#define GET_SW_VAL() (*SW_BASE & 0x3FF)

#define ROOM_NUMBER_REQ "room_number.bin"
#define GET_PIXEL_ROOM_FILE "pixel_file.txt"

#define COORD_BUF_SIZE (11)
#define SERVER_SOCKET_PORT (3001)
#define SERVER_IPV4 "192.168.137.1"

// Camera PIO
#define PIXEL_RDY_OFF 0
#define ACK_PIXEL_OFF 1
#define PIXEL_DATA_OFF 2

void * virtual_base_lw ;
void * virtual_base_sdram ;
volatile unsigned int * BUFFER_BASE ; 
volatile unsigned short * VGA_BASE  ; 
volatile unsigned char * TOUCHSCREEN_UART ;
volatile unsigned short * SW_BASE ;
volatile unsigned short * KEY_BASE ;
volatile unsigned short * HEX_BASE ;  
// Camera
volatile int * PIX_RDY_PTR ;
volatile int * ACK_PTR ;
volatile int * PIX_DATA_PTR ;
int SOCKET_FD = -1;

int main()
{
    int roomID;
    initBridge();
    paintVGAPicture(ROOM_NUMBER_REQ, VGA_BASE);
    roomID = waitRoomID();
    getCloudPicture(GET_PIXEL_ROOM_FILE);
    pixel_t * head = NULL;
    pixel_t * tail = NULL;   
    while (1)
    {
        // Paint pixels
        if (checkTouchUartEmpty(TOUCHSCREEN_UART))
        {
            unsigned char readData = getUart(TOUCHSCREEN_UART);
            if (readData == TOUCHSCREEN_PEN_DOWN)
            {
                unsigned short x, y, vga_x, vga_y;
                getTouchscreenCoords(TOUCHSCREEN_UART, &x, &y);
                translateTouchscreenVGACoords(&x, &vga_x, &y, &vga_y);
                printf("Coords: Touch (%04d, %04d) | VGA (%04d, %04d)\n", x, y, vga_x, vga_y);
                if (vga_x < VGA_X && vga_y < VGA_Y)
                {
                    unsigned short colour = getSWColour(SW_BASE);
                    paintPixel(VGA_BASE, vga_x, vga_y, colour);
                    pixel_t * new_pixel = (pixel_t *) malloc(sizeof(pixel_t));
                    new_pixel->x = vga_x;
                    new_pixel->y = vga_y;
                    new_pixel->colour = colour;
                    new_pixel->next = NULL;
                    if (head == NULL) {
                        head = new_pixel;
                        tail = new_pixel;
                    } else
                    {
                        tail->next = new_pixel;
                        tail = new_pixel;
                    }
                    // putRoomPixels(roomID, vga_x, vga_y, colour); 
                }
            }
        }
        // Start camera taking operation
        else if (GET_SW_VAL() == 256)
        {
            int flag = 1;
            while (!(getKeyAH(KEY_BASE) & KEY3_MASK)) if (!(GET_SW_VAL() & 256)) flag = 0; // Wait down
            if (flag)
            {
                while (getKeyAH(KEY_BASE) & KEY3_MASK);    // Wait up
                getCameraPicture("camera.txt");
                printf("Flip switch\n");
                while(GET_SW_VAL() == 256);
                sleep(1);
                // FILE * fp = fopen("camera.txt", "r");
                // for (int i  = 0; i < VGA_X * VGA_Y;i++)
                // {
                //     unsigned short colour;
                //     fscanf(fp, "%hu,", &colour);
                //     VGA_BASE[i] = colour;
                // }
                // fclose(fp);
            }
        }
        // If Reset is pressed
        else if (getKeyAH(KEY_BASE) & KEY3_MASK)
        {
            getCloudPicture(GET_PIXEL_ROOM_FILE);
            // printf("Got reset signal!!!\n");
            // while (getKeyAH(KEY_BASE) & KEY3_MASK); // Wait for key-up
            // paintVGAPicture(ROOM_NUMBER_REQ, VGA_BASE);
            // roomID = waitRoomID();
            // getCloudPicture(GET_PIXEL_ROOM_FILE); 
        } 
        else if (getKeyAH(KEY_BASE) & KEY2_MASK) // Send pixels
        {
            while (getKeyAH(KEY_BASE) & KEY2_MASK);
            printf("Sending bulk pixels\n");
            sendBulkPixel(head, 8862);
            printf("Done sending!!!\n");
            head = NULL;
            tail = NULL;
        }
    }
     
    return 0;
}

void initBridge(void)
{
    int hps_f = open("/dev/mem", (O_RDWR | O_SYNC));
    printf("HPS FD: %d\n", hps_f);
    virtual_base_lw = mmap(NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, hps_f, LW_BRIDGE_BASE);
    virtual_base_sdram = mmap(NULL, SDRAM_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, hps_f, SDRAM_BASE);
    BUFFER_BASE = (volatile unsigned int *) (virtual_base_lw + PIXEL_BUFFER_BASE);
    VGA_BASE  = ((volatile unsigned short *) virtual_base_sdram);//  + 0x3000000; 
    printf("Pixel Buffer in: 0x%08x\n", *BUFFER_BASE);
    TOUCHSCREEN_UART = (volatile unsigned char *) (virtual_base_lw + TOUCHSCREEN_UART_BASE);
    SW_BASE = (volatile unsigned short *) (virtual_base_lw + SWITCHES_PIO_BASE);
    KEY_BASE = (volatile unsigned short *) (virtual_base_lw + BUTTONS_PIO_BASE);
    HEX_BASE = (volatile unsigned short *) (virtual_base_lw + HEXES_PIO_BASE); 
    PIX_RDY_PTR = (volatile int *) (virtual_base_lw + IMG_CPU_READER_0_BASE + PIXEL_RDY_OFF);
    ACK_PTR  = (volatile int *) (virtual_base_lw + IMG_CPU_READER_0_BASE + ACK_PIXEL_OFF);
    PIX_DATA_PTR = (volatile int *) (virtual_base_lw + IMG_CPU_READER_0_BASE + PIXEL_DATA_OFF);

    if (virtual_base_lw == MAP_FAILED | virtual_base_sdram == MAP_FAILED)
    {
        printf("Could not get virtual base?\n");
        close(hps_f);
    } else
        printf("Opened HPS FD Successfully! %p %p NOT %d\n", virtual_base_lw, virtual_base_sdram, (int) MAP_FAILED);

    while (checkTouchUartEmpty(TOUCHSCREEN_UART)) // Ensure touchscreen uart buffer is clear before starting 
    {
        unsigned short x, y, vga_x, vga_y;
        getTouchscreenCoords(TOUCHSCREEN_UART, &x, &y);
    }

    close(hps_f);
}

unsigned char getUart(volatile unsigned char * uart)
{
    while (!(uart[1] & 0x80));
    return *uart;
}

void getTouchscreenCoords(volatile unsigned char * uart, unsigned short * x, unsigned short * y)
{
    *x = getUart(uart);
    *x |= getUart(uart) << 7;
    *y = getUart(uart);
    *y |= getUart(uart) << 7;
}

unsigned char checkTouchUartEmpty(volatile unsigned char * uart)
{
    return *(uart + 2);
}

void translateTouchscreenVGACoords(unsigned short * x, unsigned short * new_x, unsigned short * y, unsigned short * new_y)
{
    *new_x = *x * VGA_X / TOUCHSCREEN_X;
    *new_y = *y * VGA_Y / TOUCHSCREEN_Y;
    if (*new_x == VGA_Y)
        *new_x = *new_x - 1;
    if (*new_y == VGA_X)
        *new_y = *new_y - 1;
}



void paintPixel (volatile unsigned short * vga_base, unsigned short x, unsigned short y, unsigned short colour)
{
    int block = 3;
    int end = block / 2;
    int start = 0 - end;
    for (int i = start; i < end; i++)
    {
       for (int j = start; j < end; j++)
       {
            if ( (int) x + i >= 0 && x + i < VGA_X &&
                    (int) y + j >= 0 && y + j < VGA_Y) 
                ((unsigned short *) vga_base)[ (x + i) + VGA_X * (y - j)] = colour;
       }
    }
}

void clearVGA(volatile unsigned short * vga_base, unsigned short colour)
{
    int i;
    for (i = 0; i < VGA_X * VGA_Y; i++)
    {
        vga_base[i] = colour;
    }
}

int paintVGAPicture (const char * file, volatile unsigned short * vga_base)
{
    FILE * fd = NULL;
    fd = fopen(file, "rb");
    if (fd == NULL)
    {
        printf("Failed to open file %s", file);
        return 0;
    }
    for (int i = 0; i < VGA_X * VGA_Y; i++)
    {
        unsigned short pixel;
        fread(&pixel, sizeof(pixel), 1, fd);
        vga_base[i] = pixel;
    }
    fclose(fd);
    return 1;
}

int waitRoomID ()
{
    unsigned int result;
    while (1)
    {
        if (!(*KEY_BASE & 2))
            break;
        unsigned short num = *SW_BASE;
        result = 0;
        for (int i = 0; i < 4; i++)
        {
            result |= convertHex(num % 10) << (i * 7);
            num /= 10;
        }
        *HEX_BASE = result & 0xFFFF;
        *(HEX_BASE + 1) = result >> 16;
    }
    printf("Got Switches!!! %d\n", GET_SW_VAL());
    *HEX_BASE = 0xFFFF;
    *(HEX_BASE + 1) = 0xFFFF;
    return GET_SW_VAL();
}

inline unsigned short convertHex(unsigned short num)
{
    switch(num)
    {
        case 0:
            return 64;
        case 1:
            return 121;
        case 2:
            return 36;
        case 3:
            return 48;
        case 4:
            return 25;
        case 5:
            return 18;
        case 6:
            return 2;
        case 7:
            return 120;
        case 8:
            return 0;
        case 9:
            return 24;
        default:
            return 127;
    }
}

unsigned short getSWColour (volatile unsigned short * switches)
{
    // Mask to get only 10 bits
    unsigned short value = *switches & 0x3FF;
    if (value & RED_MASK)
        return COLOUR_RED;
    else if (value & BLUE_MASK)
        return COLOUR_BLUE;
    else if (value & GREEN_MASK)
        return COLOUR_GREEN;
    else
        return COLOUR_BLACK;
}

unsigned short getKeyAH (volatile unsigned short * keys)
{
    return ~(*keys) & 0xF;
}

void paintCloudPicture (const char * file, volatile unsigned short * vga_base)
{
    unsigned short colour;
    char read;
    int i = 0;
    FILE * file_fd = NULL;

    file_fd = fopen(file, "r");
    if (file_fd != NULL)
    {
        printf("Starting file read\n");
        // Skip header bytes 
        do
        {
            fscanf(file_fd, "%c", &read);
        } while (read != '[');
        printf("Found first array\n");
        fscanf(file_fd, "%c", &read); // get [ - start of array
        while (i < VGA_X * VGA_Y)
        {
            int ret = fscanf(file_fd, "%hu, ", &colour);
            if (ret <= 0)
            {
                ret = fscanf(file_fd, "%hu], [", &colour);
                read = '0';
                while (read != '[') fscanf(file_fd, "%c", &read);
            }
            ((unsigned short *) vga_base)[i++] = colour;
        }
    }

}

void getCloudPicture(const char * file)
{
    volatile http_res_t * get_pixel_res = (http_res_t *) malloc(sizeof(http_res_t));
    get_pixel_res->done = CLOUD_WAIT; 
    FILE * pixel_file = fopen(GET_PIXEL_ROOM_FILE, "w");
    get_pixel_res->file = pixel_file;
    getRoomPixels(get_pixel_res, 8862); //GET_SW_VAL());
    printf("CLOUD WAIT\n");
    while (get_pixel_res->done == CLOUD_WAIT);
    printf("DONE CLOUD WAIT\n");
    if (get_pixel_res->done == CLOUD_DONE)
    {
        fclose(pixel_file);
        free((void *) get_pixel_res);
        paintCloudPicture(GET_PIXEL_ROOM_FILE, VGA_BASE);
        printf("Done painting cloud\n");
    }
}

void getCameraPicture(const char * path)
{
    unsigned int pixel_ready;
    unsigned int pixel_count = 0;
    unsigned int curr_pixel_data;
    int row = 0;
    int col = 0;
    // image transfer monitor
    pixel_ready = *PIX_RDY_PTR;
    FILE * fp = NULL;
    fp = fopen(path, "w");
    printf("Starting to read camera\n");
    int base = 0xa00000;
    char * camera_ptr = base + VGA_BASE;
    int j = 0;
    for (int i = 0; i < 640 * 480; i++)
    {
        unsigned short colour = ((camera_ptr[j + 2] & 248) << 8) | ((camera_ptr[j + 1] & 252) << 3) | (camera_ptr[j] >> 3);
        // = (curr_pixel_data & 0xF800) | (curr_pixel_data & 0x7E0) | (curr_pixel_data & 0x1F);
        fprintf(fp, "%hu,", colour);
        //VGA_BASE[i] = colour;
        // printf("%x = %x\n", VGA_BASE[i], colour);
        j += 3;
    }
    fflush(fp);
    fclose(fp);
    
    // if (pixel_ready == 1) 
    // {
    //     // turn LED
    //     while (pixel_ready != 2) 
    //     {
    //         if (pixel_ready == 0) 
    //             pixel_ready = *PIX_RDY_PTR;
    //         else 
    //         {
    //             *ACK_PTR = 1;
    //             curr_pixel_data = *PIX_DATA_PTR;
    //             if (row < 640) 
    //             {
    //                 if(col < 480) 
    //                 {   //((red & 248) << 8) | ((green & 252) << 3) | (blue >> 3)
    //                     unsigned short colour = (curr_pixel_data & 0xF800) | (curr_pixel_data & 0x7E0) | (curr_pixel_data & 0x1F);
    //                     fprintf(fp, "%hu,", colour);
    //                     // printf("pixel %x %hu\n", curr_pixel_data, colour);
    //                     col++;
    //                 } else 
    //                 {
    //                     col = 0;
    //                     row++;
    //                 }
    //             } else 
    //                 break;
    //             pixel_count++;
    //             *ACK_PTR = 0;
    //             pixel_ready = *PIX_RDY_PTR;
    //         }
    //     }
    // }
    printf("Done copying camera!\n");
}
