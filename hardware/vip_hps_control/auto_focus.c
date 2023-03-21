/*
 * auto_focus.c
 *
 *  Created on: 2015��7��27��
 *      Author: Administrato
 */

#include <stdio.h>
#include "I2C_core.h"
#include "auto_focus.h"


/////////////////////////////////
//This is a simple focus function for demonstration, focus function may be not good for some situation
//arithmetic not suit for: 1. microspur 2.light source
  // please look at TERASIC_AUTO_FOCUS IP to see register's detail.
uint16_t video_w =  640;
uint16_t video_h =  480;

uint16_t focus_width  = 240;  // <video w
uint16_t focus_height = 180; // <video h
// please observe focus performance when change the scal ,scal_f. or when change camera frame rate
uint8_t  focus_scal   =   4;  // scan 0 -> 1023 , step: SCAL , to find STEP_UP
uint8_t  focus_scal_f =   1;  //  scan STEP_UP + - SCAL/2 , step: SCAL_F
uint8_t  focus_th     =   20;

void Focus_Init(void){
  // please look at TERASIC_AUTO_FOCUS IP to see register's detail.
  IOWR(TERASIC_AUTO_FOCUS_0_BASE,REG_CTRL, 0);// focus mode : 1: window-screen, 0: full-screen
  IOWR(TERASIC_AUTO_FOCUS_0_BASE,REG_FOCUS_W, focus_width);// focus_width
  IOWR(TERASIC_AUTO_FOCUS_0_BASE,REG_FOCUS_H, focus_height);// focus_height
  IOWR(TERASIC_AUTO_FOCUS_0_BASE,REG_FOCUS_X_START, video_w/2-focus_width/2);//x_start
  IOWR(TERASIC_AUTO_FOCUS_0_BASE,REG_FOCUS_Y_START, video_h/2-focus_height/2);// y_start

  IOWR(TERASIC_AUTO_FOCUS_0_BASE,REG_SCAL, focus_scal*256 + focus_scal_f); // scan 0 -> 1023 , step: SCAL , to find STEP_UP
                                                                          //  scan STEP_UP + - SCAL/2 , step: SCAL_F
  IOWR(TERASIC_AUTO_FOCUS_0_BASE,REG_TH, focus_th);

  //////////// focus at initial time
  sleep_us(100);
  IOWR(TERASIC_AUTO_FOCUS_0_BASE,REG_GO, 1);
  sleep_us(2);
  IOWR(TERASIC_AUTO_FOCUS_0_BASE,REG_GO, 0);
}

uint16_t Focus_Window(int x,int y){
   uint16_t x_start,y_start;
   uint16_t end_focus;

   if(Focus_Released()) { // pre focus done
	   IOWR(TERASIC_AUTO_FOCUS_0_BASE,REG_CTRL, 1);// focus mode : 1: window-screen, 0: full-screen

	   if(( x -  focus_width/2) < 0 ) x_start = 0;
	   else if(( x + focus_width/2 ) > video_w )  x_start = video_w -1 -focus_width;
	   else x_start = x - focus_width/2;

	   if(( y -  focus_height/2) < 0 ) y_start = 0;
	   else if(( y + focus_height/2 ) > video_h )  y_start = video_h -1 -focus_height;
	   else y_start = y - focus_height/2;

	   printf("x_start= %d,y_start= %d\n",x_start,y_start);

	   IOWR(TERASIC_AUTO_FOCUS_0_BASE,REG_FOCUS_X_START, x_start);//x_start
	   IOWR(TERASIC_AUTO_FOCUS_0_BASE,REG_FOCUS_Y_START, y_start);//y_start

	   sleep_us(10);

	   IOWR(TERASIC_AUTO_FOCUS_0_BASE,REG_GO, 1);
	   sleep_us(2);
	   IOWR(TERASIC_AUTO_FOCUS_0_BASE,REG_GO, 0);
	   Focus_Released();

	   end_focus = IORD(TERASIC_AUTO_FOCUS_0_BASE,REG_STATUS)&0x0FFF;
	   printf("end_focus = %d \n",end_focus);

	   return end_focus;

   }
   return end_focus;

}

int Focus_Released(void){
  int Released = FALSE;
  // uint32_t TimeOut;

  // TimeOut = alt_nticks() + alt_ticks_per_second()*8;

  // while((IORD(TERASIC_AUTO_FOCUS_0_BASE,REG_STATUS)&0x8000) ==0 && alt_nticks() < TimeOut ); // waiting for VCM release I2C bus

  // if(alt_nticks() < TimeOut ) Released = TRUE;
  // else printf("\n =>��Released check TimeOut!\n");

  // sleep_us(10000);

  return Released;
}
