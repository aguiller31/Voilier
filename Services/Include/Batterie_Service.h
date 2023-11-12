#ifndef BATTERIE_SERVICE
#define BATTERIE_SERVICE
#include "stm32f10x.h"
#include "GPIO_Driver.h"
#include "TIMER_Driver.h"
#include "ADC_Driver.h"


typedef struct BatterieService
{
	void(*Free)(struct BatterieService*);
	void (*SetupADC)(struct BatterieService*);
	void (*SetupTIMER)(struct BatterieService*);
	void (*Setup)(struct BatterieService*);
	void (*RegisterBatteryLevel)(struct BatterieService*,void (*function )(int));

} BatterieService ;
 BatterieService * New_Batterie(void);

#endif
