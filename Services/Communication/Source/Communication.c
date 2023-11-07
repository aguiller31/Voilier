#include "Communication.h"
#include "stm32f10x.h"
#include "MyUART.h"
//baud_rate = 9600 Bd, 19200 Bd, 38400 Bd,..., 115200 Bd
//stop_bits = 1, 1.5, 2
//parité : c’est un 9ieme bit qui vaut 1 selon que le nombre de bit à 1 est pair ou impair (on parle de parité paire et impaire).
void init_communication(){
	MyUSART_Init(USART2, 9600, 1, 0);
	MyUSART_WriteCharacter(USART2, 'a');
}
int get_Pupitre_Adress(int address_voilier){
	return 80+address_voilier;
}
