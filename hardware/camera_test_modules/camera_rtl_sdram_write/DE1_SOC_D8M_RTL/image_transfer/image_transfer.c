#include "system.h"
#include "nios2.h"
#include "sys/alt_stdio.h"
#include "string.h"
#include "stdlib.h"

#define IMG_TRANSFER_BASE   0x1000
#define PIXEL_RDY           IMG_TRANSFER_BASE + 0
#define ACK_PIXEL           IMG_TRANSFER_BASE + 4
#define PIXEL_DATA          IMG_TRANSFER_BASE + 8

unsigned int curr_pixel_data;
// unsigned int pixel_count;

unsigned int pixels[320];
unsigned int idx;

int main(void) {
    unsigned int pixel_ready;
    idx = 0;

    volatile int *PIX_RDY_PTR = (int *) IMG_TRANSFER_BASE;
    volatile int *ACK_PTR = (int *) ACK_PIXEL;
    volatile int *PIX_DATA_PTR = (int *) PIXEL_DATA;

    // pixel_count = 0;
    while (1) {
        // image transfer monitor
        pixel_ready = *PIX_RDY_PTR;
        if (pixel_ready == 1) {
            // turn LED
            while(pixel_ready != 2) {
                if(pixel_ready == 0) {   //next pixel not ready, wait
                    pixel_ready = *PIX_RDY_PTR;
                } else {    //next pixel ready, acknowledge & process
                    *ACK_PTR = 1;
                    curr_pixel_data = *PIX_DATA_PTR;
                    // pixel_count++;
                    *ACK_PTR = 0;

                    pixel_ready = *PIX_RDY_PTR;

                    if(idx < 320) {
                        pixels[idx] = curr_pixel_data;
                        idx++;
                    }
                    // turn off LED

                    // if (pixel_count == 640 * 480 / 256) {
                    //     // full image captured
                    // }
                }
                
            }
            
            break;
        }

        alt_printf("\nDone pixels.");
        
        for(unsigned int i = 0 ; i < 320; i++) {
            alt_printf("\n Pixel %x: %x", i, pixels[i]);
        }

        // wifi monitor


        // touch screen monitor

    }

}