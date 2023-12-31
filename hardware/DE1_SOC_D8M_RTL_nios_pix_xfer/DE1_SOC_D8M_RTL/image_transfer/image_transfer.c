#include "system.h"
#include "nios2.h"
#include "string.h"
#include "stdlib.h"
#include "sys/alt_stdio.h"

#define IMG_TRANSFER_BASE   IMG_CPU_READER_0_BASE
#define PIXEL_RDY           IMG_TRANSFER_BASE + 0
#define ACK_PIXEL           IMG_TRANSFER_BASE + 1
#define PIXEL_DATA          IMG_TRANSFER_BASE + 2

#define KEY_2_BASE 			KEY_2_BASE

#define JTAG_BASE			JTAG_UART_0_BASE

int curr_pixel_data;
 unsigned int pixel_count;

unsigned int pixels[120][120];
unsigned int row;
unsigned int col;

int main(void) {
    unsigned int pixel_ready;
    row = 0;
    col = 0;

    volatile int *PIX_RDY_PTR = (int *) IMG_TRANSFER_BASE;
    volatile int *ACK_PTR = (int *) ACK_PIXEL;
    volatile int *PIX_DATA_PTR = (int *) PIXEL_DATA;

    volatile int *JTAG_PTR = (int *) JTAG_BASE;

    char prompt = 0;

     pixel_count = 0;

//     while (pixel_count < 10) {
//    	 curr_pixel_data = *PIX_DATA_PTR;
//		alt_printf("\n Pixel: %x", curr_pixel_data);
//		pixel_count++;
//     }



	while (1) {
		// image transfer monitor
		pixel_ready = *PIX_RDY_PTR;
//        alt_printf("\nStarted %x", pixel_ready);
		if (pixel_ready == 1) {
			alt_printf("\nEntered1");
			// turn LED
			while(pixel_ready != 2) {
				prompt = *JTAG_PTR;
				if(pixel_ready == 0) {   //next pixel not ready, wait
					pixel_ready = *PIX_RDY_PTR;
				} else {//if(prompt == 'a'){    //next pixel ready, acknowledge & process. Ack when prompt == a used to debug pixel by pixel
//					alt_printf("\nEntered2");
					*ACK_PTR = 1;

					curr_pixel_data = *PIX_DATA_PTR;

					if(row < 640) {
						if(col < 480) {
							pixels[row][col] = curr_pixel_data;
//							alt_printf("%x ", curr_pixel_data);
							col++;
						} else {
							alt_printf("Row %x done\n", row);
							col = 0;
							row++;
						}
					} else {
						break;
					}
//					alt_printf("\n Pixel %x: %x", pixel_count, curr_pixel_data);

					 pixel_count++;
					*ACK_PTR = 0;

					pixel_ready = *PIX_RDY_PTR;

//                    if(row < 120) {
//                    	if(col < 120) {
//                    		pixels[row][col] = curr_pixel_data;
//                    		col++;
//                    	} else {
//                    		row++;
//                    	}
//                    } else {
//                    	break;
//                    }
					// turn off LED

					// if (pixel_count == 640 * 480 / 256) {
					//     // full image captured
					// }
				}

			}
			alt_printf("\nDone pixels.");

			for(unsigned int r = 0 ; r < 120; r++) {
				for(unsigned int c = 0; c < 120; c++) {
					alt_printf("%x ", pixels[r][c]);
				}
				alt_printf("\n");
			}
			break;
		}
	}


        // wifi monitor


        // touch screen monitor



}
