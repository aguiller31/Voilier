#include "Communication.h"
#include <stdlib.h>

typedef void (*FunctionPointer)();
typedef FunctionPointer FunctionArray[128];

FunctionArray functionTable[3];

void setGPIO_communication(GPIO_TypeDef * GPIO, int rx, int tx){
		MyGPIO_Init(GPIO,tx,AltOut_Ppull); //TX
		MyGPIO_Init(GPIO,rx,In_Floating); //RX
}

void CommunicationService_Start(CommunicationService *This){
	GPIO_TypeDef * GPIO = (This->UART_nb == 1 || This->UART_nb == 2) ? GPIOA : GPIOB;
	int rx = (This->UART_nb == 1)? 10 :(This->UART_nb == 2) ? 3 : 11;
	int tx = (This->UART_nb == 1)? 9 :(This->UART_nb == 2) ? 2 : 10;
	setGPIO_communication(GPIO,rx,tx);
	
	This->UART->SetBaudRate(This->UART, 9600);
	This->UART->SetWordLength(This->UART, 1);
	This->UART->SetStopBits(This->UART, 1.0);
	This->UART->SetParityControl(This->UART, 0);
	This->UART->EnableTX(This->UART);
	This->UART->EnableRX(This->UART);
	This->UART->Start(This->UART);
}
void CommunicationService_WriteCharacter(CommunicationService *This, char c){
	This->UART->WriteCharacter(This->UART, c);
}


//permet d'enregistrer une fonction callback pour chaque caractère recu et pour chaque USART
void CommunicationService_RegisterRead(CommunicationService *This, char c, void (*function )()) {
    functionTable[This->UART_nb][(int)c] = function;
}
void Callback(char c, int UART_nb){
	functionTable[UART_nb][(int)c]();
}

void CommunicationService_ReadCharacter(CommunicationService *This){ 
	void ( * Callback_pointeur ) ( char,int ) ; 
	Callback_pointeur = Callback ;
	This->UART->ActiveIT(This->UART,0,Callback_pointeur);
}
static void  CommunicationService_Init( CommunicationService *This)
{
			This->Start = CommunicationService_Start;
			This->WriteCharacter = CommunicationService_WriteCharacter;
			This->RegisterRead = CommunicationService_RegisterRead;
			This->ReadCharacter = CommunicationService_ReadCharacter;
}
void CommunicationService_New_Free(CommunicationService *This)
{
				This->UART->Free(This->UART);
        free(This);        
}
 CommunicationService * New_Communication(USART_TypeDef * USART)
{
       CommunicationService *This = malloc(sizeof(CommunicationService));
       if(!This) return NULL;
       CommunicationService_Init(This);
			 This->Free = CommunicationService_New_Free;
			 This->UART = New_UART(USART);
				This->UART_nb = (USART == USART1)?1 : (USART == USART2)? 2 :3;
       return This;
}

//baud_rate = 9600 Bd, 19200 Bd, 38400 Bd,..., 115200 Bd
//stop_bits = 1, 1.5, 2
//parité : c’est un 9ieme bit qui vaut 1 selon que le nombre de bit à 1 est pair ou impair (on parle de parité paire et impaire).

/*
// Définition du type de fonction
typedef void (*FunctionPointer)();
FunctionPointer functionTable[128];

//permet d'enregistrer une fonction callback pour chaque caractère recu
void registerFunction_communication(char character, void (*function )()) {
    functionTable[(int)character] = function;
}
//baud_rate = 9600 Bd, 19200 Bd, 38400 Bd,..., 115200 Bd
//stop_bits = 1, 1.5, 2
//parité : c’est un 9ieme bit qui vaut 1 selon que le nombre de bit à 1 est pair ou impair (on parle de parité paire et impaire).
void Callback(char letter){
    if (functionTable[(int)letter]) {
        functionTable[(int)letter]();
    }
}
void setGPIO_communication(GPIO_TypeDef * GPIO, int rx, int tx){
		MyGPIO_Init(GPIO,tx,AltOut_Ppull); //TX
		MyGPIO_Init(GPIO,rx,In_Floating); //RX
}
void init_communication(USART_TypeDef * USART){
	GPIO_TypeDef * GPIO = (USART == USART1 || USART == USART2) ? GPIOA : GPIOB;
	int rx = (USART == USART1)? 10 :(USART == USART2) ? 3 : 11;
	int tx = (USART == USART1)? 9 :(USART == USART2) ? 2 : 10;

	setGPIO_communication(GPIO,rx,tx);
	
	MyUSART_Init(USART, 9600, 1, 0);
}
void write_communication(char letter){
		MyUSART_WriteCharacter(USART2, letter);
}
void read_communication(){ 
	void ( * Callback_pointeur ) ( char ) ; 
	Callback_pointeur = Callback ;
	MyUSART_ActiveRXIT(USART2,0,Callback_pointeur);
}
int get_Pupitre_Adress(int address_voilier){
	return 80+address_voilier;
}
*/