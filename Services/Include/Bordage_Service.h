#ifndef BORDAGE_SERVICE_H
#define BORDAGE_SERVICE_H
#include "stm32f10x.h"

extern float teta;
extern float duty_cycle;

void InitBordage(void);
void bordage(void);

#endif
