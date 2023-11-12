#include "Batterie_Service.h"

#include "stm32f10x.h"
#include "conf.h"
#include <stdlib.h>

#ifndef CONFIG_H
	#define BATTERIE_ADC_IT_PRIORITY 4
	#define BATTERIE_ADC 			ADC1
	#define BATTERIE_ADC_CHANNEL	0
	#define BATTERIE_ADC_SAMPLING	7.5
	#define BATTERIE_ADC_TRIGGER TIM4_CC4_event
	#define BATTERIE_ADC_CC_VALUE 1000/2-1
	
	#define BATTERIE_TIMER_ARR			16000-1
	#define BATTERIE_TIMER_PSC			1000-1
	
	#define BATTERIE_GPIO	GPIOA
	#define BATTERIE_GPIO_PIN	0
#endif

void ( * Batterie_Callback_pointeur ) ( int ) ;

void Callback(int nbre)
{
	(*Batterie_Callback_pointeur) (nbre/92*100);
}
void BatterieService_SetupTIMER(BatterieService *This)
{
	TIM_TypeDef * Timer_temp;
	char channel;
	volatile uint16_t *CCR;
	if(BATTERIE_ADC_TRIGGER != TIM3_TRGO_event){
		channel = (BATTERIE_ADC_TRIGGER ==TIM1_CC1_event) ? 1 :(BATTERIE_ADC_TRIGGER == TIM1_CC2_event | BATTERIE_ADC_TRIGGER == TIM2_CC2_event)? 2 : (BATTERIE_ADC_TRIGGER ==TIM1_CC3_event) ? 3 : 4;
		Timer_temp = (BATTERIE_ADC_TRIGGER <=TIM1_CC3_event)? TIM1 :(BATTERIE_ADC_TRIGGER ==TIM2_CC2_event)? TIM2 : TIM4;
		CCR = (BATTERIE_ADC_TRIGGER == TIM1_CC1_event)?&(Timer_temp->CCR1) : (BATTERIE_ADC_TRIGGER ==TIM1_CC2_event)?&(Timer_temp->CCR2) : (BATTERIE_ADC_TRIGGER ==TIM1_CC3_event)?&(Timer_temp->CCR3) : (BATTERIE_ADC_TRIGGER ==TIM2_CC2_event)?&(Timer_temp->CCR2) : &(Timer_temp->CCR4); 
		MyTimer_PWM_init( Timer_temp, channel, BATTERIE_TIMER_ARR, BATTERIE_TIMER_PSC);
		*CCR = BATTERIE_ADC_CC_VALUE; //nécessaire pour activer le capture compare
	}
}
void BatterieService_SetupADC(BatterieService *This)
{ 
	void ( * Callback_pointeur ) ( int ) ; /* Pointeur de fonction */
	MyADC_Base_Init(BATTERIE_ADC, BATTERIE_ADC_CHANNEL,BATTERIE_ADC_SAMPLING,BATTERIE_ADC_TRIGGER );
	Callback_pointeur = Callback ; /* Affectation */
	MyADC_ActiveIT(BATTERIE_ADC,BATTERIE_ADC_IT_PRIORITY,Callback_pointeur);
}
void BatterieService_Setup(BatterieService *This)
{ 
	This->SetupADC(This);
	This->SetupTIMER(This);
}
void BatterieService_RegisterBatteryLevel(BatterieService *This, void (*function )(int)) {
    Batterie_Callback_pointeur = function;
}
static void  BatterieService_Init( BatterieService *This)
{
			This->Setup = BatterieService_Setup;
			This->SetupADC = BatterieService_SetupADC;
			This->SetupTIMER = BatterieService_SetupTIMER;
			This->RegisterBatteryLevel = BatterieService_RegisterBatteryLevel;

}
void BatterieService_New_Free(BatterieService *This)
{
				
        free(This);        
}
 BatterieService * New_Batterie()
{
       BatterieService *This = malloc(sizeof(BatterieService));
       if(!This) return NULL;
       BatterieService_Init(This);
			 This->Free = BatterieService_New_Free;
       return This;
}
