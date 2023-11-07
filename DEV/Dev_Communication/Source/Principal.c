#include "stm32f10x.h"
#define PERIODE 100 //en ms
#define SIZE_MESSAGE 1 // en ocet
#define MIN_MESSAGE_VAL -100
#define MAX_MESSAGE_VAL 100
#define CHANNEL 'F'
#define PAN_ID 111
#include "Communication.h"

void setup(){

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
