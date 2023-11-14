
#ifndef ACCELERO_H
#define ACCELERO_H
#include "stm32f10x.h"
#include "MySPI.h"

void Accelero_Init (SPI_TypeDef * SPI);
char MySPI_Read(void);

#endif