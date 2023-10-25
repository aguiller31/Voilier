#include "../../MesDrivers/Include/MyGPIO.h"
#include "stm32f10x.h"
int i;
int main (void)
{

	MyGPIO_Init(GPIOA, 5,Out_Ppull); //led embarquée
	
	MyGPIO_Init(GPIOC, 8,In_Floating); //bouton
	MyGPIO_Init(GPIOC, 7,In_PullUp); //

	
	MyGPIO_Init(GPIOC, 10,Out_OD);
	do
{
	MyGPIO_Reset(GPIOA,5);

	/*
	if(MyGPIO_Read(GPIOC,8) == 0){
		MyGPIO_Reset(GPIOC,10);
	}else{
		MyGPIO_Set(GPIOC,10);
	}*/
	if(MyGPIO_Read(GPIOC,8) != 0){
		MyGPIO_Toggle(GPIOC,10);
		i=0;
		MyGPIO_Set(GPIOA,5);
		do{
			i++;
		}while(i < 10e5);
	}
}while(1);
}
