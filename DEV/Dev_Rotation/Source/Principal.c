#include "stm32f10x.h"
#include "Rotation.h"
void setup(){
	rotation_sens_setup(GPIOC,10);
	rotation_vitesse_setup(TIM2,2,GPIOA,10);
}
void loop(){
	/*rotation_sens(LEFT);
	rotation_vitesse(100);*/
}

int main (void)
{
	setup();
	do
	{
		loop();
	}while(1);
}
