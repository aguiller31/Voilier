#ifndef HORLOGE_SERVICE
#define HORLOGE_SERVICE
#include "stm32f10x.h"
//#include "MyI2C.h"



typedef struct HorlogeService
{
	void(*Free)(struct HorlogeService*);

	void (*Setup)(struct HorlogeService*);

} HorlogeService ;
 HorlogeService * New_Horloge(void);

#endif
