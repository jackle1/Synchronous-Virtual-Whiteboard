/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include <system.h>
#include <unistd.h>
#include <fcntl.h>
#include <altera_up_avalon_rs232.h>

#define COORDS_S 8

volatile unsigned short * LED_ADDR = (unsigned short *) LEDS_R_BASE;
unsigned char getUart(alt_up_rs232_dev* uart);

int main()
{
  printf("Hello from Nios II!\n");
  *LED_ADDR = 0;
  alt_up_rs232_dev* touchUart = alt_up_rs232_open_dev(TOUCHSCREEN_UART_NAME);
  alt_up_rs232_dev* wifiUart= alt_up_rs232_open_dev(WIFI_UART_NAME);
  alt_fd* wuart = (alt_fd *) open(WIFI_UART_NAME, O_RDWR);
  if (wuart == NULL)
  {
	  printf("NOPE");
	  return 1;
  }
  while (1)
  {
	  if (alt_up_rs232_get_used_space_in_read_FIFO(touchUart))
	  {
		  alt_u8 readData, parityEr;
		  alt_up_rs232_read_data(touchUart, &readData, &parityEr);
		  if (readData == 0x81)
		  {
			  unsigned short x = getUart(touchUart);
			  x |= getUart(touchUart) << 7;
			  unsigned short y = getUart(touchUart);
			  y |= getUart(touchUart) << 7;
			  printf("Coords (%04d, %04d) | Buf %d\n", x, y, alt_up_rs232_get_used_space_in_read_FIFO(touchUart));

			  char coords[COORDS_S + 1];
			  snprintf(coords, COORDS_S + 1, "%04d%04d", x, y);
			  for (int i = 0; i < COORDS_S; i++)
			  {
				  alt_up_rs232_write_data(wifiUart, (alt_u8) coords[i]);
			  }
			  unsigned char toLED = getUart(wifiUart);
			  *LED_ADDR = toLED;
		  }
	  }

  }

  return 0;
}

inline unsigned char getUart(alt_up_rs232_dev* uart)
{
	while(alt_up_rs232_get_used_space_in_read_FIFO(uart) == 0);
	unsigned char result, parity;
	alt_up_rs232_read_data(uart, &result, &parity);
	return result;
}

