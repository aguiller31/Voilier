/*********************************************************************
 * @file  RTC_Service.h
 * @author Paul Gadanho
 * @brief Fichier head du service RTC
 *********************************************************************/
 
 #ifndef RTC_H
	
	#define RTC_H
	#include "stm32f10x.h"
	#include "conf.h"
	#include "MyI2C.h"

	void Get_Time(char * T);
	void Set_Time(void);
#endif
