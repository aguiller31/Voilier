#include "Communication.h"
#include <stdlib.h>

//CONFIGURATION
#define USART1_GPIO GPIOA
#define USART1_GPIO_RX 10
#define USART1_GPIO_TX 9

#define USART2_GPIO GPIOA
#define USART2_GPIO_RX 3
#define USART2_GPIO_TX 2

#define USART3_GPIO GPIOB
#define USART3_GPIO_RX 11
#define USART3_GPIO_TX 10

#define BAUD_RATE 9600
#define WORD_LENGTH 1
#define STOP_BITS 1.0
#define PARITY_CONTROL 0



//CODE
char *alerts[] = {
	"Detection Limite du Roulis",
	"Tension de batterie trop faible"
};
typedef void (*FunctionPointer)();
typedef FunctionPointer FunctionArray[128]; //tableaux des 128 fonctions pour les caractères

FunctionArray functionTable[3]; //tableaux des 128 fonctions pour les 3 USART

typedef void (*FunctionPointerBytes)(signed char);
FunctionPointerBytes functionTableBytes[3];

typedef void (*FunctionPointerDirection)(signed char);
typedef FunctionPointerDirection FunctionArrayDirection[2]; //tableaux des 2 fonctions pour tribord et babord
FunctionArrayDirection functionTableDirection[3];


void setGPIO_communication(GPIO_TypeDef * GPIO, int rx, int tx){
		MyGPIO_Init(GPIO,tx,AltOut_Ppull); //TX
		MyGPIO_Init(GPIO,rx,In_Floating); //RX
}

void CommunicationService_Start(CommunicationService *This){
	GPIO_TypeDef * GPIO = (This->UART_nb == 1)? USART1_GPIO :(This->UART_nb == 2) ? USART2_GPIO : USART3_GPIO;
	int rx = (This->UART_nb == 1)? USART1_GPIO_RX :(This->UART_nb == 2) ? USART2_GPIO_RX : USART3_GPIO_RX;
	int tx = (This->UART_nb == 1)? USART1_GPIO_TX :(This->UART_nb == 2) ? USART2_GPIO_TX : USART3_GPIO_TX;
	setGPIO_communication(GPIO,rx,tx);
	
	This->UART->SetBaudRate(This->UART, BAUD_RATE);
	This->UART->SetWordLength(This->UART, WORD_LENGTH);
	This->UART->SetStopBits(This->UART, STOP_BITS);
	This->UART->SetParityControl(This->UART, PARITY_CONTROL);
	This->UART->EnableTX(This->UART);
	This->UART->EnableRX(This->UART);
	This->UART->Start(This->UART);
}
void CommunicationService_WriteCharacter(CommunicationService *This, char c){
	This->UART->WriteCharacter(This->UART, c);
}
void CommunicationService_WriteString(CommunicationService *This, char *s){
	This->UART->WriteString(This->UART, s);
}
void CommunicationService_SendNewLine(CommunicationService *This){
	This->UART->WriteString(This->UART, "\r\n");
}
void CommunicationService_WriteStringNL(CommunicationService *This, char *s){
	This->UART->WriteString(This->UART, s);
	This->SendNewLine(This);
}
void CommunicationService_SendAlert(CommunicationService *This, int n){
	This->WriteStringNL(This, alerts[n]);
}
//permet d'enregistrer une fonction callback pour chaque caractère recu et pour chaque USART
void CommunicationService_RegisterReadChar(CommunicationService *This, signed char c, void (*function )()) {
    functionTable[This->UART_nb][(int)c] = function;
}
void CommunicationService_RegisterReadBytes(CommunicationService *This, void (*function )(signed char)) {
    functionTableBytes[This->UART_nb] = function;
}
void CommunicationService_RegisterReadDirection(CommunicationService *This, int direction, void (*function )(signed char)) {
    functionTableDirection[This->UART_nb][direction] = function;
}
void Callback(signed char c, int UART_nb){
	if( c >= 0x0 & c <= 0x7F){ // compris entre le char(0) et le char(128)
		if(functionTable[UART_nb][(int)c]){
			functionTable[UART_nb][(int)c]();
		}
		else if(c > 0 & c <= 100){ // entre 0 et 100
			functionTableDirection[UART_nb][TRIBORD](c);
		}
		else{
			functionTableBytes[UART_nb](c);
		}
	} // caractère non ASCII
	else{
		
		if(c >= -100 & c <= -1){// compris entre -100 et -1
				functionTableDirection[UART_nb][BABORD](c);
			}else{ //ce qui est < à -100
				functionTableBytes[UART_nb](c);
			}
		}
}

void CommunicationService_Read(CommunicationService *This){ 
	void ( * Callback_pointeur ) ( signed char,int ) ; 
	Callback_pointeur = Callback ;
	This->UART->ActiveIT(This->UART,0,Callback_pointeur);
}
static void  CommunicationService_Init( CommunicationService *This)
{
			This->Start = CommunicationService_Start;
			This->WriteCharacter = CommunicationService_WriteCharacter;
			This->WriteString = CommunicationService_WriteString;
			This->SendAlert = CommunicationService_SendAlert;
			This->SendNewLine = CommunicationService_SendNewLine;
			This->WriteStringNL = CommunicationService_WriteStringNL;
			This->RegisterReadChar = CommunicationService_RegisterReadChar;
			This->RegisterReadBytes = CommunicationService_RegisterReadBytes;
			This->RegisterReadDirection = CommunicationService_RegisterReadDirection;
			This->Read = CommunicationService_Read;
	
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
