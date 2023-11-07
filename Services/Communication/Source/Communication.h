#ifndef COMMUNICATION
#define COMMUNICATION
#include "stm32f10x.h"
void MyUSART_Init(USART_TypeDef * USART, int message_size, int message_period, double stop_bits, char parity);
int get_Pupitre_Adress(int);
#endif
