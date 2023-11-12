#include "Horloge_Service.h"

#include "stm32f10x.h"
#include "conf.h"
#include <stdlib.h>

#ifndef CONFIG_H
#endif


void HorlogeService_Setup(HorlogeService *This)
{ 
	
}

static void  HorlogeService_Init( HorlogeService *This)
{
			This->Setup = HorlogeService_Setup;

}
void HorlogeService_New_Free(HorlogeService *This)
{
        free(This);        
}
HorlogeService * New_Horloge()
{
       HorlogeService *This = malloc(sizeof(HorlogeService));
       if(!This) return NULL;
       HorlogeService_Init(This);
			 This->Free = HorlogeService_New_Free;
       return This;
}
