/*********************************************************************
 * @file  RTC_Service.c
 * @author Paul Gadanho
 * @brief Fichier source du service RTC
 *********************************************************************/
 
#include "RTC_Service.h"


MyI2C_RecSendData_Typedef Temps;
MyI2C_RecSendData_Typedef TempsOUT;
MyI2C_Err_Enum Err;

void (*ITErr_function)();



void Set_Time()
{
	#if MODE == 2 //en mode réel
		char T[2];
		T[0] = HORLOGE_DEFAULT_MIN;
		T[1] = HORLOGE_DEFAULT_HOUR;
		MyI2C_Init(I2C1,10,ITErr_function); // Déclaration tableau de carectères qui stockeront le temps

		// structure Time
		Temps.SlaveAdress7bits = 0x68; // adresse I2C du slave (1101000)
		Temps.Ptr_Data = T;
		Temps.Nb_Data = 2;

		MyI2C_PutString(I2C1,0x01, &Temps);
	#endif
}

void Get_Time(char * TOUT)
{
	#if MODE == 1 // en mode simulé
		TOUT[0] = 0;
		TOUT[1] = HORLOGE_DEFAULT_MIN;
		TOUT[2] = HORLOGE_DEFAULT_HOUR;
	#else
		TempsOUT.SlaveAdress7bits = 0x68; // adresse I2C du slave (1101000)
		TempsOUT.Ptr_Data = TOUT;
		TempsOUT.Nb_Data = 3;
		// Lecture Temps
		MyI2C_GetString(I2C1,0x00, &TempsOUT);

	#endif
}