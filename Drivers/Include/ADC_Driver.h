#ifndef MYADC_H
#define MYADC_H
#include "stm32f10x.h"

#define TIM1_CC1_EVENT 0x00
#define TIM1_CC2_EVENT 0x01
#define TIM1_CC3_EVENT 0x02
#define TIM2_CC2_EVENT 0x03
#define TIM3_TRGO_EVENT 0x04
//100: Timer 3 TRGO event
#define TIM4_CC4_EVENT 0x05
#define SW_EVENT 0x07

void MyADC_Base_Init(ADC_TypeDef * Adc,char Channel, float echantillonage,int extsel);
void MyADC_ActiveIT(ADC_TypeDef * Adc ,char Prio,void (*IT_function )(int));
void MyADC_SwStart(ADC_TypeDef * Adc);

#endif
