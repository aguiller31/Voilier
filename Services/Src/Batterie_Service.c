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

void BatterieService_Callback(int nbre)
{
	(*Batterie_Callback_pointeur) (nbre/92*100);
}

void BatterieService_Setup(BatterieService *This)
{ 
	void ( * BatterieService_Callback_pointeur ) ( int ) ; /* Pointeur de fonction */
	MyADC_Base_Init(BATTERIE_ADC, BATTERIE_ADC_CHANNEL,BATTERIE_ADC_SAMPLING,SW_EVENT );
	BatterieService_Callback_pointeur = BatterieService_Callback ; /* Affectation */
	MyADC_ActiveIT(BATTERIE_ADC,BATTERIE_ADC_IT_PRIORITY,BatterieService_Callback_pointeur);
}
void BatterieService_RegisterBatteryLevel(BatterieService *This, void (*function )(int)) {
    Batterie_Callback_pointeur = function;
}
void BatterieService_GetBatteryLevel(BatterieService *This) {
		MyADC_SwStart(BATTERIE_ADC);
}
static void  BatterieService_Init( BatterieService *This)
{
			This->Setup = BatterieService_Setup;
			This->RegisterBatteryLevel = BatterieService_RegisterBatteryLevel;
			This->GetBatteryLevel = BatterieService_GetBatteryLevel;
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
