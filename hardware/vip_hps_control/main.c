#include "cloud.c"
#include "I2C_core.h"
#include "main.h"
#include "terasic_includes.h"
#include "mipi_camera_config.h"
#include "mipi_bridge_config.h"
#include "auto_focus.h"


/**
#include <sys/socket.h>
#include <arpa/inet.h>
**/



int main()
{
    uint16_t * roomID = (uint16_t *) malloc(sizeof(uint16_t));
    pthread_t ws_conn;
    pixel_buffer_t * rb = pixel_buffer_init();
    uint8_t * RECV_STOP_FLAG = (uint8_t *)malloc(sizeof(uint8_t));
    void *ws_conn_args[5];

    initBridge();
    initCamera();
    DISABLE_CAMERA();
    paintVGAPicture(ROOM_NUMBER_REQ, VGA_BASE);
    printf("Waiting for Room ID...\n");
    *roomID = waitRoomID();
    *roomID = 8862;
    // getCloudPicture(GET_PIXEL_ROOM_FILE, *roomID);
    // paintCloudPicture(GET_PIXEL_ROOM_FILE, VGA_BASE);

    ws_conn_args[0] = (void *)initWSConn(*roomID);
    ws_conn_args[1] = (void *)VGA_BASE;
    ws_conn_args[2] = (void *)RECV_STOP_FLAG;
    ws_conn_args[3] = (void *)rb;
    ws_conn_args[4] = (void *) roomID;
    pthread_create(&ws_conn, NULL, readWSConn, ws_conn_args);

    pixel_t *head = NULL;
    pixel_t *tail = NULL;
    while (1)
    {
        // Paint pixels
        if (checkTouchUartEmpty(TOUCHSCREEN_UART))
        {
            uint8_t readData = getUart(TOUCHSCREEN_UART);
            if (readData == TOUCHSCREEN_PEN_DOWN)
            {
                uint32_t x, y, vga_x, vga_y;
                getTouchscreenCoords(TOUCHSCREEN_UART, &x, &y);
                translateTouchscreenVGACoords(&x, &vga_x, &y, &vga_y);
                printf("Coords: Touch (%04d, %04d) | VGA (%04d, %04d)\n", x, y, vga_x, vga_y);
                if (vga_x < VGA_X && vga_y < VGA_Y)
                {
                    uint32_t colour = getSWColour(SW_BASE_PTR);
                    int end = PIXEL_WIDTH / 2;
                    int start = 0 - end;
                    for (int i = start; i < end; i++)
                        for (int j = start; j < end; j++)
                        {
                            if (drawPixel(VGA_BASE, vga_x + i, vga_y + j, colour))
                            {
                                pixel_t *new_pixel = (pixel_t *)malloc(sizeof(pixel_t));
                                while (!pixel_buffer_add(rb, vga_x + i, vga_y + j, colour));
                            }
                        }
                }
            }
        }
        // Start camera taking operation
        else if (GET_SW_VAL() & SW_9)
        {
            saveCurrentPicture(SAVED_PICTURE_FILE, VGA_BASE);
            ENABLE_CAMERA();
            char *toDisplay = SAVED_PICTURE_FILE;
            while (GET_SW_VAL() & SW_9)
            {
                if (getKeyAH(KEY_BASE_PTR) & KEY3_MASK)
                {
                    saveCurrentPicture(CAMERA_PICTURE_FILE, VGA_BASE);
                    toDisplay = CAMERA_PICTURE_FILE;
                    printf("To take Picture\n");
                }
                else if (getKeyAH(KEY_BASE_PTR) & KEY2_MASK)
                {
                    while (WAIT_KEY_UP(KEY2_MASK))
                        ;
                    printf("Swap Camera On/Off\n");
                    SWAP_CAMERA();
                }
            }
            DISABLE_CAMERA();
            sleep_us(2000);
            paintSavedPicture(toDisplay, VGA_BASE);
        }
        // If Reset is pressed
        else if (getKeyAH(KEY_BASE_PTR) & KEY3_MASK)
        {
            while (WAIT_KEY_UP(KEY3_MASK))
                ;
            getCloudPicture(GET_PIXEL_ROOM_FILE, *roomID);
            // printf("Got reset signal!!!\n");
            // while (getKeyAH(KEY_BASE_PTR) & KEY3_MASK); // Wait for key-up
            // paintVGAPicture(ROOM_NUMBER_REQ, VGA_BASE);
            // roomID = waitRoomID();
            // getCloudPicture(GET_PIXEL_ROOM_FILE);
        }
        else if (getKeyAH(KEY_BASE_PTR) & KEY2_MASK) // Send pixels
        {
            while (WAIT_KEY_UP(KEY2_MASK))
                ;
        }
    }

    return 0;
}

