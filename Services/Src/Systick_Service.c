#include "Systick_Service.h"
#include "stm32f10x.h"
#include "conf.h"
#include "TIMER_Driver.h"
#include <stdlib.h>

#define DIVIDER 2 // (CLOCK_FREQUENCY/(SYSTICK_TIMER_ARR*SYSTICK_TIMER_PSC))
int time_spent;

typedef void (*FunctionPointer)(int);
FunctionPointer FunctionArray[SYSTICK_SIZE]; // Fixé arbitrairement, permet de définir 10 fonctions, on pourrait faire de la réalloc mais pas nécessaire ici

#ifndef CONFIG_H
	#define SYSTICK_RAZ_INTERVAL 5 // en s
#endif

/**
 * @brief Fonction de rappel pour le service Systick.
 * @details Compte toutes les X ms et appelle les fonctions enregistrées.
 */
void SystickService_Callback() // compte toutes les 500ms
{
	int i;
	time_spent++;

	for(i = 0; i < SYSTICK_SIZE; i++) {
		if(FunctionArray[i]) {
			FunctionArray[i](time_spent);
		}
	}

	// Tous les SYSTICK_RAZ_INTERVAL secondes, on remet à 0, pour éviter de stocker un nombre trop grand
	time_spent = (time_spent / DIVIDER == SYSTICK_RAZ_INTERVAL) ? 0 : time_spent;
}

/**
 * @brief Enregistre une fonction pour le service Systick.
 * @details Permet à l'utilisateur d'ajouter une fonction à la liste des fonctions appelées périodiquement.
 * 
 * @param This Pointeur vers l'instance du service Systick.
 * @param function Pointeur vers la fonction de rappel.
 */
void SystickService_Register(SystickService *This, void (*function)(int))
{
    FunctionArray[This->registered_nb] = function;
    This->registered_nb++;
    This->registered_nb = (This->registered_nb == SYSTICK_SIZE) ? 0 : This->registered_nb;
    // écrase les premières fonctions, le mieux aurait été de faire de l'allocation dynamique et réallouer de la mémoire
    // mais trop de puissance pour rien
}

/**
 * @brief Configure le service Systick.
 * @details Initialise les paramètres nécessaires pour le service Systick.
 * 
 * @param This Pointeur vers l'instance du service Systick.
 */
void SystickService_Setup(SystickService *This)
{ 
	void (*SystickService_Callback_pointeur)(); /* Pointeur de fonction */
	time_spent = 0;
	This->registered_nb = 0;
	MyTimer_Base_Init(SYSTICK_TIMER, SYSTICK_TIMER_ARR, SYSTICK_TIMER_PSC);
	SystickService_Callback_pointeur = SystickService_Callback;
	MyTimer_ActiveIT(SYSTICK_TIMER, SYSTICK_TIMER_IT_PRIORITY, SystickService_Callback_pointeur);
}

/**
 * @brief Démarre le service Systick.
 * @details Active le timer pour commencer à appeler les fonctions périodiquement.
 * 
 * @param This Pointeur vers l'instance du service Systick.
 */
void SystickService_Start(SystickService *This)
{ 
	MyTimer_Base_Start(SYSTICK_TIMER);
}

/**
 * @brief Initialise l'instance du service Systick.
 * @details Initialise les fonctions membres de l'instance du service Systick.
 * 
 * @param This Pointeur vers l'instance du service Systick.
 */
static void  SystickService_Init(SystickService *This)
{
	This->Setup = SystickService_Setup;
	This->Start = SystickService_Start;
	This->Register = SystickService_Register;
}

/**
 * @brief Libère la mémoire allouée pour l'instance du service Systick.
 * 
 * @param This Pointeur vers l'instance du service Systick.
 */
void SystickService_New_Free(SystickService *This)
{
    free(This);        
}

/**
 * @brief Crée une nouvelle instance du service Systick.
 * @details Alloue la mémoire et initialise l'instance du service Systick.
 * 
 * @return Pointeur vers la nouvelle instance du service Systick.
 */
SystickService *New_Systick()
{
    SystickService *This = malloc(sizeof(SystickService));
    if(!This) return NULL;
    SystickService_Init(This);
    This->Free = SystickService_New_Free;
    return This;
}
