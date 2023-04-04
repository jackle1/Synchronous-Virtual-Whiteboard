#ifndef MIPI_CAMERA_CONFIG_H_
#define MIPI_CAMERA_CONFIG_H_

#define MIPI_I2C_ADDR   0x6C
#define MIPI_AF_I2C_ADDR 0x18

#define BYD_MODE_SELECT   0x0100
#define BYD_SOFTWARE_SET  0x0103
#define BYD_STREAMING     0x301A


#define BIN_LEVEL_MAX    3
#define BIN_LEVEL_MIN    1



void OV8865_FOCUS_Move_to(uint16_t a_u2MovePosition);
void MIPI_BIN_LEVEL(uint8_t level);
void BLC_LEVEL(uint8_t blc0,uint8_t blc1);

void MipiCameraInit(void);

#endif