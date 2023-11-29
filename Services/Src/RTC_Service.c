#include "RTC_Service.h"


MyI2C_RecSendData_Typedef Temps;
MyI2C_RecSendData_Typedef TempsOUT;
	MyI2C_Err_Enum Err;

void Erreur()
	{
		Err = MyI2C_Get_Error(I2C1);
	}
	void (*ITErr_function)();
void Set_Time()
{
	#if MODE == 2

	char T[2];
	T[0] = HORLOGE_DEFAULT_MIN;
	T[1] = HORLOGE_DEFAULT_HOUR;
	MyI2C_Init(I2C1,10,ITErr_function);
// Déclaration tableau de charectère qui stockeront le temps

// structure Time
Temps.SlaveAdress7bits = 0x68; // adresse I2C du slave (1101000)
Temps.Ptr_Data = T;
Temps.Nb_Data = 2;

MyI2C_PutString(I2C1,0x01, &Temps);
	#endif
}

void Init_RTC(){
	
}
void Get_Time(char * TOUT)
{//={s,m,h};
	
// Déclaration tableau de charectère qui stockeront le temps
#if MODE == 1
	TOUT[0] = HORLOGE_DEFAULT_SEC;
	TOUT[1] = HORLOGE_DEFAULT_MIN;
	TOUT[2] = HORLOGE_DEFAULT_HOUR;
	return T;
#else
	TempsOUT.SlaveAdress7bits = 0x68; // adresse I2C du slave (1101000)
	TempsOUT.Ptr_Data = TOUT;
	TempsOUT.Nb_Data = 3;

	// Lecture Temps
	MyI2C_GetString(I2C1,0x00, &TempsOUT);

#endif

}