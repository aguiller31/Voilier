#include "TIMER_Driver.h"
#include "stm32f10x.h"
void ( * TIM1_Callback_pointeur ) ( void ) ;
void ( * TIM2_Callback_pointeur ) ( void ) ;
void ( * TIM3_Callback_pointeur ) ( void ) ;
void ( * TIM4_Callback_pointeur ) ( void ) ;

void MyTimer_Base_Init( TIM_TypeDef * Timer, unsigned short ARR, unsigned short PSC ){
	if(Timer==TIM1){
		RCC->APB2ENR |=  RCC_APB2ENR_TIM1EN;
	}else if(Timer==TIM2){
		RCC->APB1ENR |=  RCC_APB1ENR_TIM2EN;
	}else if(Timer==TIM3){
		RCC->APB1ENR |=  RCC_APB1ENR_TIM3EN;
	}else if(Timer==TIM4){
		RCC->APB1ENR |=  RCC_APB1ENR_TIM4EN;
	}
	Timer->ARR = ARR;
	Timer->PSC = PSC;
}

void MyTimer_ActiveIT(TIM_TypeDef * Timer , char Prio, void (*IT_function )(void)){
	IRQn_Type IRQ_pin;
	//uint16_t DIER_ue;
	Timer->DIER |= TIM_DIER_UIE;

	if(Timer == TIM1){
		IRQ_pin = TIM1_UP_IRQn;
		TIM1_Callback_pointeur = IT_function;
	}else if(Timer == TIM2){
		IRQ_pin = TIM2_IRQn;
		TIM2_Callback_pointeur = IT_function;
	}else if(Timer == TIM3){
		IRQ_pin = TIM3_IRQn;
		TIM3_Callback_pointeur = IT_function;
	}
	else if(Timer == TIM4){
		IRQ_pin = TIM4_IRQn;
		TIM4_Callback_pointeur = IT_function;
	}
	NVIC_EnableIRQ(IRQ_pin);
	NVIC_SetPriority(IRQ_pin,Prio);
}

void TIM1_UP_IRQHandler (void)
{
	//if((TIM1->SR &(0x01<< 0)) == 1){ //on vérifie que c'est l'évenement overflow
	if((TIM1->SR & TIM_SR_UIF) == TIM_SR_UIF){
		TIM1->SR &= ~(1<<0);
		if (TIM1_Callback_pointeur != 0)
			(*TIM1_Callback_pointeur) ();
	}
/* comple te r avec l e code de l a r o u ti n e du t r ai t em e n t de l ’ i n t e r r u p t i o n */
}
void TIM2_IRQHandler (void)
{
	if((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF){ //on vérifie que c'est l'évenement overflow
		TIM2->SR &= ~(1<<0);
		(*TIM2_Callback_pointeur) ();
	}
/* comple te r avec l e code de l a r o u ti n e du t r ai t em e n t de l ’ i n t e r r u p t i o n */
}
void TIM3_IRQHandler (void)
{
	if((TIM3->SR & TIM_SR_UIF) == TIM_SR_UIF){ //on vérifie que c'est l'évenement overflow
		TIM3->SR &= ~(1<<0);
		(*TIM3_Callback_pointeur) ();
	}
/* comple te r avec l e code de l a r o u ti n e du t r ai t em e n t de l ’ i n t e r r u p t i o n */
}
void TIM4_IRQHandler (void)
{
	if((TIM4->SR & TIM_SR_UIF) == TIM_SR_UIF){ //on vérifie que c'est l'évenement overflow
		TIM4->SR &= ~(1<<0);
		(*TIM4_Callback_pointeur) ();
	}
/* comple te r avec l e code de l a r o u ti n e du t r ai t em e n t de l ’ i n t e r r u p t i o n */
}

void MyTimer_PWM( TIM_TypeDef * Timer , char Channel ){ //il y a 4 channels par Timer lire p28/127 de la datasheet 
	switch(Channel){
		case 1:
			Timer->CCER = TIM_CCER_CC1E;
			Timer->CR2 = TIM_BDTR_MOE;
		Timer->CCMR1 &= ~(0x03);
			Timer->CCMR1 &= ~(0x07<<4);
			Timer->CCMR1 |= (0x06<<4);
		//Timer->CCMR1|= (0x06<<8);
		break;
		case 2:
			
			Timer->CCER = TIM_CCER_CC2E;
					Timer->CCMR1 &= ~(0x03 << 8);
					Timer->CCMR1 &= ~(0x07 << 12);
			Timer->CCMR1|= (0x06<<12);
		break;
		case 3:
			Timer->CCER = TIM_CCER_CC3E;
		Timer->CCMR2 &= ~(0x03);
		Timer->CCMR2 &= ~(0x07<<4);
			Timer->CCMR2|= (0x06<<4);
		break;
		case 4:
			Timer->CCER = TIM_CCER_CC4E;
							Timer->CCMR2 &= ~(0x03 << 8);
					Timer->CCMR2 &= ~(0x07 << 12);
			Timer->CCMR2 |= (0x06<<12);
		break;
	}
}
	void MyTimer_PWM_init( TIM_TypeDef * Timer , char Channel ,unsigned short ARR, unsigned short PSC ){ //il y a 4 channels par Timer lire p28/127 de la datasheet 
				MyTimer_Base_Init( Timer, ARR, PSC);
		MyTimer_PWM(Timer,Channel);
		MyTimer_Base_Start(Timer);
}
	void MyTimer_PWM_dutyCycle( TIM_TypeDef * Timer , char Channel, float alpha){ 
			int new_alpha = alpha*Timer->ARR;
		switch(Channel){
	
		case 1:
			
			Timer->CCR1 = new_alpha;
		break;
		case 2:
			Timer->CCR2 = new_alpha;
		break;
		case 3:
			
			Timer->CCR3 = new_alpha;
		break;
		case 4:
			Timer->CCR4 = new_alpha;
		break;
	}
	}
	
	void MyTimer_Config_line_9 (void) { 
		//DEfnie comme non masqué le pin 9
	EXTI->IMR |=	EXTI_IMR_MR9;
	EXTI->RTSR |=	EXTI_RTSR_TR9;
	//Definie le mux pour pc9
	AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI9_PC;
}

void MyTimer_Config_line_10 (void){
	//DEfnie comme non masqué le pin 10
	EXTI->IMR |=	EXTI_IMR_MR10;
	EXTI->RTSR |=	EXTI_RTSR_TR10;
	//Definie le mux pour pc10
	AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PC;
}

void MyTimer_ActiveIT_EXTI (char Prio, int Pin, void (*IT_function)(void)){
		if (4 < Pin && Pin < 10){
			NVIC_SetPriority(EXTI9_5_IRQn, Prio);
			NVIC_EnableIRQ(EXTI9_5_IRQn);
			//ptrIT_EXTI9_5 = IT_function;
		}else if (9 < Pin && Pin < 16){
			NVIC_SetPriority(EXTI15_10_IRQn, Prio);
			NVIC_EnableIRQ(EXTI15_10_IRQn);
		//ptrIT_EXTI15_10 = IT_function;
		}
}

void EXTI9_5_IRQHandler (void){
	//Remise a 0 de l'interruption a faire
	EXTI->PR |= EXTI_PR_PR9;
	//(*ptrIT_EXTI9_5) ();
}

void EXTI15_10_IRQHandler (void){
	//Remise a 0 de l'interruption a faire
	EXTI->PR |= EXTI_PR_PR10;
	//(*ptrIT_EXTI15_10) ();
}
