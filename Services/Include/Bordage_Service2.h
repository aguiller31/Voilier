/*********************************************************************
 * @file  Bordage_Service2.h
 * @author Nathan Laschon, Florian Cunnac
 * @brief Fichier head du service Bordage 
 *********************************************************************/
 
#ifndef BORDAGE_SERVICE
	#define BORDAGE_SERVICE
	#include "GPIO_Driver.h"
	#include "TIMER_Driver.h"
	#include "stm32f10x.h"
	#include "conf.h"
	#include <stdlib.h>

	// Structure décrivant l'interface du service de Bordage
	typedef struct BordageService
	{
			// Fonction pour libérer la mémoire allouée pour l'instance du service de Bordage
			void (*Free)(struct BordageService*);

			// Fonction pour paramétrer le service de Bordage
			void (*Setup)(struct BordageService*);
		
			// Fonction pour changer le bordage en fonction de l'angle mesuré par la girouette
			void (*Change)(struct BordageService*,int);
		
			// Fonction qui renvoie l'angle teta du bordage
			int (*GetTeta)(struct BordageService*);
	
			// Fonction pour lacher les voiles
			void (*Lacher)(struct BordageService*);
		
			float teta;
			float duty_cycle;

	} BordageService;

	// Fonction pour créer une nouvelle instance du service de Bordage
	BordageService *New_Bordage(void);

#endif
