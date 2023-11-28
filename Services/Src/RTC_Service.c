#include "RTC_Service.h"


MyI2C_RecSendData_Typedef Temps;
	MyI2C_Err_Enum Err;
void Erreur()
	{
		Err = MyI2C_Get_Error(I2C1);
	}
	void (*ITErr_function)();
void Set_Time()
{

char T[3]={HORLOGE_DEFAULT_SEC,HORLOGE_DEFAULT_MIN,HORLOGE_DEFAULT_HOUR};


	MyI2C_Init(I2C1,10,ITErr_function);
// Déclaration tableau de charectère qui stockeront le temps

// structure Time
Temps.SlaveAdress7bits = 0x68; // adresse I2C du slave (1101000)
Temps.Ptr_Data = T;
Temps.Nb_Data = 3;

MyI2C_PutString(I2C1,0x00, &Temps);
}


char * Get_Time()
{
// Déclaration tableau de charectère qui stockeront le temps

char T[3];

TimePaul *WTII;

// structure Time
Temps.SlaveAdress7bits = 0x68; // adresse I2C du slave (1101000)
Temps.Ptr_Data = T;
Temps.Nb_Data = 3;



// Lecture Temps
MyI2C_GetString(I2C1,0x00, &Temps);

	return Temps.Ptr_Data;

}