#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "Qsys.h"

void initBridge();
void initCamera();
uint8_t getUart(volatile uint8_t *uart);
uint8_t checkTouchUartEmpty(volatile uint8_t *uart);
void getTouchscreenCoords(volatile uint8_t *uart, uint32_t *x, uint32_t *y);
void translateTouchscreenVGACoords(uint32_t *x, uint32_t *new_x, uint32_t *y, uint32_t *new_y);
void clearVGA(volatile uint32_t *vga_base, uint32_t colour);
bool drawPixel(volatile uint32_t *vga_base, uint32_t x, uint32_t y, uint32_t colour);
void paintPixel(volatile uint32_t *vga_base, uint32_t x, uint32_t y, uint32_t colour);
int paintVGAPicture(const char *file, volatile uint32_t *vga_base);
void paintCloudPicture(const char *file, volatile uint32_t *vga_base);
void saveCurrentPicture(const char *file, volatile uint32_t *vga_base);
void paintSavedPicture(const char *file, volatile uint32_t *vga_base);
void getCloudPicture(const char *file, uint16_t roomID);
uint32_t convertHex(uint32_t num);
int waitRoomID();
uint32_t getSWColour(volatile uint16_t *switches);
uint16_t getKeyAH(volatile uint16_t *keys);
void getCameraPicture(const char *path);
void *ledProgressBar(void *);

#define RESET_SLEEP 2000 // Need to wait for reset on FPGA side

#define TOUCHSCREEN_PEN_DOWN (0x81)
#define TOUCHSCREEN_X (4095)
#define TOUCHSCREEN_Y (4095)

#define PIXEL_WIDTH (3)
#define COLOUR_WHITE (-1)
#define COLOUR_BLACK (0)
#define COLOUR_BLUE (0xFF)
#define COLOUR_GREEN (0xFF << 8)
#define COLOUR_RED (0xFF << 16)
#define WHITE_MASK (7)
#define BLUE_MASK (1)
#define GREEN_MASK (2)
#define RED_MASK (4)

#define SW_9 (512)
#define KEY0_MASK (1)
#define KEY1_MASK (2)
#define KEY2_MASK (4)
#define KEY3_MASK (8)

#define GET_SW_VAL() (*SW_BASE_PTR & 0x3FF)
#define WAIT_KEY_UP(KEY_MASK) ((~(*KEY_BASE_PTR) & 0xF) & KEY_MASK)

#define ROOM_NUMBER_REQ "room_number.bin"
#define GET_PIXEL_ROOM_FILE "pixel_file.txt"
#define SAVED_PICTURE_FILE "current_pic.txt"
#define CAMERA_PICTURE_FILE "camera_pic.txt"

#define COORD_BUF_SIZE (11)
#define SERVER_SOCKET_PORT (3001)
#define SERVER_IPV4 "192.168.137.1"

// Camera Functions
#define DISABLE_CAMERA() IOWR(MIPI_PWDN_N_BASE, 0x00, 0x00)
#define ENABLE_CAMERA() IOWR(MIPI_PWDN_N_BASE, 0x00, 0xFF)
#define SWAP_CAMERA() IOWR(MIPI_PWDN_N_BASE, 0x00, ~IORD(MIPI_PWDN_N_BASE, 0))
#define NUM_FRAME_BUFFERS 3
#define GET_FRAME_START(frame) (frame * VGA_X * VGA_Y + (frame == 0 ? 4 : (frame == 1 ? 8 : 12)))

uint8_t PROGRESS_STOP_FLAG;
void *virtual_base_sdram;
volatile uint32_t *VGA_BASE;
volatile uint8_t *TOUCHSCREEN_UART;
volatile uint16_t *SW_BASE_PTR;
volatile uint16_t *KEY_BASE_PTR;
volatile uint16_t *HEX_BASE_PTR;
volatile uint16_t *LEDR_BASE_PTR;