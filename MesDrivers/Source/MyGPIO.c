#include "MyGPIO.h"
#include "stm32f10x.h"

#define ONE 0x01
#define  FULL_ONE 0x0F
#define PORTCONF_SIZE 4


//permet d'activier le regirstre APB2 de la clock périphérique "When the peripheral clock is not active, the peripheral register values may not be readable
//by software and the returned value is always 0x0.e"

void MyGPIO_Init(GPIO_TypeDef * GPIO, char GPIO_Pin, char GPIO_Conf){
	int registre = GPIO_Pin%(PORTCONF_SIZE*2); //pour ne pas refaire le calcul dans toute la fonction
	volatile uint32_t *Ctrl;
	if(GPIO == GPIOA){
		RCC->APB2ENR |=RCC_APB2ENR_IOPAEN; //permet d'activier le regirstre APB2 de la clock périphérique "When the peripheral clock is not active, the peripheral register values may not be readable
//by software and the returned value is always 0x0.e"
	}else if(GPIO == GPIOB){
		RCC->APB2ENR |=RCC_APB2ENR_IOPBEN;
	}else{
		RCC->APB2ENR |=RCC_APB2ENR_IOPCEN;
	}
	 if(GPIO_Conf == In_PullUp){ //on met le registre en mode UP
		 MyGPIO_Set(GPIO, GPIO_Pin);
		 GPIO_Conf=In_PullDown;
	}else{
		MyGPIO_Reset(GPIO, GPIO_Pin); //on met tous à 0 lors de l'init
	}
	
	GPIO_Conf = (GPIO_Conf == In_PullUp) ? In_PullDown : GPIO_Conf; //on remet l'adresse des Pull
	Ctrl = (GPIO_Pin<PORTCONF_SIZE*2) ? &(GPIO->CRL) : &(GPIO->CRH);
	*Ctrl &= ~(FULL_ONE << PORTCONF_SIZE*registre);
	*Ctrl |= (GPIO_Conf << PORTCONF_SIZE*registre);
}
int MyGPIO_Read(GPIO_TypeDef * GPIO, char GPIO_Pin){
	return GPIO->IDR &(ONE<< GPIO_Pin);
}
int MyGPIO_Set(GPIO_TypeDef * GPIO, char GPIO_Pin){
	GPIO->BSRR = (ONE << GPIO_Pin);//pas besoin de |= car cela est plus efficace et c'est une bascule SET:RESET
	return MyGPIO_Read(GPIO, GPIO_Pin);
}
int MyGPIO_Reset(GPIO_TypeDef * GPIO, char GPIO_Pin){
	GPIO->BSRR = (ONE << (GPIO_Pin + 16)); //pas besoin de |= car cela est plus efficace et c'est une bascule SET:RESET
	return MyGPIO_Read(GPIO, GPIO_Pin);
}
int MyGPIO_Toggle(GPIO_TypeDef * GPIO, char GPIO_Pin){
	GPIO->ODR ^= (ONE << GPIO_Pin);
	return MyGPIO_Read(GPIO, GPIO_Pin);
}
