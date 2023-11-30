/*********************************************************************
 * @file  ADC_Driver.c
 * @author Antoine Guillermin, Nathan Laschon, Paul Gadanho, Yorgo Challoub, Florian Cunnac
 * @brief Fichier source du driver ADC
 *********************************************************************/
 
 
 #include "ADC_Driver.h"
void ( * ADC1_Callback_pointeur ) ( int ) ;
void ( * ADC2_Callback_pointeur ) ( int ) ;
int ADC1_It = 0;
int ADC2_It = 0;

void MyADC_Base_Init(ADC_TypeDef * Adc, char Channel , float echantillonage,int extsel)
{
	int echantillonnage_hex;
	if(Adc == ADC1){
		RCC->APB2ENR |=  RCC_APB2ENR_ADC1EN;  //activation de la clock du rcc
	}else{
		RCC->APB2ENR |=  RCC_APB2ENR_ADC2EN;
	}
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6; //on met un prescaler sur la clock precedente
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
	Adc->CR2 &= ~ADC_CR2_EXTSEL; 
	Adc->CR2 |= (extsel<<17);
	Adc->CR2 |= ADC_CR2_ADON;
	
			
}
void MyADC_SwStart(ADC_TypeDef * Adc)
{
	Adc->CR2 |= ADC_CR2_SWSTART;
}
void MyADC_ActiveIT(ADC_TypeDef * Adc  ,char Prio,void (*IT_function )(int))
{
	IRQn_Type IRQ_pin;

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
{ 
	if(ADC1_It==1){
		(*ADC1_Callback_pointeur) (ADC1->DR);
	}
	if( ADC2_It==1){
		(*ADC2_Callback_pointeur) (ADC2->DR);
	}
}

	
