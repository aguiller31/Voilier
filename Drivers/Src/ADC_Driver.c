#include "ADC_Driver.h"
void ( * ADC1_Callback_pointeur ) ( int ) ;
void ( * ADC2_Callback_pointeur ) ( int ) ;
int ADC1_It = 0;
int ADC2_It = 0;
void MyADC_Base_Init(ADC_TypeDef * Adc, char Channel , float echantillonage,int extsel){
int echantillonnage_hex;
	if(Adc == ADC1){
		RCC->APB2ENR |=  RCC_APB2ENR_ADC1EN;  //activation de la clock du rcc //FAIRE UN IF EN FONCTION DU ADC
	}else{
		RCC->APB2ENR |=  RCC_APB2ENR_ADC2EN;
	}
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6; //on met un prescaler de 4 sur la clock precedente
	echantillonnage_hex = (echantillonage  == 1.5)? 0x0 : (echantillonage  == 7.5)? 0x1 : (echantillonage  == 13.5)? 0x2 :  (echantillonage  == 28.5)? 0x3 :  (echantillonage  == 41.5)? 0x4 :  (echantillonage  == 55.5)? 0x5 :  (echantillonage  == 71.5)? 0x6 : 0x7; 
	if(Channel <= 10){
		Adc->SMPR2 |=(echantillonnage_hex << 3*Channel);
	}else{
		Adc->SMPR1 |=(echantillonnage_hex << 3*Channel);
	}
	
	//paramètres du séquenceur
	Adc->SQR3 &= ~ADC_SQR1_L; //on ne veut pas utiliser le séquenceur donc on met 1 conversion
	Adc->SQR3 |= Channel;
	Adc->CR2 &= ~ADC_CR2_CONT;
	Adc->CR1 &= ~ADC_CR1_SCAN;
	Adc->CR1 &= ~ADC_CR1_DISCNUM;
	

	//on configure le channel
	Adc->SQR3 &= ~ADC_SQR3_SQ1;
	Adc->SQR3 = Channel;

	Adc->CR2 |= ADC_CR2_EXTTRIG;
		Adc->CR2 &= ~ADC_CR2_EXTSEL; // je ne sais pas pourquoi il faut l'activer ici pour que cela fonctionne avec TIM1
																	// le problème c'est qu'il semble qu'il faut faire ca à chaque callback
		Adc->CR2 |= (extsel<<17);
	Adc->CR2 |= ADC_CR2_ADON;
			
}
void MyADC_ActiveIT(ADC_TypeDef * Adc  ,char Prio,void (*IT_function )(int)){
	IRQn_Type IRQ_pin;
	//uint16_t DIER_ue;
	Adc->CR1 |= ADC_CR1_EOCIE;
	
	if(Adc == ADC1){
		ADC1_Callback_pointeur = IT_function;
		IRQ_pin = ADC1_2_IRQn;
		ADC1_It = 1;
	}
	else {
		ADC2_Callback_pointeur = IT_function;
		IRQ_pin = ADC1_2_IRQn;
		ADC2_It = 1;
	}
	NVIC_EnableIRQ(IRQ_pin);
	NVIC_SetPriority(IRQ_pin,Prio);
}

void ADC1_2_IRQHandler (void)
{ //visiblement il n'y a pas besoin de lire le flag eoc ni de le remettre à 0 avec (ADC2->SR & ADC_SR_EOC) == 1 dans la condition et ADC1->SR &= ~(1<<1); dans le if
	if(ADC1_It==1){ //on vérifie que c'est l'évenement overflow
		(*ADC1_Callback_pointeur) (ADC1->DR);
	}
	if( ADC2_It==1){ //on vérifie que c'est l'évenement overflow
		(*ADC2_Callback_pointeur) (ADC2->DR);
	}
/* comple te r avec l e code de l a r o u ti n e du t r ai t em e n t de l ’ i n t e r r u p t i o n */
}
