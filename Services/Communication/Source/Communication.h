#ifndef COMMUNICATION
#define COMMUNICATION
#include "stm32f10x.h"
#include "MyUART.h"
#include "MyGPIO.h"

typedef struct CommunicationService
{
	void(*Free)(struct CommunicationService*);
	void (*Start)(struct CommunicationService*);
	void (*WriteCharacter)(struct CommunicationService*, char);
	void (*RegisterRead)(struct CommunicationService*, char,void (*function )());
	void (*ReadCharacter)(struct CommunicationService*);
	UARTDriver * UART;
	int UART_nb;
} CommunicationService ;
 CommunicationService * New_Communication(USART_TypeDef * USART);


/*
void init_communication(USART_TypeDef *);
void write_communication(char);
void registerFunction_communication(char, void (*function )());
void read_communication(void);
int get_Pupitre_Adress(int);
*/
#endif
