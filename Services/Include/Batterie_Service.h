#ifndef BATTERIE_SERVICE
#define BATTERIE_SERVICE
#include "stm32f10x.h"
#include "GPIO_Driver.h"
#include "TIMER_Driver.h"
#include "ADC_Driver.h"

// Structure décrivant l'interface du service de batterie
typedef struct BatterieService
{
    // Fonction pour libérer la mémoire allouée pour l'instance du service de batterie
    void (*Free)(struct BatterieService*);

    // Fonction pour configurer le service de batterie
    void (*Setup)(struct BatterieService*);

    // Fonction pour enregistrer la fonction de rappel du niveau de batterie
    void (*RegisterBatteryLevel)(struct BatterieService*, void (*function)(int));

    // Fonction pour obtenir le niveau de batterie
    void (*GetBatteryLevel)(struct BatterieService*);

} BatterieService;

// Fonction pour créer une nouvelle instance du service de batterie
BatterieService* New_Batterie(void);

#endif
