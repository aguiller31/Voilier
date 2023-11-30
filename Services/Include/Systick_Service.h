/*********************************************************************
 * @file  Systick_Service.h
 * @author Antoine Guillermin
 * @brief Fichier head du service Systick 
 *********************************************************************/
#ifndef SYSTICK_SERVICE
	#define SYSTICK_SERVICE
	#include "stm32f10x.h"
	#include "conf.h"
	#include "TIMER_Driver.h"
	#include <stdlib.h>

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
			void (*Register)(struct SystickService *, int, void (*function)(int));

			// Nombre de fonctions enregistr�es dans le service Systick
			int registered_nb;
			int period;
	} SystickService;

	// Fonction pour cr�er une nouvelle instance du service Systick
	SystickService *New_Systick(void);
#endif