void initBridge(void)
{
    int hps_f = open("/dev/mem", (O_RDWR | O_SYNC));
    printf("HPS FD: %d\n", hps_f);
    lw_bridge_ptr = mmap(NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, hps_f, LW_BRIDGE_BASE);
    virtual_base_sdram = mmap(NULL, HPS_SDRAM_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, hps_f, HPS_SDRAM_BASE);
    VGA_BASE = ((volatile uint32_t *)virtual_base_sdram); //  + 0x3000000;
    TOUCHSCREEN_UART = (volatile uint8_t *)(lw_bridge_ptr + TOUCHSCREEN_UART_BASE);
    SW_BASE_PTR = (volatile uint16_t *)(lw_bridge_ptr + SW_BASE);
    KEY_BASE_PTR = (volatile uint16_t *)(lw_bridge_ptr + KEY_BASE);
    HEX_BASE_PTR = (volatile uint16_t *)(lw_bridge_ptr + HEXES_PIO_BASE);

    if (lw_bridge_ptr == MAP_FAILED | virtual_base_sdram == MAP_FAILED)
    {
        printf("Could not get virtual base?\n");
        close(hps_f);
    }
    else
        printf("Opened HPS FD Successfully! %p %p NOT %d\n", lw_bridge_ptr, virtual_base_sdram, (int)MAP_FAILED);

    while (checkTouchUartEmpty(TOUCHSCREEN_UART)) // Ensure touchscreen uart buffer is clear before starting
    {
        uint32_t x, y, vga_x, vga_y;
        getTouchscreenCoords(TOUCHSCREEN_UART, &x, &y);
    }

    close(hps_f);
}

uint8_t getUart(volatile uint8_t *uart)
{
    while (!(uart[1] & 0x80))
        ;
    return *uart;
}

void getTouchscreenCoords(volatile uint8_t *uart, uint32_t *x, uint32_t *y)
{
    *x = getUart(uart);
    *x |= getUart(uart) << 7;
    *y = getUart(uart);
    *y |= getUart(uart) << 7;
}

uint8_t checkTouchUartEmpty(volatile uint8_t *uart)
{
    return *(uart + 2);
}

void translateTouchscreenVGACoords(uint32_t *x, uint32_t *new_x, uint32_t *y, uint32_t *new_y)
{
    *new_x = *x * VGA_X / TOUCHSCREEN_X;
    *new_y = *y * VGA_Y / TOUCHSCREEN_Y;
    if (*new_x == VGA_Y)
        *new_x = *new_x - 1;
    if (*new_y == VGA_X)
        *new_y = *new_y - 1;
}

bool drawPixel(volatile uint32_t *vga_base, uint32_t x, uint32_t y, uint32_t colour)
{
    for (int frame = 0; frame < NUM_FRAME_BUFFERS; frame++)
    {
        int offset = frame * VGA_X * VGA_Y;
        if (x >= 0 && x < VGA_X &&
            y >= 0 && y < VGA_Y)
        {
            ((uint32_t *)vga_base)[x + VGA_X * y + offset] = colour;
        }
        else
            return FALSE;
    }
    return TRUE;
}

void clearVGA(volatile uint32_t *vga_base, uint32_t colour)
{
    int i;
    for (i = 0; i < VGA_X * VGA_Y; i++)
    {
        vga_base[i] = colour;
    }
}

int paintVGAPicture(const char *file, volatile uint32_t *vga_base)
{
    FILE *fd = NULL;
    fd = fopen(file, "rb");
    if (fd == NULL)
    {
        printf("Failed to open file %s", file);
        return 0;
    }
    for (int frame = 0; frame < NUM_FRAME_BUFFERS; frame++)
    {
        int start = GET_FRAME_START(frame);
        for (int i = start; i < start + VGA_X * VGA_Y; i++)
        {
            uint32_t pixel;
            fread(&pixel, sizeof(pixel), 1, fd);
            vga_base[i] = pixel;
        }
        fseek(fd, 0, SEEK_SET);
    }
    printf("Painted waiting room\n");
    fclose(fd);
    return 1;
}

int waitRoomID()
{
    uint32_t result;
    while (1)
    {
        if (!(*KEY_BASE_PTR & 2))
            break;
        uint16_t num = GET_SW_VAL();
        result = 0;
        for (int i = 0; i < 4; i++)
        {
            result |= convertHex(num % 10) << (i * 7);
            num /= 10;
        }
        *HEX_BASE_PTR = result & 0xFFFF;
        *(HEX_BASE_PTR + 1) = result >> 16;
    }
    printf("Got Switches!!! %d\n", GET_SW_VAL());
    sleep_us(3000);
    *HEX_BASE_PTR = -1;
    sleep_us(3000);
    *(HEX_BASE_PTR + 1) = -1;
    return GET_SW_VAL();
}

