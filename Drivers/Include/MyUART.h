#ifndef MYUART_H
#define MYUART_H
#include "stm32f10x.h"
typedef struct UARTDriver
{
	void(*Free)(struct UARTDriver*);
	void(*ClearDR)(struct UARTDriver*);
	void(*EnableTX)(struct UARTDriver*);
	void(*DisableTX)(struct UARTDriver*);
	void(*EnableRX)(struct UARTDriver*);
	void(*DisableRX)(struct UARTDriver*);
	void (*SetBaudRate)(struct UARTDriver*, int);
	void (*SetStopBits)(struct UARTDriver*, double);
	void (*SetParityControl)(struct UARTDriver*, char);
	void (*SetWordLength)(struct UARTDriver*, char);
	void (*Start)(struct UARTDriver*);
	void (*WriteCharacter)(struct UARTDriver*, char);
	void (*ActiveIT)(struct UARTDriver *, char ,void (*IT_function )(char,int));
	USART_TypeDef * UART;
	double StopBits;
	char Parity;
} UARTDriver ;
 UARTDriver * New_UART(USART_TypeDef * USART);
/*
void MyUSART_Init(USART_TypeDef * USART,  int baud_rate, double stop_bits, char parity);
void MyUSART_WriteCharacter(USART_TypeDef * USART, char lettre);
void MyUSART_ActiveRXIT(USART_TypeDef * Timer , char Prio ,void (*IT_function )(char));

*/


#endif
