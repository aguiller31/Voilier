#ifndef MYTIMERS_H
#define MYTIMERS_H
#include "stm32f10x.h"

/*
*****************************************************************************************
* @brie f
* @param -> Paramè t r e sous forme d ’ une s t r u c t u r e ( son a d re s se ) con ten an t l e s
i n f o rm a ti o n s de base
* @Note -> F onc ti on à l a n c e r s y s t ématiquement avan t d ’ a l l e r pl u s en dé t a i l dans l e s
con f pl u s f i n e s (PWM, codeur i n c . . . )
*************************************************************************************************
*/
void MyTimer_Base_Init( TIM_TypeDef * Timer, unsigned short ARR, unsigned short PSC );
void MyTimer_ActiveIT(TIM_TypeDef * Timer , char Prio ,void (*IT_function )(void));

#define MyTimer_Base_Start(Timer) (Timer->CR1 |= TIM_CR1_CEN)
#define MyTimer_Base_Stop(Timer) (Timer->CR1 &= ~TIM_CR1_CEN)
/**
* @brief
* @param . . . .
* @Note Active le channel spécifié sur Timer le timer spécifié
*la gestion de la configuration I/O n’est pas faite dans cette fonction
*ni le réglage de la période de la PWM (ARR,PSC)
*/
void MyTimer_PWM( TIM_TypeDef * Timer , char Channel );
void MyTimer_PWM_init( TIM_TypeDef * Timer , char Channel ,unsigned short ARR, unsigned short PSC );
void MyTimer_PWM_dutyCycle( TIM_TypeDef * Timer , char Channel,  float alpha);


void MyTimer_Config_line_9 (void);
void MyTimer_Config_line_10 (void);
void MyTimer_ActiveIT_EXTI (char Prio, int Pin, void (*IT_function)(void));

#endif
