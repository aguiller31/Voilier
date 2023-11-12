#ifndef CONFIG_H
	#define CONFIG_H

	//ROTATION
	#define ROTATION_DIRECTION_GPIO 			GPIOC
	#define ROTATION_DIRECTION_GPIO_PIN		10

	#define ROTATION_SPEED_GPIO 					GPIOA
	#define ROTATION_SPEED_GPIO_PIN				10

	#define ROTATION_SPEED_TIMER 					TIM2
	#define ROTATION_SPEED_TIMER_CHANNEL	2
	#define ROTATION_SPEED_TIMER_ARR			61-1
	#define ROTATION_SPEED_TIMER_PSC			61-1


	//COMMUNICATION
	#define COMMUNICATION_UART USART1
	#define COMMUNICATION_BAUD_RATE 9600
	#define COMMUNICATION_WORD_LENGTH 1
	#define COMMUNICATION_STOP_BITS 1.0
	#define COMMUNICATION_PARITY_CONTROL 0


	#define USART1_GPIO GPIOA
	#define USART1_GPIO_RX 10
	#define USART1_GPIO_TX 9

	#define USART2_GPIO GPIOA
	#define USART2_GPIO_RX 3
	#define USART2_GPIO_TX 2

	#define USART3_GPIO GPIOB
	#define USART3_GPIO_RX 11
	#define USART3_GPIO_TX 10

	#define COMMUNICATION_ALERT_ROULIS "Detection Limite du Roulis"
	#define COMMUNICATION_ALERT_BATTERY "Tension de batterie trop faible"
	
	//BATTERIE
	#define BATTERIE_ADC_IT_PRIORITY 4
	#define BATTERIE_ADC 			ADC1
	#define BATTERIE_ADC_CHANNEL	0
	#define BATTERIE_ADC_SAMPLING	7.5
	#define BATTERIE_ADC_TRIGGER TIM4_CC4_event
	#define BATTERIE_ADC_CC_VALUE 1000/2-1
	
	#define BATTERIE_TIMER_ARR			16000-1
	#define BATTERIE_TIMER_PSC			1000-1
	
	#define BATTERIE_GPIO	GPIOA
	#define BATTERIE_GPIO_PIN	0
#endif
