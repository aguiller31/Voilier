#include "stm32f10x.h"
#define PERIODE 100 //en ms
#define SIZE_MESSAGE_OCTET 1 // en ocet
#define MIN_MESSAGE_VAL -100
#define MAX_MESSAGE_VAL 100
#define CHANNEL 'F'
#define PAN_ID 111
#include "Communication.h"
CommunicationService * ComSer;


void Callback_USART2_a(){
	//write_communication('b');
	ComSer->WriteCharacter(ComSer,'b');
}

void setup(){
	void ( * Callback_pointeur ) ( ) ; 
	Callback_pointeur = Callback_USART2_a ;
	ComSer = New_Communication(USART2);
	ComSer->Start(ComSer);
	ComSer->RegisterRead(ComSer,'a',Callback_pointeur);
	ComSer->ReadCharacter(ComSer);
	/*
	void ( * Callback_pointeur ) ( ) ; 
	Callback_pointeur = Callback_a ;
	
	init_communication(USART2);
	read_communication();
	registerFunction_communication('a',Callback_pointeur);
	*/
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
