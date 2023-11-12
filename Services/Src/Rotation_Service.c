#include "Rotation_Service.h"

#include "stm32f10x.h"
#include "conf.h"
#include <stdlib.h>

#ifndef CONFIG_H
#define ROTATION_DIRECTION_GPIO 			GPIOC

#define ROTATION_DIRECTION_GPIO_PIN		10

#define ROTATION_SPEED_GPIO 					GPIOA
#define ROTATION_SPEED_GPIO_PIN				10

#define ROTATION_SPEED_TIMER 					TIM2
#define ROTATION_SPEED_TIMER_CHANNEL	2
#define ROTATION_SPEED_TIMER_ARR			60
#define ROTATION_SPEED_TIMER_PSC			60
#endif
void RotationService_SetupDirection(RotationService *This)
{ 
	MyGPIO_Init(ROTATION_DIRECTION_GPIO,ROTATION_DIRECTION_GPIO_PIN,Out_Ppull);
}
void RotationService_SetDirection(RotationService *This,int dir)
{ 
	if(dir == LEFT){
		MyGPIO_Set(ROTATION_DIRECTION_GPIO,ROTATION_DIRECTION_GPIO_PIN);
	}else{
		MyGPIO_Reset(ROTATION_DIRECTION_GPIO,ROTATION_DIRECTION_GPIO_PIN);
	}
}
void RotationService_SetupSpeed(RotationService *This)
{ 
	MyGPIO_Init(ROTATION_SPEED_GPIO, ROTATION_SPEED_GPIO_PIN,AltOut_Ppull);
	MyTimer_PWM_init(ROTATION_SPEED_TIMER, ROTATION_SPEED_TIMER_CHANNEL, ROTATION_SPEED_TIMER_ARR,ROTATION_SPEED_TIMER_PSC);
}
void RotationService_SetSpeed(RotationService *This,float speed)
{ 
	MyTimer_PWM_dutyCycle(ROTATION_SPEED_TIMER,ROTATION_SPEED_TIMER_CHANNEL,(float)speed/100.0);
}
void RotationService_Setup(RotationService *This)
{ 
	This->SetupDirection(This);
	This->SetupSpeed(This);
}
static void  RotationService_Init( RotationService *This)
{
			This->Setup = RotationService_Setup;
			This->SetupDirection = RotationService_SetupDirection;
			This->SetDirection = RotationService_SetDirection;
			This->SetupSpeed = RotationService_SetupSpeed;
			This->SetSpeed = RotationService_SetSpeed;
}
void RotationService_New_Free(RotationService *This)
{
				
        free(This);        
}
 RotationService * New_Rotation()
{
       RotationService *This = malloc(sizeof(RotationService));
       if(!This) return NULL;
       RotationService_Init(This);
			 This->Free = RotationService_New_Free;
       return This;
}
/*
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
void rotation_start(){

	rotation_sens_setup(GPIOC,10);
	rotation_vitesse_setup(TIM2,2,GPIOA,10);
	

	rotation_sens(LEFT);
	rotation_vitesse(20);
}
*/