inline uint32_t convertHex(uint32_t num)
{
    switch (num)
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

uint32_t getSWColour(volatile uint16_t *switches)
{
    uint32_t value = GET_SW_VAL();
    if (value & WHITE_MASK)
        return COLOUR_WHITE;
    if (value & RED_MASK)
        return COLOUR_RED;
    else if (value & BLUE_MASK)
        return COLOUR_BLUE;
    else if (value & GREEN_MASK)
        return COLOUR_GREEN;
    else
        return COLOUR_BLACK;
}

uint16_t getKeyAH(volatile uint16_t *keys)
{
    return ~(*keys) & 0xF;
}

void paintCloudPicture(const char *file, volatile uint32_t *vga_base)
{
    uint32_t colour;
    char read;
    FILE *file_fd = NULL;
    FILE *debug = NULL;
    debug = fopen("debug.txt", "w");
    fprintf(debug, "[[");

    file_fd = fopen(file, "r");
    if (file_fd != NULL)
    {
        printf("Starting file read\n");
        // Skip header bytes
        do
        {
            fscanf(file_fd, "%c", &read);
        } while (read != '[');
        int pic_start = ftell(file_fd);
        printf("Found first array @ %d\n", pic_start);
        for (int frame = 0; frame < NUM_FRAME_BUFFERS; frame++)
        {
            int start = GET_FRAME_START(frame);
            for (int y = 0; y < VGA_Y; y++)
            {
                for (int x = 0; x < VGA_X; x++)
                {
                    int ret = fscanf(file_fd, "%u, ", &colour);
                    if (ret == 0)
                    {
                        ret = fscanf(file_fd, "], [%u, ", &colour);
                        if (!ret)
                        {
                            ret = fscanf(file_fd, "[%u, ", &colour);
                            if (!ret)
                                printf("Failed again (%d, %d) %ld\n", x, y, ftell(file_fd));
                        }
                    }
                    vga_base[start + x + VGA_X * y] = colour;
                }
            }

            fseek(file_fd, pic_start, SEEK_SET);
        }
    }
    printf("Done Painting\n");
    fclose(file_fd);
    fflush(debug);
    fclose(debug);
}

void getCloudPicture(const char *file, uint16_t roomID)
{
    volatile http_res_t *get_pixel_res = (http_res_t *)malloc(sizeof(http_res_t));
    get_pixel_res->done = CLOUD_WAIT;
    FILE *pixel_file = fopen(GET_PIXEL_ROOM_FILE, "w");
    get_pixel_res->file = pixel_file;
    getRoomPixels(get_pixel_res, roomID); // GET_SW_VAL());
    printf("CLOUD WAIT\n");
    while (get_pixel_res->done == CLOUD_WAIT)
        ;
    printf("DONE CLOUD WAIT\n");
    if (get_pixel_res->done == CLOUD_DONE)
    {
        fclose(pixel_file);
        free((void *)get_pixel_res);
        paintCloudPicture(GET_PIXEL_ROOM_FILE, VGA_BASE);
        printf("Done painting cloud\n");
    }
}

void initCamera(void)
{
    IOWR(MIPI_PWDN_N_BASE, 0x00, 0x00);
    IOWR(MIPI_RESET_N_BASE, 0x00, 0x00);

    sleep_us(2000);
    IOWR(MIPI_PWDN_N_BASE, 0x00, 0xFF);
    sleep_us(2000);
    IOWR(MIPI_RESET_N_BASE, 0x00, 0xFF);

    sleep_us(2000);

    // MIPI Init
    if (!MIPI_Init())
    {
        printf("MIPI_Init Init failed!\r\n");
    }
    else
    {
        printf("MIPI_Init Init successfully!\r\n");
    }

    mipi_clear_error();
    sleep_us(50 * 1000);
    mipi_clear_error();
    sleep_us(1000 * 1000);
    mipi_show_error_info();
    //	    mipi_show_error_info_more();
    printf("\n");
}

void saveCurrentPicture(const char *file, volatile uint32_t *vga_base)
{
    FILE *fd = fopen(file, "w");
    if (fd == NULL)
    {
        printf("ERROR: Could not save current picture!!!\n");
        return;
    }
    for (int i = GET_FRAME_START(0); i < VGA_X * VGA_Y; i++)
        fprintf(fd, "%u,", vga_base[i]);
    fflush(fd);
    fclose(fd);
    printf("Saved Current Picture!\n");
}

void paintSavedPicture(const char *file, volatile uint32_t *vga_base)
{
    FILE *fd = fopen(file, "r");
    if (fd == NULL)
    {
        printf("ERROR: Could not save current picture!!!\n");
        return;
    }
    for (int frame = 0; frame < NUM_FRAME_BUFFERS; frame++)
    {
        int start = GET_FRAME_START(frame);
        for (int i = start; i < start + VGA_X * VGA_Y; i++)
        {
            uint32_t colour;
            fscanf(fd, "%u,", &colour);
            vga_base[i] = colour;
        }
        fseek(fd, 0, SEEK_SET);
    }

    fclose(fd);
    printf("Repainted Current Picture\n");
}
