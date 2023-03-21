// --------------------------------------------------------------------
// Copyright (c) 2010 by Terasic Technologies Inc. 
// --------------------------------------------------------------------
//
// Permission:
//
//   Terasic grants permission to use and modify this code for use
//   in synthesis for all Terasic Development Boards and Altera Development 
//   Kits made by Terasic.  Other use of this code, including the selling 
//   ,duplication, or modification of any portion is strictly prohibited.
//
// Disclaimer:
//
//   This VHDL/Verilog or C/C++ source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Terasic provides no warranty regarding the use 
//   or functionality of this code.
//
// --------------------------------------------------------------------
//           
//                     Terasic Technologies Inc
//                     356 Fu-Shin E. Rd Sec. 1. JhuBei City,
//                     HsinChu County, Taiwan
//                     302
//
//                     web: http://www.terasic.com/
//                     email: support@terasic.com
//
// --------------------------------------------------------------------
#ifndef _TERASIC_INCLUDES_H_
#define _TERASIC_INCLUDES_H_
#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <string.h>
#include <stddef.h>
#include <unistd.h>  // sleep_us (unix standard?)
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <string.h>
#include "Qsys.h"

typedef int bool;
#define TRUE    1
#define FALSE   0

#define DEFAULT_LEVEL 3

// From DE1 Manual (https://people.ece.cornell.edu/land/courses/ece5760/DE1_SOC/SoC-FPGA%20Design%20Guide_EPFL.pdf)
#define LW_BRIDGE_BASE (0xff200000)
#define LW_BRIDGE_SPAN (0x00200000) // 2MB
#define HPS_SDRAM_BASE     (0xC0000000)
#define HPS_SDRAM_SPAN     (0x03FFFFFF)

void * lw_bridge_ptr;
void IOWR(uint32_t base_addr, uint32_t offset, uint32_t data);
uint32_t IORD(uint32_t base_addr, uint32_t offset);
void sleep_us(unsigned int sleep);

void mipi_clear_error(void);
void mipi_show_error_info(void);
void mipi_show_error_info_more(void);
bool MIPI_Init(void);

#define DEBUG_DUMP  /*printf */ 

#endif