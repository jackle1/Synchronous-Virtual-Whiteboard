/*
 * mipi_bridge_config.h
 *
 *  Created on: 2015��4��22��
 *      Author: Administrator
 */
#ifndef MIPI_BRIDGE_CONFIG_H_
#define MIPI_BRIDGE_CONFIG_H_

#define MIPI_BRIDGE_I2C_ADDR   0x1C  // 8'b0001_1100 - 7'b0E + 1'b0 (write bit)

void MipiBridgeInit(void);

void MipiBridgeRegWrite(uint16_t Addr, uint16_t Value);
uint16_t MipiBridgeRegRead(uint16_t Addr);

#endif