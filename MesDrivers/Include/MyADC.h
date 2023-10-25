#ifndef MYADC_H
#define MYADC_H
#include "stm32f10x.h"

#define TIM1_CC1_event 0x00
#define TIM1_CC2_event 0x01
#define TIM1_CC3_event 0x02
#define TIM2_CC2_event 0x03
#define TIM3_TRGO_event 0x04
//100: Timer 3 TRGO event
#define TIM4_CC4_event 0x05

void MyADC_Base_Init(ADC_TypeDef * Adc,char Channel, float echantillonage,int extsel);
void MyADC_ActiveIT(ADC_TypeDef * Adc ,char Prio,void (*IT_function )(int));
#endif
