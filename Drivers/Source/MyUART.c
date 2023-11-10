#include "MyUART.h"
#include "stm32f10x.h"
#include <stdlib.h>
void ( * USART1_Callback_RXpointeur ) ( char,int ) ;
void ( * USART2_Callback_RXpointeur ) ( char,int ) ;
void ( * USART3_Callback_RXpointeur ) ( char,int ) ;

void UARTClass_ClearDR(UARTClass * This){
	This->UART->DR &= ~0xFF;
}
void UARTClass_EnableTX(UARTClass * This)
{
	This->UART->CR1 |= USART_CR1_TE;
}
void UARTClass_DisableTX(UARTClass * This)
{
	This->UART->CR1 &= ~USART_CR1_TE;
	This->ClearDR(This);
}
void UARTClass_EnableRX(UARTClass * This)
{
	This->UART->CR1 |= USART_CR1_RE;
}
void UARTClass_DisableRX(UARTClass * This)
{
	This->UART->CR1 &= ~USART_CR1_RE;
	This->ClearDR(This);
}
void UARTClass_SetBaudRate(UARTClass * This, int br)
{
	float usart_div = (72.0*1000000.0)/(((This->UART == USART1)? 1.0 : 2.0)*16*(float)br);
	This->UART->BRR &= ~0xFFF1;
	This->UART->BRR |= ((int) usart_div << 4);
}
void UARTClass_SetStopBits(UARTClass * This, double sb)
{
	This->UART->CR2 &= ~USART_CR2_STOP;
	if(sb == 0.5){
		This->UART->CR2 |= USART_CR2_STOP_0;
	}else if(sb == 2.0){
		This->UART->CR2 |= USART_CR2_STOP_1;
	}else if(sb == 1.5){
		This->UART->CR2 |= USART_CR2_STOP;
	}
}
void UARTClass_SetParityControl(UARTClass * This, char p)
{
	if(p == 0){
		This->UART->CR1 &= ~USART_CR1_PCE;
	}else{
		This->UART->CR1 |= USART_CR1_PCE;
	}
}
void UARTClass_SetWordLength(UARTClass * This, char wl)
{
	if(wl == 0){ //  1 Start bit, 8 Data bits, n Stop bit
		This->UART->CR1 &= ~USART_CR1_M;
	}else{ //  1 Start bit, 9 Data bits, n Stop bit
		This->UART->CR1 |= USART_CR1_M;
	}
}
void UARTClass_Start(UARTClass * This)
{
	This->UART->CR1 |= USART_CR1_UE;
}

