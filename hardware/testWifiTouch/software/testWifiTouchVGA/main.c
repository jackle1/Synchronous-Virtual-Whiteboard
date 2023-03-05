#include "sys/alt_stdio.h"
#include <stdio.h>
#include "system.h"

unsigned char getUart (volatile unsigned char * uart);
unsigned char checkEmpty (volatile unsigned char * uart);
void getTouchscreenCoords (volatile unsigned char * uart, unsigned short * x, unsigned short * y);
void translateTouchscreenVGACoords (unsigned short * x, unsigned short * new_x, unsigned short * y, unsigned short * new_y);
void clearVGA (volatile unsigned short * vga_base, unsigned short colour);
void paintPixel (volatile unsigned short * vga_base, unsigned short x, unsigned short y, unsigned short colour);

#define TOUCHSCREEN_UART ((volatile unsigned char *) TOUCHSCREEN_UART_BASE)
#define TOUCHSCREEN_PEN_DOWN 0x81
#define TOUCHSCREEN_X 4095
#define TOUCHSCREEN_Y 4095
#define VGA_X 640
#define VGA_Y 480

#define COLOUR_WHITE -1
#define COLOUR_BLACK 0

int main()
{
  alt_putstr("Hello from Nios II!\n");
  volatile unsigned short * VGA_BASE = *((unsigned int *) VIDEO_PIXEL_BUFFER_DMA_0_BASE);
  clearVGA(VGA_BASE, COLOUR_WHITE);
  while (1)
  {
	  if (checkEmpty(TOUCHSCREEN_UART))
	  {
		  unsigned char readData = getUart(TOUCHSCREEN_UART);
		  if (readData == TOUCHSCREEN_PEN_DOWN)
		  {
			  unsigned short x, y, vga_x, vga_y;
        getTouchscreenCoords(TOUCHSCREEN_UART, &x, &y);
        //printf("Hi mary %d\n", x);
        translateTouchscreenVGACoords(&x, &vga_x, &y, &vga_y);
        paintPixel(VGA_BASE, vga_x, vga_y, COLOUR_BLACK);
		  }
	  }
  }

  return 0;
}

inline unsigned char getUart(volatile unsigned char * uart)
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

inline unsigned char checkEmpty(volatile unsigned char * uart)
{
  return *(uart + 2);
}

inline void translateTouchscreenVGACoords(unsigned short * x, unsigned short * new_x, unsigned short * y, unsigned short * new_y)
{
  *new_x = *x * VGA_X / TOUCHSCREEN_X;
  *new_y = *y * VGA_Y / TOUCHSCREEN_Y;
}

void clearVGA(volatile unsigned short * vga_base, unsigned short colour)
{
  for (int i = 0; i < VGA_X * VGA_Y; i++)
      *(vga_base + i) = colour;
}

inline void paintPixel (volatile unsigned short * vga_base, unsigned short x, unsigned short y, unsigned short colour)
{
  vga_base[x + VGA_X * y] = colour;
}
