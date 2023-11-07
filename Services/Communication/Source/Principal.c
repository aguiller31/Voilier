#include "stm32f10x.h"
#define PERIODE 100 //en ms
#define SIZE_MESSAGE_OCTET 1 // en ocet
#define MIN_MESSAGE_VAL -100
#define MAX_MESSAGE_VAL 100
#define CHANNEL 'F'
#define PAN_ID 111
#include "Communication.h"

//Only USART1 is clocked with PCLK2 (72 MHz Max). Other USARTs are clocked with PCLK1 (36 MHz Max).
void setup(){
MyUSART_Init(USART2, SIZE_MESSAGE_OCTET, PERIODE, 1, 0);

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
