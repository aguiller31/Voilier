#ifndef MYUART_H
#define MYUART_H
#include "stm32f10x.h"

void MyUSART_Init(USART_TypeDef * USART, int message_size, int message_period, double stop_bits, char parity);
#endif
