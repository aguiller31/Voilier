#ifndef ROTATION_SERVICE
#define ROTATION_SERVICE
#include "stm32f10x.h"
#include "MyGPIO.h"
#include "MyTimers.h"
#define LEFT 0
#define RIGHT 1

typedef struct RotationService
{
	void(*Free)(struct RotationService*);
	void (*Setup)(struct RotationService*);
	void (*SetupDirection)(struct RotationService*);
	void (*SetDirection)(struct RotationService*, int);
	void (*SetupSpeed)(struct RotationService*);
	void (*SetSpeed)(struct RotationService*, float);

} RotationService ;
 RotationService * New_Rotation(void);

/*
void rotation_sens_setup(GPIO_TypeDef * GPIO, char pin_sens);
void rotation_sens(int sens);
void rotation_vitesse_setup(TIM_TypeDef * Timer , char Channel,GPIO_TypeDef * GPIO, char pin);
void rotation_vitesse(float vitesse);
void rotation_start();
*/
#endif
