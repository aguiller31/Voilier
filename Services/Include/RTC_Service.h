#ifndef RTC_H
#define RTC_H
#include "stm32f10x.h"
#include "conf.h"
#include "MyI2C.h"
#include "GPIO_Driver.h"


char * Get_Time(void);
void Set_Time(void);

#endif