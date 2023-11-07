#include "Communication.h"
#include "stm32f10x.h"

//baud_rate = 9600 Bd, 19200 Bd, 38400 Bd,..., 115200 Bd
//stop_bits = 1, 1.5, 2
//parité : c’est un 9ieme bit qui vaut 1 selon que le nombre de bit à 1 est pair ou impair (on parle de parité paire et impaire).
void MyUSART_Init(USART_TypeDef * USART, int message_size, int message_period, double stop_bits, char parity){
	double baud_rate = message_size*8/(message_period/1000);
	//cf p.804 du RM0008
}
int get_Pupitre_Adress(int address_voilier){
	return 80+address_voilier;
}
double baud_rate_selector(double baud_rate){
	if(baud_rate <= 6){
		return 937.5;
	}else if(baud_rate <= 14.4){
		return 234.375;
	}else if(baud_rate <= 38.4){
		return 117.1875;
	}else if(baud_rate <= 86.4){
		return 39.0625;
	}else if(baud_rate <= 172.8){
		return 19.5;
	}else if(baud_rate <= 345.6){
		return 9.75;
	}else if(baud_rate <= 691.2){
		return 4.875;
	}else if(baud_rate <= 1585.8){
		return 2.4375;
	}else {
		return 1;
	}
}
int get_mantissa(double baud_rate){
	return (int)baud_rate;
}