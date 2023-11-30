/*********************************************************************
 * @file  GPIO_Driver.h
 * @author Antoine Guillermin, Nathan Laschon, Paul Gadanho, Yorgo Challoub, Florian Cunnac
 * @brief Fichier head du driver GPIO
 *********************************************************************/
 
#ifndef MYGPIO_H
	#define MYGPIO_H
	#include "stm32f10x.h"

	#define In_Floating 0x04
	#define In_PullDown    0x08
	#define In_PullUp      0x09
	#define In_Analog      0x00
	#define Out_Ppull      0x02
	#define Out_OD         0x06
	#define AltOut_Ppull   0xA
	#define AltOut_OD      0xE

	void MyGPIO_Init(GPIO_TypeDef * GPIO, char GPIO_Pin, char GPIO_Conf);
	int MyGPIO_Read(GPIO_TypeDef * GPIO, char GPIO_Pin);
	int MyGPIO_Set(GPIO_TypeDef * GPIO, char GPIO_Pin);
	int MyGPIO_Reset(GPIO_TypeDef * GPIO, char GPIO_Pin);
	int MyGPIO_Toggle(GPIO_TypeDef * GPIO, char GPIO_Pin);

	#define MyGPIO_Active_IT RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; 

#endif
