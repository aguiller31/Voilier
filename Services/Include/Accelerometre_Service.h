#ifndef ACCELERO_H
#define ACCELERO_H
#include "stm32f10x.h"
#include "MySPI.h"
#include "GPIO_Driver.h"
#include "conf.h"

#define first_bit 0x80
#define BW_RATE_W 0x2C

#define POWER_CTL_W 0x2D
#define POWER_CTL_R 0xAD

#define DATA_FORMAT_W 0x31
#define DATA_FORMAT_R 0xB1

#define X0_R_MB 0xF2

void Accelero_Init (SPI_TypeDef * SPI);
void wait(void);
//char MySPI_Read(void);
int Angle_Roulis (SPI_TypeDef * SPI);

#endif
