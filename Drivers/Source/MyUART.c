#include "MyUART.h"
#include "stm32f10x.h"

void MyUSART_Init(USART_TypeDef * USART, int message_size, int message_period, double stop_bits, char parity){
	//double baud_rate = message_size*8/(message_period/1000);
	int divider = (USART == USART1)? 1 : 2;
	int baud_rate = 72/divider * 
		
	USART->BRR = (
	//cf p.804 du RM0008
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