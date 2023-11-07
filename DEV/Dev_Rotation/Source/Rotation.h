#ifndef ROTATION
#define ROTATION
#include "stm32f10x.h"
#define LEFT 0
#define RIGHT 1
void rotation_sens_setup(GPIO_TypeDef * GPIO, char pin_sens);
void rotation_sens(int sens);
void rotation_vitesse_setup(TIM_TypeDef * Timer , char Channel,GPIO_TypeDef * GPIO, char pin);
void rotation_vitesse(float vitesse);
void rotation_start();
#endif
