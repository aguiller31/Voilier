#ifndef MYUART_H
#define MYUART_H
#include "stm32f10x.h"
typedef struct UARTClass
{
	void(*Free)(struct UARTClass*);
	void(*ClearDR)(struct UARTClass*);
	void(*EnableTX)(struct UARTClass*);
	void(*DisableTX)(struct UARTClass*);
	void(*EnableRX)(struct UARTClass*);
	void(*DisableRX)(struct UARTClass*);
	void (*SetBaudRate)(struct UARTClass*, int);
	void (*SetStopBits)(struct UARTClass*, double);
	void (*SetParityControl)(struct UARTClass*, char);
	void (*SetWordLength)(struct UARTClass*, char);
	void (*Start)(struct UARTClass*);
	void (*WriteCharacter)(struct UARTClass*, char);
	void (*ActiveIT)(struct UARTClass *, char ,void (*IT_function )(char,int));
	USART_TypeDef * UART;
	double StopBits;
	char Parity;
} UARTClass ;
 UARTClass * New_UART(USART_TypeDef * USART);
/*
void MyUSART_Init(USART_TypeDef * USART,  int baud_rate, double stop_bits, char parity);
void MyUSART_WriteCharacter(USART_TypeDef * USART, char lettre);
void MyUSART_ActiveRXIT(USART_TypeDef * Timer , char Prio ,void (*IT_function )(char));

*/


#endif
