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

	// Structure d�crivant l'interface du service de Bordage
	typedef struct BordageService
	{
			// Fonction pour lib�rer la m�moire allou�e pour l'instance du service de Bordage
			void (*Free)(struct BordageService*);

			// Fonction pour param�trer le service de Bordage
			void (*Setup)(struct BordageService*);
		
			// Fonction pour changer le bordage en fonction de l'angle mesur� par la girouette
			void (*Change)(struct BordageService*,int);
		
			// Fonction qui renvoie l'angle teta du bordage
			int (*GetTeta)(struct BordageService*);
	
			// Fonction pour lacher les voiles
			void (*Lacher)(struct BordageService*);
		
			float teta;
			float duty_cycle;

	} BordageService;

	// Fonction pour cr�er une nouvelle instance du service de Bordage
	BordageService *New_Bordage(void);

#endif
