#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
/**
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
**/

// From DE1 Manual (https://people.ece.cornell.edu/land/courses/ece5760/DE1_SOC/SoC-FPGA%20Design%20Guide_EPFL.pdf)
#define LW_BRIDGE_BASE (0xff200000)
#define LW_BRIDGE_SPAN (0x00200000) // 2MB
#define SDRAM_BASE     (0xC0000000)
#define SDRAM_SPAN     (0x03FFFFFF)

void initBridge();
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
    initBridge();   
    while (1)
    {
        if (GET_SW_VAL() == 256)
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

    if (virtual_base_lw == MAP_FAILED | virtual_base_sdram == MAP_FAILED)
    {
        printf("Could not get virtual base?\n");
        close(hps_f);
    } else
        printf("Opened HPS FD Successfully! %p %p NOT %d\n", virtual_base_lw, virtual_base_sdram, (int) MAP_FAILED);

    close(hps_f);
}

unsigned short getKeyAH (volatile unsigned short * keys)
{
    return ~(*keys) & 0xF;
}

void getCameraPicture(const char * path)
{
    unsigned int pixel_ready;
    unsigned int pixel_count = 0;
    unsigned int curr_pixel_data;
    int row = 0;
    int col = 0;
    // image transfer monitor
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
    printf("Done copying camera!\n");
}


// #include <stdio.h>
// #include <stdlib.h>
// #include "cpu.h"
// #include <fcntl.h>
// #include <sys/mman.h>
// #include <unistd.h>
// #include <string.h>
// /**
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <pthread.h>
// **/

// // From DE1 Manual (https://people.ece.cornell.edu/land/courses/ece5760/DE1_SOC/SoC-FPGA%20Design%20Guide_EPFL.pdf)
// #define LW_BRIDGE_BASE (0xff200000)
// #define LW_BRIDGE_SPAN (0x00200000) // 2MB
// #define SDRAM_BASE     (0xC0000000)
// #define SDRAM_SPAN     (0x03FFFFFF)

// void initBridge();
// unsigned short getKeyAH (volatile unsigned short * keys);
// void getCameraPicture(const char * path);

// #define RESET_SLEEP 2000 // Need to wait for reset on FPGA side

// #define TOUCHSCREEN_PEN_DOWN (0x81)
// #define TOUCHSCREEN_X (4095)
// #define TOUCHSCREEN_Y (4095)
// #define VGA_X (640)
// #define VGA_Y (480)

// #define COLOUR_WHITE (-1)
// #define COLOUR_BLACK (0)
// #define COLOUR_BLUE (0x1F)
// #define COLOUR_GREEN (0x7E0)
// #define COLOUR_RED (0xF800)
// #define BLUE_MASK (1)
// #define GREEN_MASK (2)
// #define RED_MASK   (4)

// #define KEY0_MASK (1)
// #define KEY1_MASK (2)
// #define KEY2_MASK (4)
// #define KEY3_MASK (8)

// #define GET_SW_VAL() (*SW_BASE & 0x3FF)

// #define ROOM_NUMBER_REQ "room_number.bin"
// #define GET_PIXEL_ROOM_FILE "pixel_file.txt"

// #define COORD_BUF_SIZE (11)
// #define SERVER_SOCKET_PORT (3001)
// #define SERVER_IPV4 "192.168.137.1"

// // Camera PIO
// #define PIXEL_RDY_OFF 0
// #define ACK_PIXEL_OFF 1
// #define PIXEL_DATA_OFF 2

// void * virtual_base_lw ;
// void * virtual_base_sdram ;
// volatile unsigned int * BUFFER_BASE ; 
// volatile unsigned short * VGA_BASE  ; 
// volatile unsigned char * TOUCHSCREEN_UART ;
// volatile unsigned short * SW_BASE ;
// volatile unsigned short * KEY_BASE ;
// volatile unsigned short * HEX_BASE ;  
// // Camera
// volatile int * PIX_RDY_PTR ;
// volatile int * ACK_PTR ;
// volatile int * PIX_DATA_PTR ;
// int SOCKET_FD = -1;

