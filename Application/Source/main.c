#include "stm32f10x.h"
#include "conf.h"
#include "Communication_Service.h"
#include "Rotation_Service.h"

void ( * Callback_pointeur_Communication_Babord ) (signed char) ;
void ( * Callback_pointeur_Communication_Tribord ) (signed char) ;
void ( * Callback_pointeur_Communication_0 ) () ;

CommunicationService * ComSer;
RotationService * RotSer;

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
void setup(){
 

	ComSer = New_Communication();
	ComSer->Start(ComSer);
	
	RotSer = New_Rotation();
	RotSer->Setup(RotSer);
	
	Callback_pointeur_Communication_Babord = Callback_Communication_Babord ;
	Callback_pointeur_Communication_Tribord = Callback_Communication_Tribord ;
	Callback_pointeur_Communication_0 = Callback_Communication_0 ;

	ComSer->RegisterReadDirection(ComSer,BABORD,Callback_pointeur_Communication_Babord);
	ComSer->RegisterReadDirection(ComSer,TRIBORD,Callback_pointeur_Communication_Tribord);
	ComSer->RegisterReadChar(ComSer,'0',Callback_pointeur_Communication_0);
	ComSer->Read(ComSer);
	
	ComSer->WriteString(ComSer, "Test de String avec new line integre \r\n");
	ComSer->SendNewLine(ComSer);
	ComSer->WriteStringNL(ComSer, "Test de String avec new line fonction");
	ComSer->SendAlert(ComSer,ALERT_LOW_BATTERY);
}



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