void UARTClass_WriteCharacter(UARTClass * This, char character)
{
		This->ClearDR(This);
		This->UART->DR = character;
		while((This->UART->SR & USART_SR_TXE) != USART_SR_TXE){} //polling sur TXE
}
void  UARTClass_ActiveIT(UARTClass * This , char Prio ,void (*IT_function )(char,int)){
		//interruption RX
	IRQn_Type IRQ_pin;
	This->UART->CR1 |= USART_CR1_RXNEIE;
	if(This->UART == USART1){
		IRQ_pin = USART1_IRQn;
		USART1_Callback_RXpointeur = IT_function;
	}else if(This->UART == USART2){
		IRQ_pin = USART2_IRQn;
	USART2_Callback_RXpointeur = IT_function;
	}else if(This->UART == USART3){
		IRQ_pin = USART3_IRQn;
		USART3_Callback_RXpointeur = IT_function;
	}

	NVIC_EnableIRQ(IRQ_pin);
	NVIC_SetPriority(IRQ_pin,Prio);
}
static void  UARTClass_Init( UARTClass *This)
{
      This->ClearDR = UARTClass_ClearDR;    
			This->EnableTX = UARTClass_EnableTX;   
			This->DisableTX = UARTClass_DisableTX;   
			This->EnableRX = UARTClass_EnableRX;  
			This->DisableRX = UARTClass_DisableRX;
			This->SetBaudRate =UARTClass_SetBaudRate;
			This->SetStopBits =UARTClass_SetStopBits;
			This->SetParityControl =UARTClass_SetParityControl;
			This->SetWordLength = UARTClass_SetWordLength;
			This->Start =UARTClass_Start;
			This->WriteCharacter =UARTClass_WriteCharacter;
			This->ActiveIT =UARTClass_ActiveIT;
	
		
}
void UARTClass_New_Free(UARTClass *This)
{
        free(This);        
}
 UARTClass * New_UART(USART_TypeDef * USART)
{
       UARTClass *This = malloc(sizeof(UARTClass));
       if(!This) return NULL;
       UARTClass_Init(This);
			 This->Free = UARTClass_New_Free;
			 This->UART = USART;
				if(USART == USART1){
				RCC->APB2ENR |=RCC_APB2ENR_USART1EN;
			}else if(USART == USART2){
				RCC->APB1ENR |=RCC_APB1ENR_USART2EN;
			}else if(USART == USART3){
				RCC->APB1ENR |=RCC_APB1ENR_USART3EN;
			}
			
       return This;
}
void USART1_IRQHandler (void)
{
	if((USART1->SR & USART_SR_RXNE) == USART_SR_RXNE){ //RX
			(*USART1_Callback_RXpointeur) (USART1->DR,1);
	} 
}
void USART2_IRQHandler (void)
{
if((USART2->SR & USART_SR_RXNE) == USART_SR_RXNE){ //RX
			(*USART2_Callback_RXpointeur) (USART2->DR,2);
	} 

}
void USART3_IRQHandler (void)
{
	if((USART3->SR & USART_SR_RXNE) == USART_SR_RXNE){ //RX
			(*USART3_Callback_RXpointeur) (USART1->DR,3);
	} 
}
/*
void ( * USART1_Callback_RXpointeur ) ( char ) ;
void ( * USART2_Callback_RXpointeur ) ( char ) ;
void ( * USART3_Callback_RXpointeur ) ( char ) ;

void MyUSART_clearDR(USART_TypeDef * USART)
{
	USART->DR &= ~0xFF;
}
void  MyUSART_enableTX(USART_TypeDef * USART)
{
	USART->CR1 |= USART_CR1_TE;
}
void  MyUSART_disableTX(USART_TypeDef * USART)
{
	USART->CR1 &= ~USART_CR1_TE;
	MyUSART_clearDR(USART);
}
void  MyUSART_enableRX(USART_TypeDef * USART)
{
	USART->CR1 |= USART_CR1_RE;
}

void MyUSART_Init(USART_TypeDef * USART, int baud_rate, double stop_bits, char parity)
{
	//double baud_rate = message_size*8/(message_period/1000);

	
	int divider = (USART == USART1)? 1.0 : 2.0;
	float usart_div = (72.0*1000000.0)/(divider*16*(float)baud_rate);
	
	if(USART == USART1){
		RCC->APB2ENR |=RCC_APB2ENR_USART1EN; //permet d'activier le regirstre APB2 de la clock périphérique "When the peripheral clock is not active, the peripheral register values may not be readable
//by software and the returned value is always 0x0.e"
	}else if(USART == USART2){
		RCC->APB1ENR |=RCC_APB1ENR_USART2EN; //permet d'activier le regirstre APB2 de la clock périphérique "When the peripheral clock is not active, the peripheral register values may not be readable
//by software and the returned value is always 0x0.e"
	}else if(USART == USART3){
		RCC->APB1ENR |=RCC_APB1ENR_USART3EN; //permet d'activier le regirstre APB2 de la clock périphérique "When the peripheral clock is not active, the peripheral register values may not be readable
//by software and the returned value is always 0x0.e"
	}
	USART->BRR &= ~0xFFF1;
	USART->BRR |= ((int) usart_div << 4);

	USART->CR1 &= ~USART_CR1_M;
	if(parity == 0){
		USART->CR1 &= ~USART_CR1_PCE;
	}else{
		USART->CR1 |= USART_CR1_PCE;
	}
	//cf p.804 du RM0008
		//MyUSART_enableTX(USART);
		MyUSART_enableRX(USART);
	MyUSART_enableTX(USART);
	USART->CR2 &= ~(USART_CR2_STOP_0 | USART_CR2_STOP_1);
		USART->CR1 |= USART_CR1_UE;

}


void MyUSART_WriteCharacter(USART_TypeDef * USART, char character)
{

	
	//MyUSART_enableTX(USART);
		MyUSART_clearDR(USART);
	USART->DR = character;
	while((USART->SR & USART_SR_TXE) != USART_SR_TXE){} //polling sur TXE
	//MyUSART_disableTX(USART);
}
void  MyUSART_ActiveRXIT(USART_TypeDef * USART , char Prio ,void (*IT_function )(char)){
		//interruption RX
	IRQn_Type IRQ_pin;
	USART->CR1 |= USART_CR1_RXNEIE;
	if(USART == USART1){
		IRQ_pin = USART1_IRQn;
		USART1_Callback_RXpointeur = IT_function;
	}else if(USART == USART2){
		IRQ_pin = USART2_IRQn;
	USART2_Callback_RXpointeur = IT_function;
	}else if(USART == USART3){
		IRQ_pin = USART3_IRQn;
		USART3_Callback_RXpointeur = IT_function;
	}

	NVIC_EnableIRQ(IRQ_pin);
	NVIC_SetPriority(IRQ_pin,Prio);
}
void USART1_IRQHandler (void)
{

	if((USART1->SR & USART_SR_RXNE) == USART_SR_RXNE){ //RX
			(*USART1_Callback_RXpointeur) (USART1->DR);
	} 
}
void USART2_IRQHandler (void)
{
if((USART2->SR & USART_SR_RXNE) == USART_SR_RXNE){ //RX
			(*USART2_Callback_RXpointeur) (USART2->DR);
	} 

}
void USART3_IRQHandler (void)
{
	if((USART3->SR & USART_SR_RXNE) == USART_SR_RXNE){ //RX
			(*USART3_Callback_RXpointeur) (USART1->DR);
	} 
}
*/
