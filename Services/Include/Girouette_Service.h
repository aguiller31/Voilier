/*********************************************************************
 * @file  Girouette_Service.h
 * @author Nathan Laschon, Florian Cunnac
 * @brief Fichier head du service Girouette 
 *********************************************************************/
 
 #ifndef GIROUETTE_SERVICE_H
	#define GIROUETTE_SERVICE_H
	#include "stm32f10x.h"
	#include "GPIO_Driver.h"
	#include "TIMER_Driver.h"
	#include "conf.h"

	extern int angle_girouette;

	void Remise_zero(void);
	void Increment(void);

	void InitGirouette(void);
	void InitGirouette2(TIM_TypeDef * Timer);

	int getAngleGirouette(void);
	int getAngleGirouette2(void);


	#define TIMER_BASE_START(Timer) ((Timer)->CR1|=TIM_CR1_CEN)

#endif

