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
	if(MyGPIO_Read(GIROUETTE_SIGNAL_B_GPIO,GIROUETTE_SIGNAL_B_GPIO_PIN)!=0){
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
  MyGPIO_Init(GIROUETTE_SIGNAL_A_GPIO, GIROUETTE_SIGNAL_A_GPIO_PIN, In_Floating);
	// Configuration PC 8 : Input Floating B
	MyGPIO_Init(GIROUETTE_SIGNAL_B_GPIO, GIROUETTE_SIGNAL_B_GPIO_PIN, In_Floating);
	// Configuration PC 9 : Input Floating I
	MyGPIO_Init(GIROUETTE_SIGNAL_RST_GPIO, GIROUETTE_SIGNAL_RST_GPIO_PIN, In_Floating);

		
	//Active la clock
	MyGPIO_Active_IT; // ??????
	
	MyTimer_Config_line_9();
	MyTimer_Config_line_10();
	MyTimer_ActiveIT_EXTI (0, 9, Remise_zero);
	MyTimer_ActiveIT_EXTI (1, 10,Increment);
}
