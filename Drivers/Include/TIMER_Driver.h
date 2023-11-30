/*********************************************************************
 * @file  TIMER_Driver.h
 * @author Antoine Guillermin, Nathan Laschon, Paul Gadanho, Yorgo Challoub, Florian Cunnac
 * @brief Fichier head du driver TIMER
 *********************************************************************/
 
#ifndef MYTIMERS_H
	#define MYTIMERS_H
	#include "stm32f10x.h"

	void MyTimer_Base_Init( TIM_TypeDef * Timer, unsigned short ARR, unsigned short PSC );
	void MyTimer_ActiveIT(TIM_TypeDef * Timer , char Prio ,void (*IT_function )(void));

	#define MyTimer_Base_Start(Timer) (Timer->CR1 |= TIM_CR1_CEN)
	#define MyTimer_Base_Stop(Timer) (Timer->CR1 &= ~TIM_CR1_CEN)
	
	void MyTimer_PWM( TIM_TypeDef * Timer , char Channel );
	void MyTimer_PWM_init( TIM_TypeDef * Timer , char Channel ,unsigned short ARR, unsigned short PSC );
	void MyTimer_PWM_dutyCycle( TIM_TypeDef * Timer , char Channel,  float alpha);


	void MyTimer_Config_line_9 (void);
	void MyTimer_Config_line_10 (void);
	void MyTimer_ActiveIT_EXTI (char Prio, int Pin, void (*IT_function)(void));

#endif
