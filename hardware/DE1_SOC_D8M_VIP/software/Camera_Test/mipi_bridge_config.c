/*
 * mipi_bridge_config.c
 *
 *  Created on: 2015�~4��22��
 *      Author: Administrator
 */

#include <stdio.h>
#include "I2C_core.h"
#include "terasic_includes.h"
#include "mipi_bridge_config.h"

typedef struct
{
	uint16_t Addr;
	uint16_t Data;
} SZ_MIPI_REG_T;

#define FIFO_LEVEL 8 // try others? [0~511]
#define DATA_FORMAT 0x0010

// REFCLK    20 MHz
// PPIrxCLK  100 MHz
// PCLK      25 MHz
// MCLK      25 MHz

#define PLL_PRD 1  // 0- 15
#define PLL_FBD 39 // 0-511
#define PLL_FRS 1  // 0-3

#define MCLK_HL 1 // (MCLK_HL+1)+ (MCLK_HL+1)

// 2b'00: div 8, 2b'01: div 4, 2b'10: div 2
#define PPICLKDIV 2	 // ppi_clk:must between 66~125MHz
#define MCLKREFDIV 2 // mclkref clock:  < 125MHz
#define SCLKDIV 0	 // sys_clk clock:  < 100MHz

#define WORDCOUNT 800

static SZ_MIPI_REG_T MipiBridgeReg[] = {

	{0x0002, 0x0001},											 // System Control Register
	{0xFFFF, 10},												 // delay
	{0x0002, 0x0000},											 // System Control Register
	{0x0016, ((PLL_PRD << 12) + PLL_FBD)},						 // PLL Control Register 0
	{0x0018, ((PLL_FRS << 10) + (0x2 << 8) + (0x1 << 1) + 0x1)}, // PLL Control Register 1
	{0xFFFF, 10},												 // delay

	{0x0018, ((PLL_FRS << 10) + (0x2 << 8) + (0x1 << 4) + (0x1 << 1) + 0x1)}, // PLL Control Register 1
	{0x0020, ((PPICLKDIV << 4) + (MCLKREFDIV << 2) + SCLKDIV)},				  // PLL Control Register 0
	{0x000C, ((MCLK_HL << 8) + MCLK_HL)},									  // MCLK Control Register
	{0x0060, 0x8006},
	{0x0006, FIFO_LEVEL}, // FiFo Control Register   [0~511]
	// when reaches to this level FiFo controller asserts FiFoRdy for Parallel port to start output data
	{0x0008, DATA_FORMAT}, // Data FormatControl Register
	//	{0x0022,WORDCOUNT}, //Word Count Register
	{0x0004, 0x8047} // Configuration Control Register

};

uint16_t nSWAP16(uint16_t x)
{
	uint16_t y;
	// y =  (((x) >> 8) & 0xff) | (((x) & 0xff) << 8);

	//	y = x;
	y = (x >> 8) & 0x00ff;
	y |= (x << 8) & 0xff00;
	return y;
}

void MipiBridgeRegWrite(uint16_t Addr, uint16_t Value)
{
	const uint8_t device_address = MIPI_BRIDGE_I2C_ADDR;
	OC_I2CL_Write(I2C_OPENCORES_MIPI_BASE, device_address, Addr, (uint8_t *)&Value, sizeof(Value));
}

uint16_t MipiBridgeRegRead(uint16_t Addr)
{
	uint16_t Value, tValue;
	const uint8_t device_address = MIPI_BRIDGE_I2C_ADDR;

	OC_I2CL_Read(I2C_OPENCORES_MIPI_BASE, device_address, Addr, (uint8_t *)&Value, sizeof(Value));

	tValue = nSWAP16(Value);

	return (tValue);
}

void MipiBridgeInit(void)
{

	uint16_t data;
	int i, num;

	printf("\nStart MipiBridgeInit!\n");

	data = MipiBridgeRegRead(0x0000); // read chip and revision id;

	printf("Chip and Revision ID is 0x%04xh(expected: 0x4401);\n", data);

	num = sizeof(MipiBridgeReg) / sizeof(MipiBridgeReg[0]);

	for (i = 0; i < num; i++)
	{
		if (MipiBridgeReg[i].Addr == 0xFFFF)
			sleep_us(MipiBridgeReg[i].Data * 1000);
		else
			MipiBridgeRegWrite(MipiBridgeReg[i].Addr, MipiBridgeReg[i].Data);
	}

	printf("End MipiBridgeInit!\n\n");
}
