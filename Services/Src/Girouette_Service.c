#include "Girouette_Service.h"
#include "GPIO_Driver.h"
#include "TIMER_Driver.h"
#include "stm32f10x.h"
#include "conf.h"

int angle_girouette = 0;

void Remise_zero(void){
	angle_girouette = 0;
}

	
void Increment(void){
	if(MyGPIO_Read(GPIOC,8)!=0){
		angle_girouette += 1;
	} else {
		angle_girouette += -1;
		if(angle_girouette < 0){
			angle_girouette = 359;
		}
	}	
}
int getAngleGirouette(){
	return angle_girouette;
}
void InitGirouette(void){
	// Configuration PC 10 : Input Floating A
  MyGPIO_Init(GPIOC, 10, In_Floating);
	// Configuration PC 8 : Input Floating B
	MyGPIO_Init(GPIOC, 8, In_Floating);
	// Configuration PC 9 : Input Floating I
	MyGPIO_Init(GPIOC, 9, In_Floating);

		
	//Active la clock
	MyGPIO_Active_IT;
	
	MyTimer_Config_line_9();
	MyTimer_Config_line_10();
	MyTimer_ActiveIT_EXTI (0, 9, Remise_zero);
	MyTimer_ActiveIT_EXTI (1, 10,Increment);
}
