#include "stm32f10x.h"
#include "conf.h"
#include "Systick_Service.h"
#include "Communication_Service.h"
#include "Rotation_Service.h"
#include "Batterie_Service.h"
#include "Horloge_Service.h"
#include "Girouette_Service.h"
#include "Bordage_Service.h"
/*
	Informations sur la "Programation Orientée Objet" dans ce programme :
	Le langage C n'étant pas un langage orienté objet, il a fallut établir des règles de programmation afin d'obtenir une POO proche de la POO traditionelle.
	Pour ce faire j'ai suivi la norme proposée par le site developpez.com : https://chgi.developpez.com/c/objet/
	Bien sûr cette norme ne fait qu'approcher la "vraie" POO mais ce choix a été fait pour faciliter le développement, la lisbilité et la maintenance(théorique) de ce code.
========================================
*/
int main_angle_girouette;
void ( * Callback_pointeur_Systick_GetBattery ) (int) ;
void ( * Callback_pointeur_Systick_Bordage ) (int) ;
void ( * Callback_pointeur_Systick_GetAngleGirouette) (int) ;

void ( * Callback_pointeur_Communication_Babord ) (signed char) ;
void ( * Callback_pointeur_Communication_Tribord ) (signed char) ;
void ( * Callback_pointeur_Communication_0 ) () ;
void ( * Callback_pointeur_Batterie ) (int) ;
void ( * Callback_pointeur_Systick_HelloWorld ) (int) ;
SystickService * SysSer;
CommunicationService * ComSer;
RotationService * RotSer;
BatterieService * BatSer;
HorlogeService * HorSer;

void Callback_Systick_GetAngleGirouette(int time)
{
	if(time%GIROUETTE_SYSTIC_PERIOD ==0)
		main_angle_girouette=getAngleGirouette();
}
void Callback_Systick_GetBattery(int time)
{
	if(time%BATTERIE_SYSTIC_PERIOD ==0)
		BatSer->GetBatteryLevel(BatSer);
}

void Callback_Systick_Bordage(int time)
{
	if(time%BORDAGE_SYSTIC_PERIOD ==0){
		bordage(main_angle_girouette);
	}
}
void Callback_Communication_Tribord(signed char val){
	RotSer->SetDirection(RotSer,RIGHT);
	RotSer->SetSpeed(RotSer,(int)val);
}

void Callback_Communication_Babord(signed char val){
	RotSer->SetDirection(RotSer,LEFT);
	RotSer->SetSpeed(RotSer,(int)val);
}
void Callback_Communication_0(){
	RotSer->SetSpeed(RotSer,0);
}

void Callback_Batterie(int battery_level){
	if(battery_level < BATTERIE_LOW_LEVEL){
		ComSer->SendAlert(ComSer,ALERT_LOW_BATTERY);
	}
}

void setup(){
 
	SysSer = New_Systick();
	SysSer->Setup(SysSer);
	
	
	ComSer = New_Communication();
	ComSer->Start(ComSer);
	
	RotSer = New_Rotation();
	RotSer->Setup(RotSer);
	
	BatSer = New_Batterie();
	BatSer->Setup(BatSer);
	
	HorSer = New_Horloge();
	HorSer->Setup(HorSer);
	
	Callback_pointeur_Systick_GetBattery = Callback_Systick_GetBattery;
	Callback_pointeur_Systick_Bordage = Callback_Systick_Bordage;
	Callback_pointeur_Systick_GetAngleGirouette = Callback_Systick_GetAngleGirouette;
	
	Callback_pointeur_Communication_Babord = Callback_Communication_Babord ;
	Callback_pointeur_Communication_Tribord = Callback_Communication_Tribord ;
	Callback_pointeur_Communication_0 = Callback_Communication_0 ;
	Callback_pointeur_Batterie = Callback_Batterie ;

	ComSer->RegisterReadDirection(ComSer,BABORD,Callback_pointeur_Communication_Babord);
	ComSer->RegisterReadDirection(ComSer,TRIBORD,Callback_pointeur_Communication_Tribord);
	ComSer->RegisterReadChar(ComSer,0,Callback_pointeur_Communication_0);
	
	ComSer->Read(ComSer);
	
	BatSer->RegisterBatteryLevel(BatSer,Callback_pointeur_Batterie);
	
	SysSer->Register(SysSer,Callback_pointeur_Systick_GetBattery); //pour toutes les 3sec
	SysSer->Register(SysSer,Callback_pointeur_Systick_Bordage); //pour toutes les 20 ms
	SysSer->Register(SysSer,Callback_pointeur_Systick_GetAngleGirouette);
	
	
	InitGirouette();
	InitBordage();
	SysSer->Start(SysSer);
	
}

void loop(){}

int main (void)
{
	setup();
	do
	{
		loop();
	}while(1);
}
