#ifndef ROTATION_SERVICE
#define ROTATION_SERVICE
#include "stm32f10x.h"
#include "GPIO_Driver.h"
#include "TIMER_Driver.h"

#define LEFT 0
#define RIGHT 1

// Structure d�crivant l'interface du service de rotation
typedef struct RotationService
{
    // Fonction pour lib�rer la m�moire allou�e pour l'instance du service de rotation
    void (*Free)(struct RotationService*);

    // Fonction pour configurer le service de rotation
    void (*Setup)(struct RotationService*);

    // Fonction pour configurer la direction de rotation
    void (*SetupDirection)(struct RotationService*);

    // Fonction pour d�finir la direction de rotation
    void (*SetDirection)(struct RotationService*, int);

    // Fonction pour configurer la vitesse de rotation
    void (*SetupSpeed)(struct RotationService*);

    // Fonction pour d�finir la vitesse de rotation
    void (*SetSpeed)(struct RotationService*, float);

} RotationService;

// Fonction pour cr�er une nouvelle instance du service de rotation
RotationService *New_Rotation(void);

#endif
