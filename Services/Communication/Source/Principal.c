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
void Callback_USART2_Bytes(signed char c){
	//write_communication('b');
	ComSer->WriteCharacter(ComSer,c);
}

void Callback_USART2_Tribord(signed char val){
	
}

void Callback_USART2_Babord(signed char val){
	
}

void setup(){
	void ( * Callback_pointeur_USART2_a ) ( ) ; 
	void ( * Callback_pointeur_USART2_Bytes ) (signed char) ;
	void ( * Callback_pointeur_USART2_Babord ) (signed char) ;
	void ( * Callback_pointeur_USART2_Tribord ) (signed char) ;
	
	Callback_pointeur_USART2_a = Callback_USART2_a ; 
	Callback_pointeur_USART2_Bytes = Callback_USART2_Bytes ;
	Callback_pointeur_USART2_Babord = Callback_USART2_Babord ;
	Callback_pointeur_USART2_Tribord = Callback_USART2_Tribord ;
	
	ComSer = New_Communication(USART2);
	ComSer->Start(ComSer);
	ComSer->RegisterReadChar(ComSer,'a',Callback_pointeur_USART2_a);
	ComSer->RegisterReadBytes(ComSer,Callback_pointeur_USART2_Bytes);
	ComSer->RegisterReadDirection(ComSer,BABORD,Callback_pointeur_USART2_Babord);
	ComSer->RegisterReadDirection(ComSer,TRIBORD,Callback_pointeur_USART2_Tribord);
	ComSer->Read(ComSer);
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
