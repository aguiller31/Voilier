#include "MyTimers.h"
#include "MyGPIO.h"
#include "stm32f10x.h"
	int i = 0;
	TIM_TypeDef * Timer = TIM1;
void test_stop(){
	if(MyGPIO_Read(GPIOC,8) != 0){
		MyTimer_Base_Stop(Timer);
		i=0;
		do{
			i++;
		}while(i < 10e5);
	}
}
void Callback(void){
	MyGPIO_Toggle(GPIOC,10);
}
int main (void)
{
	void ( * Callback_pointeur ) ( void ) ; /* Pointeur de fonction */
	void Callback ( void ) ; /* Fonction */
	MyGPIO_Init(GPIOC, 8,In_Floating); //pour tester le stops
	MyGPIO_Init(GPIOC, 10,Out_OD);
	
	MyTimer_Base_Init( Timer, 1000-1, 36000 -1);
	MyTimer_Base_Start(Timer);
	Callback_pointeur = Callback ; /* Affectation */
	MyTimer_ActiveIT(Timer,0,Callback_pointeur);
	
	MyGPIO_Init(GPIOA, 0,AltOut_Ppull);
	MyTimer_PWM_init(TIM2, 1, 10-1,72-1);
	MyTimer_PWM_dutyCycle(TIM2,1,0.80);
	do
{
	//test_stop();
}while(1);
}
