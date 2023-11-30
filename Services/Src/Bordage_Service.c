#include "GPIO_Driver.h"
#include "TIMER_Driver.h"
#include "Bordage_Service.h"
#include "stm32f10x.h"
#include "conf.h"

float teta=0;
float duty_cycle=0;

void bordage(int angle_girouette){
	if(BORDAGE_MODE == 1){
		if(angle_girouette <45 || angle_girouette > 315){
				MyTimer_PWM_dutyCycle(BORDAGE_PWM_TIMER , BORDAGE_PWM_TIMER_CHANNEL, BORDAGE_PWM_TIMER_DUTYCYCLE_MODE1); //Régler pour augmenter au max
			} else {
				if(angle_girouette < 180){
					teta = angle_girouette*(0.667) -30;
				} else {
					teta = 180 - (angle_girouette*(0.667) - 30); 
				}
				duty_cycle = 0.05 +(teta * (0.05/90));
				MyTimer_PWM_dutyCycle(BORDAGE_PWM_TIMER , 2, duty_cycle); //Channel différent
			}
		} else {
			if(angle_girouette <45 || angle_girouette > 315){
				MyTimer_PWM_dutyCycle(BORDAGE_PWM_TIMER , BORDAGE_PWM_TIMER_CHANNEL, BORDAGE_PWM_TIMER_DUTYCYCLE_MODE2);
			} else {
				if(angle_girouette < 180){
					teta = angle_girouette*(0.667) -30;
				} else {
					teta = 180 - (angle_girouette*(0.667) - 30); 
				}
				duty_cycle = 0.1 -(teta * (0.05/90));
				MyTimer_PWM_dutyCycle(BORDAGE_PWM_TIMER , 2, duty_cycle);
			}
		}
}
		

void InitBordage(void){
	MyGPIO_Init(BORDAGE_PWM_TIMER_GPIO, BORDAGE_PWM_TIMER_GPIO_PIN, AltOut_Ppull );
	MyTimer_PWM_init( BORDAGE_PWM_TIMER, BORDAGE_PWM_TIMER_CHANNEL, BORDAGE_PWM_TIMER_ARR, BORDAGE_PWM_TIMER_PSC );
	MyTimer_PWM_dutyCycle(BORDAGE_PWM_TIMER , BORDAGE_PWM_TIMER_CHANNEL, 0.080);
}
