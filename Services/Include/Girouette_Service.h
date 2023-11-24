#ifndef GIROUETTE_SERVICE_H
#define GIROUETTE_SERVICE_H
#include "stm32f10x.h"

extern int angle_girouette;

void Remise_zero(void);
void Increment(void);
void InitGirouette(void);

int getAngleGirouette(void);

#endif

