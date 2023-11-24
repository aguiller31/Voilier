#include "GPIO_Driver.h"
#include "TIMER_Driver.h"
#include "Girouette_Service.h"
#include "Bordage_Service.h"
#include "stm32f10x.h"
#include "conf.h"

float teta=0;
float duty_cycle=0;

void bordage(void){
	if(angle_girouette <45 || angle_girouette > 315){
			MyTimer_PWM_dutyCycle(BORDAGE_PWM_TIMER , BORDAGE_PWM_TIMER_CHANNEL, BORDAGE_PWM_TIMER_DUTYCYCLE); //Régler pour augmenter au max
		} else {
			if(angle_girouette < 180){
				teta = angle_girouette*(0.667) -30;
			} else {
				teta = 180 - (angle_girouette*(0.667) - 30); 
			}
			duty_cycle = 0.05 +(teta * (0.05/90));
			MyTimer_PWM_dutyCycle(TIM2 , 2, duty_cycle);
		}
}

void InitBordage(void){
	MyGPIO_Init(GPIOA, 1, AltOut_Ppull );


	
	
	MyTimer_PWM_init( TIM2, 2, 39999, 35 );
	MyTimer_PWM_dutyCycle(TIM2 , 2, 0.080);
}
