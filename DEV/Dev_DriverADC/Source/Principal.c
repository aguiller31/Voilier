#include "MyTimers.h"
#include "MyADC.h"
#include "MyGPIO.h"
#include "stm32f10x.h"
ADC_TypeDef * Adc = ADC1;
int event = TIM4_CC4_event; //fonctionne pour les TIMER 2 et 4 

GPIO_TypeDef * potentiometre_GPIO = GPIOA;
char potentiometre_pin = 0;

GPIO_TypeDef * led_GPIO = GPIOA;
char led_pin = 1;

void Callback(int nbre){
	float nbre_percent = (float)nbre / 4026.0;
	MyTimer_PWM_dutyCycle(TIM2,2,nbre_percent);
}
void SetTimer(unsigned short ARR, unsigned short PSC, unsigned short CC_value,int extsel){
		TIM_TypeDef * Timer_temp;
		char channel;
		volatile uint16_t *CCR;
		if(extsel != TIM3_TRGO_event){
			channel = (extsel ==TIM1_CC1_event) ? 1 :(extsel == TIM1_CC2_event | extsel == TIM2_CC2_event)? 2 : (extsel ==TIM1_CC3_event) ? 3 : 4;
			Timer_temp = (extsel <=TIM1_CC3_event)? TIM1 :(extsel ==TIM2_CC2_event)? TIM2 : TIM4;
			CCR = (extsel == TIM1_CC1_event)?&(Timer_temp->CCR1) : (extsel ==TIM1_CC2_event)?&(Timer_temp->CCR2) : (extsel ==TIM1_CC3_event)?&(Timer_temp->CCR3) : (extsel ==TIM2_CC2_event)?&(Timer_temp->CCR2) : &(Timer_temp->CCR4); 
			MyTimer_PWM_init( Timer_temp, channel, ARR, PSC);
			*CCR = CC_value; //nécessaire pour activer le capture compare
		}
	
	//TIM1->CCER |= TIM_CCER_CC1E;
}
void conf_led(){
	MyGPIO_Init(led_GPIO, led_pin,AltOut_Ppull);
	MyTimer_PWM_init(TIM2, 2, 10-1,72-1);
}
void conf_potentiometre(){
	MyGPIO_Init(potentiometre_GPIO, potentiometre_pin, In_Analog);
}
void conf_ADC(){
	void ( * Callback_pointeur ) ( int ) ; /* Pointeur de fonction */
	void Callback ( int ) ; /* Fonction */
	MyADC_Base_Init(Adc, 0,7.5,event );
	Callback_pointeur = Callback ; /* Affectation */
	MyADC_ActiveIT(ADC1,1,Callback_pointeur);
	SetTimer(16000-1,1000-1,1000/2-1,event);
}
void setup(){
	conf_potentiometre();
	conf_led();
	conf_ADC();
}
void loop(){
	
}

int main (void)
{
	setup();
	do
	{
		loop();
	}while(1);
}
