#include "Communication_Service.h"
#include <stdlib.h>
#include "conf.h"
//CONFIGURATION
#ifndef CONFIG_H
#define COMMUNICATION_UART_READ_IT_PRIORITY 3
	
#define COMMUNICATION_UART USART1
#define COMMUNICATION_BAUD_RATE 9600
#define COMMUNICATION_WORD_LENGTH 1
#define COMMUNICATION_STOP_BITS 1.0
#define COMMUNICATION_PARITY_CONTROL 0


#define USART1_GPIO GPIOA
#define USART1_GPIO_RX_PIN 10
#define USART1_GPIO_TX_PIN 9

#define USART2_GPIO GPIOA
#define USART2_GPIO_RX_PIN 3
#define USART2_GPIO_TX_PIN 2

#define USART3_GPIO GPIOB
#define USART3_GPIO_RX_PIN 11
#define USART3_GPIO_TX_PIN 10

#define COMMUNICATION_ALERT_ROULIS "Detection Limite du Roulis"
#define COMMUNICATION_ALERT_BATTERY "Tension de batterie trop faible"
#endif


//CODE
char *alerts[] = {
	COMMUNICATION_ALERT_ROULIS,
	COMMUNICATION_ALERT_BATTERY
};


// Définition du type de fonction pour les caractères
typedef void (*FunctionPointer)();

// Tableau des 128 fonctions pour les caractères
typedef FunctionPointer FunctionArray[128];

// Tableaux des fonctions pour les 3 USART
FunctionArray functionTable[3];

// Tableaux des fonctions pour les 3 USART, prenant un argument signed char
typedef void (*FunctionPointerBytes)(signed char);
FunctionPointerBytes functionTableBytes[3];

// Tableaux des fonctions pour les 3 USART, prenant un argument signed char et direction
typedef void (*FunctionPointerDirection)(signed char);
typedef FunctionPointerDirection FunctionArrayDirection[2]; //tableaux des 2 fonctions pour tribord et babord
FunctionArrayDirection functionTableDirection[3];

// Fonction pour configurer les GPIO pour la communication
void setGPIO_communication(GPIO_TypeDef * GPIO, int rx, int tx){
	MyGPIO_Init(GPIO,tx,AltOut_Ppull); // TX
	MyGPIO_Init(GPIO,rx,In_Floating);  // RX
}

/**
 * @brief Démarre le service de communication.
 * @details Configure les paramètres de l'UART et active la transmission et la réception.
 * 
 * @param This Pointeur vers l'instance du service de communication.
 */
void CommunicationService_Start(CommunicationService *This){
	GPIO_TypeDef * GPIO = (This->UART_nb == 1) ? USART1_GPIO : (This->UART_nb == 2) ? USART2_GPIO : USART3_GPIO;
	int rx = (This->UART_nb == 1) ? USART1_GPIO_RX_PIN : (This->UART_nb == 2) ? USART2_GPIO_RX_PIN : USART3_GPIO_RX_PIN;
	int tx = (This->UART_nb == 1) ? USART1_GPIO_TX_PIN : (This->UART_nb == 2) ? USART2_GPIO_TX_PIN : USART3_GPIO_TX_PIN;
	setGPIO_communication(GPIO,rx,tx);
	
	This->UART->SetBaudRate(This->UART, COMMUNICATION_BAUD_RATE);
	This->UART->SetWordLength(This->UART, COMMUNICATION_WORD_LENGTH);
	This->UART->SetStopBits(This->UART, COMMUNICATION_STOP_BITS);
	This->UART->SetParityControl(This->UART, COMMUNICATION_PARITY_CONTROL);
	This->UART->EnableTX(This->UART);
	This->UART->EnableRX(This->UART);
	This->UART->Start(This->UART);
}

/**
 * @brief Envoie un caractère par l'UART.
 * 
 * @param This Pointeur vers l'instance du service de communication.
 * @param c Caractère à envoyer.
 */
void CommunicationService_WriteCharacter(CommunicationService *This, char c){
	This->UART->WriteCharacter(This->UART, c);
}

/**
 * @brief Envoie une chaîne de caractères par l'UART.
 * 
 * @param This Pointeur vers l'instance du service de communication.
 * @param s Chaîne de caractères à envoyer.
 */
void CommunicationService_WriteString(CommunicationService *This, char *s){
	This->UART->WriteString(This->UART, s);
}

/**
 * @brief Envoie une nouvelle ligne par l'UART.
 * 
 * @param This Pointeur vers l'instance du service de communication.
 */
void CommunicationService_SendNewLine(CommunicationService *This){
	This->UART->WriteString(This->UART, "\r\n");
}

/**
 * @brief Envoie une chaîne de caractères suivie d'une nouvelle ligne par l'UART.
 * 
 * @param This Pointeur vers l'instance du service de communication.
 * @param s Chaîne de caractères à envoyer.
 */
void CommunicationService_WriteStringNL(CommunicationService *This, char *s){
	This->UART->WriteString(This->UART, s);
	This->SendNewLine(This);
}