// int main()
// {
//     initBridge();
  
//     while (1)
//     {
//         if (GET_SW_VAL() == 256)
//         {
//             int flag = 1;
//             while (!(getKeyAH(KEY_BASE) & KEY3_MASK)) if (!(GET_SW_VAL() & 256)) flag = 0; // Wait down
//             if (flag)
//             {
//                 while (getKeyAH(KEY_BASE) & KEY3_MASK);    // Wait up
//                 getCameraPicture("camera.txt");
//                 printf("Flip switch\n");
//                 while(GET_SW_VAL() == 256);
//                 sleep(1);
//                 // FILE * fp = fopen("camera.txt", "r");
//                 // for (int i  = 0; i < VGA_X * VGA_Y;i++)
//                 // {
//                 //     unsigned short colour;
//                 //     fscanf(fp, "%hu,", &colour);
//                 //     VGA_BASE[i] = colour;
//                 // }
//                 // fclose(fp);
//             }
//         }
//     }
     
//     return 0;
// }

// void initBridge(void)
// {
//     int hps_f = open("/dev/mem", (O_RDWR | O_SYNC));
//     printf("HPS FD: %d\n", hps_f);
//     virtual_base_lw = mmap(NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, hps_f, LW_BRIDGE_BASE);
//     virtual_base_sdram = mmap(NULL, SDRAM_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, hps_f, SDRAM_BASE);
//     BUFFER_BASE = (volatile unsigned int *) (virtual_base_lw + PIXEL_BUFFER_BASE);
//     VGA_BASE  = ((volatile unsigned short *) virtual_base_sdram);//  + 0x3000000; 
//     printf("Pixel Buffer in: 0x%08x\n", *BUFFER_BASE);
//     TOUCHSCREEN_UART = (volatile unsigned char *) (virtual_base_lw + TOUCHSCREEN_UART_BASE);
//     SW_BASE = (volatile unsigned short *) (virtual_base_lw + SWITCHES_PIO_BASE);
//     KEY_BASE = (volatile unsigned short *) (virtual_base_lw + BUTTONS_PIO_BASE);
//     HEX_BASE = (volatile unsigned short *) (virtual_base_lw + HEXES_PIO_BASE); 

//     if (virtual_base_lw == MAP_FAILED | virtual_base_sdram == MAP_FAILED)
//     {
//         printf("Could not get virtual base?\n");
//         close(hps_f);
//     } else
//         printf("Opened HPS FD Successfully! %p %p NOT %d\n", virtual_base_lw, virtual_base_sdram, (int) MAP_FAILED);

//     close(hps_f);
// }


// unsigned short getKeyAH (volatile unsigned short * keys)
// {
//     return ~(*keys) & 0xF;
// }

// void getCameraPicture(const char * path)
// {
//     unsigned int pixel_ready;
//     unsigned int pixel_count = 0;
//     unsigned int curr_pixel_data;
//     int row = 0;
//     int col = 0;
//     // image transfer monitor
//     pixel_ready = *PIX_RDY_PTR;
//     FILE * fp = NULL;
//     fp = fopen(path, "w");
//     printf("Starting to read camera\n");
//     int base = 0xa00000;
//     char * camera_ptr = base + VGA_BASE;
//     int j = 0;
//     for (int i = 0; i < 640 * 480; i++)
//     {
//         unsigned short colour = ((camera_ptr[j + 2] & 248) << 8) | ((camera_ptr[j + 1] & 252) << 3) | (camera_ptr[j] >> 3);
//         // = (curr_pixel_data & 0xF800) | (curr_pixel_data & 0x7E0) | (curr_pixel_data & 0x1F);
//         fprintf(fp, "%hu,", colour);
//         //VGA_BASE[i] = colour;
//         // printf("%x = %x\n", VGA_BASE[i], colour);
//         j += 3;
//     }
//     fflush(fp);
//     fclose(fp);

//     printf("Done copying camera!\n");
// }