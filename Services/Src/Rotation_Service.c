#include "Rotation_Service.h"


#ifndef CONFIG_H
#define ROTATION_DIRECTION_GPIO 			GPIOC
#define ROTATION_DIRECTION_GPIO_PIN		10

#define ROTATION_SPEED_GPIO 				GPIOA
#define ROTATION_SPEED_GPIO_PIN			10

#define ROTATION_SPEED_TIMER 				TIM2
#define ROTATION_SPEED_TIMER_CHANNEL		2
#define ROTATION_SPEED_TIMER_ARR			60
#define ROTATION_SPEED_TIMER_PSC			60
#endif

/**
 * @brief Configuration de la direction de rotation.
 * @details Initialise la GPIO pour la direction de rotation.
 * 
 * @param This Pointeur vers l'instance du service de rotation.
 */
void RotationService_SetupDirection(RotationService *This)
{ 
    MyGPIO_Init(ROTATION_DIRECTION_GPIO, ROTATION_DIRECTION_GPIO_PIN, Out_Ppull);
}

/**
 * @brief Définir la direction de rotation.
 * 
 * @param This Pointeur vers l'instance du service de rotation.
 * @param dir Direction de rotation (LEFT ou RIGHT).
 */
void RotationService_SetDirection(RotationService *This, int dir)
{ 
    if(dir == LEFT){
        MyGPIO_Set(ROTATION_DIRECTION_GPIO, ROTATION_DIRECTION_GPIO_PIN);
    }else{
        MyGPIO_Reset(ROTATION_DIRECTION_GPIO, ROTATION_DIRECTION_GPIO_PIN);
    }
}

/**
 * @brief Configuration de la vitesse de rotation.
 * @details Initialise la GPIO et le timer pour la régulation de vitesse.
 * 
 * @param This Pointeur vers l'instance du service de rotation.
 */
void RotationService_SetupSpeed(RotationService *This)
{ 
    MyGPIO_Init(ROTATION_SPEED_GPIO, ROTATION_SPEED_GPIO_PIN, AltOut_Ppull);
    MyTimer_PWM_init(ROTATION_SPEED_TIMER, ROTATION_SPEED_TIMER_CHANNEL, ROTATION_SPEED_TIMER_ARR, ROTATION_SPEED_TIMER_PSC);
}

/**
 * @brief Définir la vitesse de rotation.
 * 
 * @param This Pointeur vers l'instance du service de rotation.
 * @param speed Vitesse de rotation (entre 0 et 100).
 */
void RotationService_SetSpeed(RotationService *This, float speed)
{ 
    MyTimer_PWM_dutyCycle(ROTATION_SPEED_TIMER, ROTATION_SPEED_TIMER_CHANNEL, (float)speed / 100.0);
}

/**
 * @brief Configuration globale du service de rotation.
 * @details Appelle les fonctions de configuration de direction et de vitesse.
 * 
 * @param This Pointeur vers l'instance du service de rotation.
 */
void RotationService_Setup(RotationService *This)
{ 
    This->SetupDirection(This);
    This->SetupSpeed(This);
}

/**
 * @brief Initialise l'instance du service de rotation.
 * 
 * @param This Pointeur vers l'instance du service de rotation.
 */
static void RotationService_Init(RotationService *This)
{
    This->Setup = RotationService_Setup;
    This->SetupDirection = RotationService_SetupDirection;
    This->SetDirection = RotationService_SetDirection;
    This->SetupSpeed = RotationService_SetupSpeed;
    This->SetSpeed = RotationService_SetSpeed;
}

/**
 * @brief Libère la mémoire allouée pour l'instance du service de rotation.
 * 
 * @param This Pointeur vers l'instance du service de rotation.
 */
void RotationService_New_Free(RotationService *This)
{
    free(This);        
}

/**
 * @brief Crée une nouvelle instance du service de rotation.
 * @details Alloue la mémoire et initialise l'instance du service de rotation.
 * 
 * @return Pointeur vers la nouvelle instance du service de rotation.
 */
RotationService *New_Rotation()
{
    RotationService *This = malloc(sizeof(RotationService));
    if (!This) return NULL;
    RotationService_Init(This);
    This->Free = RotationService_New_Free;
    return This;
}
