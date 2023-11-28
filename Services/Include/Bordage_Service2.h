#ifndef BORDAGE_SERVICE
#define BORDAGE_SERVICE
#include "GPIO_Driver.h"
#include "TIMER_Driver.h"
#include "stm32f10x.h"
#include "conf.h"
#include <stdlib.h>

// Structure d�crivant l'interface du service de Bordage
typedef struct BordageService
{
    // Fonction pour lib�rer la m�moire allou�e pour l'instance du service de Bordage
    void (*Free)(struct BordageService*);

    // Fonction pour d�marrer le service de Bordage
    void (*Setup)(struct BordageService*);
		void (*Change)(struct BordageService*,int);
		int (*GetTeta)(struct BordageService*);
		void (*Lacher)(struct BordageService*);
		float teta;
		float duty_cycle;

} BordageService;

// Fonction pour cr�er une nouvelle instance du service de Bordage
BordageService *New_Bordage(void);

#endif
