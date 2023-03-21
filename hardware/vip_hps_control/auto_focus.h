/*
 * auto_focus.h
 *
 *  Created on: 2015��7��27��
 *      Author: Administrator
 */

// write
#ifndef AUTO_FOCUS_H_
#define AUTO_FOCUS_H_
#define REG_GO    			0
#define REG_CTRL   			1
#define REG_FOCUS_W        2
#define REG_FOCUS_H        3
#define REG_FOCUS_X_START  4
#define REG_FOCUS_Y_START  5
#define REG_SCAL           6  // scan 0 -> 1023 , step: SCAL , to find STEP_UP
                              // scan STEP_UP + - SCAL/2 , step: SCAL_F
#define REG_TH             7

// read
#define REG_STATUS         0

void Focus_Init(void);
uint16_t Focus_Window(int x,int y);
int Focus_Released(void);

#endif