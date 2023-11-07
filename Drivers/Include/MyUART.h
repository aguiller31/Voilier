#ifndef MYUART_H
#define MYUART_H
#include "stm32f10x.h"

void MyUSART_Init(USART_TypeDef * USART,  int baud_rate, double stop_bits, char parity);
void MyUSART_WriteCharacter(USART_TypeDef * USART, char lettre);
void  MyUSART_ActiveIT(USART_TypeDef * Timer , char Prio ,void (*IT_function )(void));


#endif
