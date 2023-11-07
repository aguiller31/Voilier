#include "MyUART.h"
#include "stm32f10x.h"
void ( * USART1_Callback_pointeur ) ( void ) ;
void ( * USART2_Callback_pointeur ) ( void ) ;
void ( * USART3_Callback_pointeur ) ( void ) ;
char message_USART1[8];
char message_USART2[8];
char message_USART3[8];
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
	USART->CR1 |= USART_CR1_UE;
	USART->CR1 &= ~USART_CR1_M;
	if(parity == 0){
		USART->CR1 &= ~USART_CR1_PCE;
	}else{
		USART->CR1 |= USART_CR1_PCE;
	}
	//cf p.804 du RM0008
		USART->CR1 |= USART_CR1_RE;
	USART->CR1 |= USART_CR1_TE;
	USART->CR2 &= ~(USART_CR2_STOP_0 | USART_CR2_STOP_1);
	

}
void MyUSART_WriteCharacter(USART_TypeDef * USART, char character)
{
	USART->DR &= ~0xFF;
	USART->DR |= character;
}

void  MyUSART_ActiveIT(USART_TypeDef * USART , char Prio ,void (*IT_function )(void)){
		//interruption RX
	IRQn_Type IRQ_pin;
	USART->CR1 |= USART_CR1_RXNEIE;
	if(USART == USART1){
		IRQ_pin = USART1_IRQn;
		USART1_Callback_pointeur = IT_function;
	}else if(USART == USART2){
		IRQ_pin = USART2_IRQn;
	USART2_Callback_pointeur = IT_function;
	}else if(USART == USART3){
		IRQ_pin = USART3_IRQn;
		USART3_Callback_pointeur = IT_function;
	}

	NVIC_EnableIRQ(IRQ_pin);
	NVIC_SetPriority(IRQ_pin,Prio);
}
void USART1_IRQHandler (void)
{
	if((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF){ //on vérifie que c'est l'évenement overflow
		TIM2->SR &= ~(1<<0);
		(*USART1_Callback_pointeur) ();
	}
/* comple te r avec l e code de l a r o u ti n e du t r ai t em e n t de l ’ i n t e r r u p t i o n */
}

void USART2_IRQHandler (void)
{
	if((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF){ //on vérifie que c'est l'évenement overflow
		TIM2->SR &= ~(1<<0);
		(*USART1_Callback_pointeur) ();
	}
/* comple te r avec l e code de l a r o u ti n e du t r ai t em e n t de l ’ i n t e r r u p t i o n */
}
void USART3_IRQHandler (void)
{
	if((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF){ //on vérifie que c'est l'évenement overflow
		TIM2->SR &= ~(1<<0);
		(*USART1_Callback_pointeur) ();
	}
/* comple te r avec l e code de l a r o u ti n e du t r ai t em e n t de l ’ i n t e r r u p t i o n */
}
int get_mantissa(double baud_rate){
	return (int)baud_rate;
}