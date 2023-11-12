#ifndef COMMUNICATION_SERVICE
#define COMMUNICATION_SERVICE
#include "stm32f10x.h"
#include "MyUART.h"
#include "MyGPIO.h"

#define TRIBORD 1
#define BABORD 0

#define ALERT_DLR 0
#define ALERT_LOW_BATTERY 1

typedef struct CommunicationService
{
	void(*Free)(struct CommunicationService*);
	void (*Start)(struct CommunicationService*);
	void (*WriteCharacter)(struct CommunicationService*, char);
	void (*WriteString)(struct CommunicationService*,  char*);
	void (*WriteStringNL)(struct CommunicationService*,  char*);
	void (*SendAlert)(struct CommunicationService*, int);
	void (*SendNewLine)(struct CommunicationService*);
	void (*RegisterReadChar)(struct CommunicationService*, signed char,void (*function )());
	void (*RegisterReadBytes)(struct CommunicationService*,void (*function )(signed char));
	void (*RegisterReadDirection)(struct CommunicationService*,int,void (*function )(signed char));
	void (*Read)(struct CommunicationService*);
	
	UARTDriver * UART;
	int UART_nb;
} CommunicationService ;
 CommunicationService * New_Communication(void);


/*
void init_communication(USART_TypeDef *);
void write_communication(char);
void registerFunction_communication(char, void (*function )());
void read_communication(void);
int get_Pupitre_Adress(int);
*/
#endif
