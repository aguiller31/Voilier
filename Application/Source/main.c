#include "stm32f10x.h"

#include "Communication_Service.h"
#include "Rotation_Service.h"
#include "Batterie_Service.h"
#include "Horloge_Service.h"
/*
	Informations sur la "Programation Orient�e Objet" dans ce programme :
	Le langage C n'�tant pas un langage orient� objet, il a fallut �tablir des r�gles de programmation afin d'obtenir une POO proche de la POO traditionelle.
	Pour ce faire j'ai suivi la norme propos�e par le site developpez.com : https://chgi.developpez.com/c/objet/
	Bien s�r cette norme ne fait qu'approcher la "vraie" POO mais ce choix a �t� fait pour faciliter le d�veloppement, la lisbilit� et la maintenance(th�orique) de ce code.
========================================
*/

void ( * Callback_pointeur_Communication_Babord ) (signed char) ;
void ( * Callback_pointeur_Communication_Tribord ) (signed char) ;
void ( * Callback_pointeur_Communication_0 ) () ;
void ( * Callback_pointeur_Batterie ) (int) ;

CommunicationService * ComSer;
RotationService * RotSer;
BatterieService * BatSer;
HorlogeService * HorSer;

void Callback_Communication_Tribord(signed char val){
	RotSer->SetDirection(RotSer,RIGHT);
	RotSer->SetSpeed(RotSer,20);
}

void Callback_Communication_Babord(signed char val){
	RotSer->SetDirection(RotSer,LEFT);
	RotSer->SetSpeed(RotSer,20);
}
void Callback_Communication_0(){
	RotSer->SetSpeed(RotSer,0);
}

void Callback_Batterie(int battery_level){
	if(battery_level < 20){
		ComSer->SendAlert(ComSer,ALERT_LOW_BATTERY);
	}
}
void setup(){
 

	ComSer = New_Communication();
	ComSer->Start(ComSer);
	
	RotSer = New_Rotation();
	RotSer->Setup(RotSer);
	
	BatSer = New_Batterie();
	BatSer->Setup(BatSer);
	
	HorSer = New_Horloge();
	HorSer->Setup(HorSer);
	
	Callback_pointeur_Communication_Babord = Callback_Communication_Babord ;
	Callback_pointeur_Communication_Tribord = Callback_Communication_Tribord ;
	Callback_pointeur_Communication_0 = Callback_Communication_0 ;
	Callback_pointeur_Batterie = Callback_Batterie ;

	ComSer->RegisterReadDirection(ComSer,BABORD,Callback_pointeur_Communication_Babord);
	ComSer->RegisterReadDirection(ComSer,TRIBORD,Callback_pointeur_Communication_Tribord);
	ComSer->RegisterReadChar(ComSer,'0',Callback_pointeur_Communication_0);
	ComSer->Read(ComSer);
	
	
	BatSer->RegisterBatteryLevel(BatSer,Callback_pointeur_Batterie);
	
	ComSer->WriteString(ComSer, "Test de String avec new line integre \r\n");
	ComSer->SendNewLine(ComSer);
	ComSer->WriteStringNL(ComSer, "Test de String avec new line fonction");
	
}

// il faut cr�er une clock pour appeler BatSer->GetBatteryLevel(BatSer); reguli�rement 


void loop(){

}

int main (void)
{
	setup();
	do
	{
		loop();
	}while(1);
}
