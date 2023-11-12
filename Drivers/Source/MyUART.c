#include "MyUART.h"
#include "stm32f10x.h"
#include <stdlib.h>

typedef void (*UART_Callback_RXpointer)(signed char, int);
UART_Callback_RXpointer UART_Callbacks[3];

void UARTDriver_ClearDR(UARTDriver * This){
	This->UART->DR &= ~0xFF;
}
void UARTDriver_EnableTX(UARTDriver * This)
{
	This->UART->CR1 |= USART_CR1_TE;
}
void UARTDriver_DisableTX(UARTDriver * This)
{
	This->UART->CR1 &= ~USART_CR1_TE;
	This->ClearDR(This);
}
void UARTDriver_EnableRX(UARTDriver * This)
{
	This->UART->CR1 |= USART_CR1_RE;
}
void UARTDriver_DisableRX(UARTDriver * This)
{
	This->UART->CR1 &= ~USART_CR1_RE;
	This->ClearDR(This);
}
void UARTDriver_SetBaudRate(UARTDriver * This, int br)
{
	//Only USART1 is clocked with PCLK2 (72 MHz Max). Other USARTs are clocked with PCLK1 (36 MHz Max).
	float usart_div = (72.0*1000000.0)/(((This->UART == USART1)? 1.0 : 2.0)*16*(float)br);
	This->UART->BRR &= ~0xFFF1;
	This->UART->BRR |= ((int) usart_div << 4);
}
void UARTDriver_SetStopBits(UARTDriver * This, double sb)
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
void UARTDriver_SetParityControl(UARTDriver * This, char p)
{
	if(p == 0){
		This->UART->CR1 &= ~USART_CR1_PCE;
	}else{
		This->UART->CR1 |= USART_CR1_PCE;
	}
}
void UARTDriver_SetWordLength(UARTDriver * This, char wl)
{
	if(wl == 0){ //  1 Start bit, 8 Data bits, n Stop bit
		This->UART->CR1 &= ~USART_CR1_M;
	}else{ //  1 Start bit, 9 Data bits, n Stop bit
		This->UART->CR1 |= USART_CR1_M;
	}
}
void UARTDriver_Start(UARTDriver * This)
{
	This->UART->CR1 |= USART_CR1_UE;
}

void UARTDriver_WriteCharacter(UARTDriver * This, signed char character)
{
		This->ClearDR(This);
		This->UART->DR = character;
		while((This->UART->SR & USART_SR_TXE) != USART_SR_TXE){} //polling sur TXE
}
void  UARTDriver_ActiveIT(UARTDriver * This , char Prio ,void (*IT_function )(signed char,int)){
		//interruption RX
	IRQn_Type IRQ_pin;
	This->UART->CR1 |= USART_CR1_RXNEIE;
	if(This->UART == USART1){
		IRQ_pin = USART1_IRQn;
		UART_Callbacks[0] = IT_function;
	}
	else if(This->UART == USART2){
		IRQ_pin = USART2_IRQn;
		UART_Callbacks[1] = IT_function;
	}
	else if(This->UART == USART3){
		IRQ_pin = USART3_IRQn;
		UART_Callbacks[2] = IT_function;
	}

	NVIC_EnableIRQ(IRQ_pin);
	NVIC_SetPriority(IRQ_pin,Prio);
}
static void  UARTDriver_Init( UARTDriver *This)
{
      This->ClearDR = UARTDriver_ClearDR;    
			This->EnableTX = UARTDriver_EnableTX;   
			This->DisableTX = UARTDriver_DisableTX;   
			This->EnableRX = UARTDriver_EnableRX;  
			This->DisableRX = UARTDriver_DisableRX;
			This->SetBaudRate =UARTDriver_SetBaudRate;
			This->SetStopBits =UARTDriver_SetStopBits;
			This->SetParityControl =UARTDriver_SetParityControl;
			This->SetWordLength = UARTDriver_SetWordLength;
			This->Start =UARTDriver_Start;
			This->WriteCharacter =UARTDriver_WriteCharacter;
			This->ActiveIT =UARTDriver_ActiveIT;
}
void UARTDriver_New_Free(UARTDriver *This)
{
        free(This);        
}
 UARTDriver * New_UART(USART_TypeDef * USART)
{
       UARTDriver *This = malloc(sizeof(UARTDriver));
       if(!This) return NULL;
       UARTDriver_Init(This);
			 This->Free = UARTDriver_New_Free;
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
			(*UART_Callbacks[0]) ((signed char)USART1->DR,1);
	} 
}
void USART2_IRQHandler (void)
{
if((USART2->SR & USART_SR_RXNE) == USART_SR_RXNE){ //RX
			(*UART_Callbacks[1]) ((signed char)USART2->DR,2);
	} 

}
void USART3_IRQHandler (void)
{
	if((USART3->SR & USART_SR_RXNE) == USART_SR_RXNE){ //RX
			(*UART_Callbacks[2]) ((signed char)USART3->DR,3);
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
