#ifndef SYSTICK_SERVICE
#define SYSTICK_SERVICE
#include "stm32f10x.h"

// Structure d�crivant l'interface du service Systick
typedef struct SystickService
{
    // Fonction pour lib�rer la m�moire allou�e pour l'instance du service Systick
    void (*Free)(struct SystickService*);

    // Fonction pour configurer le service Systick
    void (*Setup)(struct SystickService*);

    // Fonction pour d�marrer le service Systick
    void (*Start)(struct SystickService*);

    // Fonction pour enregistrer une fonction de rappel dans le service Systick
    void (*Register)(struct SystickService *, void (*function)(int));

    // Nombre de fonctions enregistr�es dans le service Systick
    int registered_nb;

} SystickService;

// Fonction pour cr�er une nouvelle instance du service Systick
SystickService *New_Systick(void);

#endif
