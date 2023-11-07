#include "Rotation.h"
#include "MyGPIO.h"
#include "MyTimers.h"
#include "stm32f10x.h"



GPIO_TypeDef * GPIO_rot_sens;
char pin_rot_sens;

TIM_TypeDef * Timer_rot_vitesse;
char channel_rot_vitesse;

void rotation_sens_setup(GPIO_TypeDef * GPIO, char pin_sens){
	GPIO_rot_sens = GPIO;
	pin_rot_sens = pin_sens;
	MyGPIO_Init(GPIO,pin_sens,Out_Ppull);
}
void rotation_sens(int sens){
	if(sens == LEFT){
		MyGPIO_Set(GPIO_rot_sens,pin_rot_sens);
	}else{
		MyGPIO_Reset(GPIO_rot_sens,pin_rot_sens);
	}
}
void rotation_vitesse_setup(TIM_TypeDef * Timer , char Channel,GPIO_TypeDef * GPIO, char pin){
	GPIO_rot_sens=GPIO;
	pin_rot_sens=pin;
	MyGPIO_Init(GPIO, pin,AltOut_Ppull);
	MyTimer_PWM_init(Timer, Channel, 61-1,61-1);
}
void rotation_vitesse(float vitesse){
	MyTimer_PWM_dutyCycle(Timer_rot_vitesse,channel_rot_vitesse,(float)vitesse/100.0);
}

