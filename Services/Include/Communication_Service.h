/*********************************************************************
 * @file  Communication_Service.h
 * @author Antoine Guillermin, Yorgo Challoub
 * @brief Fichier head du service Communication 
 *********************************************************************/
 
#ifndef COMMUNICATION_SERVICE
	#define COMMUNICATION_SERVICE
	#include "stm32f10x.h"
	#include "UART_Driver.h"
	#include "GPIO_Driver.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include "conf.h"
	
	#define TRIBORD 1
	#define BABORD 0

	#define ALERT_DLR 0
	#define ALERT_LOW_BATTERY 1

	// Structure décrivant l'interface du service de communication
	typedef struct CommunicationService
	{
			// Fonction pour libérer la mémoire allouée pour l'instance du service de communication
			void (*Free)(struct CommunicationService*);

			// Fonction pour démarrer le service de communication
			void (*Start)(struct CommunicationService*);

			// Fonction pour écrire un caractère par l'UART
			void (*WriteCharacter)(struct CommunicationService*, char);

			// Fonction pour écrire un entier par l'UART
			void (*WriteInt)(struct CommunicationService*, int);
		
			// Fonction pour écrire une chaîne de caractères par l'UART
			void (*WriteString)(struct CommunicationService*, char*);

			// Fonction pour écrire une chaîne de caractères suivie d'une nouvelle ligne par l'UART
			void (*WriteStringNL)(struct CommunicationService*, char*);

			// Fonction pour envoyer un message d'alerte par l'UART
			void (*SendAlert)(struct CommunicationService*, int);
			
			// Fonction pour envoyer une nouvelle ligne par l'UART
			void (*SendNewLine)(struct CommunicationService*);
			
			// Fonction pour envoyer l'heure actuelle
			void (*WriteTime)(struct CommunicationService*, char*);
			// Fonction pour enregistrer une fonction callback pour chaque caractère reçu
			void (*RegisterReadChar)(struct CommunicationService*, signed char, void (*function)());

			// Fonction pour enregistrer une fonction callback pour la réception de bytes
			void (*RegisterReadBytes)(struct CommunicationService*, void (*function)(signed char));

			// Fonction pour enregistrer une fonction callback pour la réception de direction
			void (*RegisterReadDirection)(struct CommunicationService*, int, void (*function)(signed char));

			// Fonction pour configurer le service de communication pour la lecture
			void (*Read)(struct CommunicationService*);

			// Pointeur vers l'instance du driver UART utilisé par le service
			UARTDriver * UART;

			// Numéro de l'UART utilisé par le service
			int UART_nb;

	} CommunicationService;

	// Fonction pour créer une nouvelle instance du service de communication
	CommunicationService *New_Communication(void);

#endif
