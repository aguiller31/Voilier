#include "Batterie_Service.h"
#include "stm32f10x.h"
#include "conf.h"
#include <stdlib.h>

#ifndef CONFIG_H
	#define BATTERIE_ADC_IT_PRIORITY 4
	#define BATTERIE_ADC 			ADC1
	#define BATTERIE_ADC_CHANNEL	0
	#define BATTERIE_ADC_SAMPLING	7.5
	#define BATTERIE_ADC_TRIGGER TIM4_CC4_event
	#define BATTERIE_ADC_CC_VALUE 1000/2-1
	
	#define BATTERIE_TIMER_ARR			16000-1
	#define BATTERIE_TIMER_PSC			1000-1
	
	#define BATTERIE_GPIO	GPIOA
	#define BATTERIE_GPIO_PIN	0
#endif

/**
 * @brief Pointeur vers la fonction de rappel pour la gestion de la batterie.
 * @details Utilisé pour appeler une fonction externe lorsque la tension de la batterie est mesurée.
 * 
 * @param nbre Niveau de batterie mesuré.
 */
void ( * Batterie_Callback_pointeur ) ( int ) ;

/**
 * @brief Fonction de rappel pour la gestion de la batterie.
 * @details Appelée après la mesure de la tension de la batterie.
 * 
 * @param nbre Niveau de batterie mesuré.
 */
void BatterieService_Callback(int nbre)
{
	(*Batterie_Callback_pointeur) (nbre/92*100);
}

/**
 * @brief Configure le service de la batterie.
 * @details Initialise les paramètres nécessaires pour la mesure de la tension de la batterie.
 * 
 * @param This Pointeur vers l'instance du service de batterie.
 */
void BatterieService_Setup(BatterieService *This)
{ 
	void ( * BatterieService_Callback_pointeur ) ( int ) ; /* Pointeur de fonction */
	MyADC_Base_Init(BATTERIE_ADC, BATTERIE_ADC_CHANNEL,BATTERIE_ADC_SAMPLING,SW_EVENT );
	BatterieService_Callback_pointeur = BatterieService_Callback ; /* Affectation */
	MyADC_ActiveIT(BATTERIE_ADC,BATTERIE_ADC_IT_PRIORITY,BatterieService_Callback_pointeur);
}

/**
 * @brief Enregistre la fonction de rappel pour le niveau de batterie.
 * @details Permet à l'utilisateur de spécifier une fonction à appeler lors de la mesure de la tension de la batterie.
 * 
 * @param This Pointeur vers l'instance du service de batterie.
 * @param function Pointeur vers la fonction de rappel.
 */
void BatterieService_RegisterBatteryLevel(BatterieService *This, void (*function)(int)) {
    Batterie_Callback_pointeur = function;
}

/**
 * @brief Démarre la mesure de la tension de la batterie.
 * @details Active l'ADC pour mesurer la tension de la batterie.
 * 
 * @param This Pointeur vers l'instance du service de batterie.
 */
void BatterieService_GetBatteryLevel(BatterieService *This) {
	MyADC_SwStart(BATTERIE_ADC);
}

/**
 * @brief Initialise l'instance du service de batterie.
 * @details Initialise les fonctions membres de l'instance du service de batterie.
 * 
 * @param This Pointeur vers l'instance du service de batterie.
 */
static void  BatterieService_Init( BatterieService *This)
{
	This->Setup = BatterieService_Setup;
	This->RegisterBatteryLevel = BatterieService_RegisterBatteryLevel;
	This->GetBatteryLevel = BatterieService_GetBatteryLevel;
}

/**
 * @brief Libère la mémoire allouée pour l'instance du service de batterie.
 * 
 * @param This Pointeur vers l'instance du service de batterie.
 */
void BatterieService_New_Free(BatterieService *This)
{
    free(This);        
}

/**
 * @brief Crée une nouvelle instance du service de batterie.
 * @details Alloue la mémoire et initialise l'instance du service de batterie.
 * 
 * @return Pointeur vers la nouvelle instance du service de batterie.
 */
BatterieService * New_Batterie()
{
    BatterieService *This = malloc(sizeof(BatterieService));
    if(!This) return NULL;
    BatterieService_Init(This);
    This->Free = BatterieService_New_Free;
    return This;
}
