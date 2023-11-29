#include "Girouette_Service2.h"
#include "GPIO_Driver.h"
#include "TIMER_Driver.h"
#include "stm32f10x.h"
#include "conf.h"

int angle_girouette = 0;

void Remise_zero(void){
      angle_girouette = 0;
      GIROUETTE_TIMER->CNT = 0;
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

int getAngleGirouette2(){
      return (GIROUETTE_TIMER->CNT)/4;
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


void InitGirouette2(TIM_TypeDef * Timer){
      
      // Configuration PA 0 : Input Floating A
  MyGPIO_Init(GPIOA,0,In_PullDown);
      // Configuration PA 1 : Input Floating B
      MyGPIO_Init(GPIOA, 1, In_PullDown);
      // Configuration PA 1 : Input Floating B
      MyGPIO_Init(GPIOA, 1, In_PullDown);
      // Configuration PA 1 : Input Floating B
      MyGPIO_Init(GPIOA, 1, In_PullDown);

      MyTimer_Base_Init(GIROUETTE_TIMER ,1440-1,0);
      
      Timer->CCMR1 &= ~(0x03);
      Timer->CCMR1 |= (0x01);
      Timer->CCMR1 &= ~(0x03 <<8);
      Timer->CCMR1 |= (0x01 << 8);
      
      Timer->CCER |= (0x01);
      
      Timer->CCER &= ~(0x01 << 1);
      Timer->CCMR1 &= ~(0x0F << 4);
      Timer->CCER |= (0x01);
      
      Timer->CCER &= ~(0x01 << 5);
      Timer->CCMR1 &= ~(0x0F << 12);
      Timer->CCER |= (0x01 << 4);

      Timer->SMCR &= ~(0x07);
      Timer->SMCR |= (0x03); //011
      
      Timer->CR1 |= (0x01);
      
      // Configuration PC 9 : Input Floating I
      MyGPIO_Init(GIROUETTE_SIGNAL_RST_GPIO, GIROUETTE_SIGNAL_RST_GPIO_PIN, In_Floating);
      MyGPIO_Active_IT;
      MyTimer_Config_line_9();
      MyTimer_ActiveIT_EXTI (0, 9, Remise_zero);
      
}
