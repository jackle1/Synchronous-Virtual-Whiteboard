/*
 * I2C_ocre.h
 *
 *  Created on: 2014-6-3
 *      Author: niubility
 */
#ifndef I2C_CORE_H_
#define I2C_CORE_H_

#ifndef _TERASIC_INCLUDES_H_
#include "terasic_includes.h"
#endif

#define DEBUG_I2C

bool  oc_i2c_init(uint32_t i2c_base);
bool  oc_i2c_init_ex(uint32_t i2c_base, uint32_t ref_clk, uint32_t i2c_clk);
bool  oc_i2c_uninit(uint32_t i2c_base);
bool  ACK_single_check(uint32_t i2c_base);
bool  ACK_check(uint32_t i2c_base);
bool  OC_I2C_Write(uint32_t i2c_base,uint8_t device_address,uint8_t sub_address, uint8_t *pData, int nWriteLength);
bool  ACK_judge_for_read(uint32_t i2c_base);
bool  OC_I2C_Read(uint32_t i2c_base,uint8_t device_address,uint8_t sub_address, uint8_t *pData8, int nReadLength);
bool  OC_I2CL_Write(uint32_t i2c_base,uint8_t device_address,uint16_t sub_address,uint8_t *pData, int nWriteLength);
bool  OC_I2CL_Read(uint32_t i2c_base,uint8_t device_address, uint16_t sub_address, uint8_t *pData8, int nReadLength);

//
bool  OC_I2C_Read_Continue(uint32_t i2c_base,uint8_t device_address, uint8_t *pData8, int nReadLength);

#endif