/**
 * @brief Envoie un message d'alerte par l'UART.
 * 
 * @param This Pointeur vers l'instance du service de communication.
 * @param n Indice du message d'alerte.
 */
void CommunicationService_SendAlert(CommunicationService *This, int n){
	This->WriteStringNL(This, alerts[n]);
}

/**
 * @brief Enregistre une fonction callback pour chaque caractère reçu et pour chaque UART.
 * 
 * @param This Pointeur vers l'instance du service de communication.
 * @param c Caractère pour lequel la fonction callback est enregistrée.
 * @param function Fonction callback.
 */
void CommunicationService_RegisterReadChar(CommunicationService *This, signed char c, void (*function)()) {
    functionTable[This->UART_nb-1][(int)c] = function;
}

/**
 * @brief Enregistre une fonction callback pour la réception de bytes.
 * 
 * @param This Pointeur vers l'instance du service de communication.
 * @param function Fonction callback.
 */
void CommunicationService_RegisterReadBytes(CommunicationService *This, void (*function)(signed char)) {
    functionTableBytes[This->UART_nb-1] = function;
}

/**
 * @brief Enregistre une fonction callback pour la réception de direction.
 * 
 * @param This Pointeur vers l'instance du service de communication.
 * @param direction Direction (Tribord ou Babord).
 * @param function Fonction callback.
 */
void CommunicationService_RegisterReadDirection(CommunicationService *This, int direction, void (*function)(signed char)) {
    functionTableDirection[This->UART_nb-1][direction] = function;
}

/**
 * @brief Fonction callback pour la réception de caractères.
 * 
 * @param c Caractère reçu.
 * @param UART_nb Numéro de l'UART.
 */
void CommunicationService_Callback(signed char c, int UART_nb){
	int n =c;
	if(n > 0 & n <= 100){ // entre 0 et 100
			if(functionTableDirection[UART_nb-1]){
				if(functionTableDirection[UART_nb-1][BABORD]){
					functionTableDirection[UART_nb-1][BABORD](c);
				}
			}
			
		}
		else if(c >=0xFFFFFF9C & c <=0xFFFFFFFF){ // entre 0 et 100
			if(functionTableDirection[UART_nb-1]){
				if(functionTableDirection[UART_nb-1][TRIBORD]){
					functionTableDirection[UART_nb-1][TRIBORD](256-c);
				}
			}
		}
	else if( c >= 0x0 & c <= 0x7F){ // compris entre le char(0) et le char(128)
			if(functionTable[UART_nb-1]){
		if(functionTable[UART_nb-1][(int)c]){
			functionTable[UART_nb-1][(int)c]();
		}
	}
		
		else if(functionTableBytes[UART_nb-1]){
			functionTableBytes[UART_nb-1](c);
		}
	} // caractère non ASCII
	else{
		if(c >= -100 & c <= -1){ // compris entre -100 et -1
			
			if(functionTableDirection[UART_nb-1]){
				if(functionTableDirection[UART_nb-1][BABORD]){
					functionTableDirection[UART_nb-1][BABORD](c*(-1));
				}
			}
			
		}else{ //ce qui est < à -100
				if(functionTableBytes[UART_nb-1]){
			functionTableBytes[UART_nb-1](c);
				}
		}
	}
}

/**
 * @brief Configure le service de communication pour la lecture.
 * @details Active l'interruption de réception et configure la fonction callback.
 * 
 * @param This Pointeur vers l'instance du service de communication.
 */
void CommunicationService_Read(CommunicationService *This){ 
	void (*Callback_pointeur)(signed char, int); 
	Callback_pointeur = CommunicationService_Callback;
	This->UART->ActiveIT(This->UART, COMMUNICATION_UART_READ_IT_PRIORITY, Callback_pointeur);
}

/**
 * @brief Initialise l'instance du service de communication.
 * 
 * @param This Pointeur vers l'instance du service de communication.
 */
static void CommunicationService_Init(CommunicationService *This)
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

/**
 * @brief Libère la mémoire allouée pour l'instance du service de communication.
 * 
 * @param This Pointeur vers l'instance du service de communication.
 */
void CommunicationService_New_Free(CommunicationService *This)
{
    This->UART->Free(This->UART);
    free(This);        
}

/**
 * @brief Crée une nouvelle instance du service de communication.
 * @details Alloue la mémoire et initialise l'instance du service de communication.
 * 
 * @return Pointeur vers la nouvelle instance du service de communication.
 */
CommunicationService *New_Communication()
{
    CommunicationService *This = malloc(sizeof(CommunicationService));
    if(!This) return NULL;
    CommunicationService_Init(This);
    This->Free = CommunicationService_New_Free;
    This->UART = New_UART(COMMUNICATION_UART);
    This->UART_nb = (COMMUNICATION_UART == USART1) ? 1 : (COMMUNICATION_UART == USART2) ? 2 : 3;
    return This;
}
