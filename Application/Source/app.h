/*********************************************************************
 * @file  app.h
 * @author Antoine Guillermin
 * @brief Fichier head de l'application
 *********************************************************************/
 
#ifndef APPLICATION
	#define APPLICATION
	 
	#include "stm32f10x.h"
	#include "conf.h"
	#include "Systick_Service.h"
	#include "Communication_Service.h"
	#include "Rotation_Service.h"
	#include "Batterie_Service.h"
	#include "Girouette_Service.h"
	#include "Bordage_Service2.h"
	#include "Accelerometre_Service.h"
	#include "RTC_Service.h"
	#include <stdlib.h>



	// Structure d�crivant l'interface du service de Bordage
	typedef struct Application
	{
			// Fonction pour lib�rer la m�moire allou�e pour l'instance du service de Bordage
			void (*Free)(struct Application*);

			// Fonction pour d�marrer le service de Bordage
			void (*Setup)(struct Application*);
			
			void (*Start)(struct Application*);
		
		
			//Callbacks du SYSTICK
			void ( * Callback_pointeur_Systick_GetBattery ) () ;
			void ( * Callback_pointeur_Systick_Bordage ) () ;

			void ( * Callback_pointeur_Systick_Infos) () ;

			//Callbacks Rotation
			void ( * Callback_pointeur_Communication_Babord ) (signed char) ;
			void ( * Callback_pointeur_Communication_Tribord ) (signed char) ;
			void ( * Callback_pointeur_Communication_0 ) () ;
		
			//Callback batterie
			void ( * Callback_pointeur_Batterie ) (int) ;
			
			

	} Application;

	// Fonction pour cr�er une nouvelle instance du service de Bordage
	Application *New_Application(void);

#endif
