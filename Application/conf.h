#ifndef CONFIG_H
#define CONFIG_H
	//GENERAL
	//SYSTICK
	#define SYSTICK_SIZE 10
	#define SYSTICK_RAZ_INTERVAL 100*60*5 //RAZ ttes les 5 minutes
	
	#define SYSTICK_TIMER_IT_PRIORITY 1
	
	#define SYSTICK_TIMER TIM1 //toutes les 10 ms
	#define SYSTICK_TIMER_ARR			36-1
	#define SYSTICK_TIMER_PSC			40000-1
	
	//ROTATION
	#define ROTATION_MODE 2
	#define ROTATION_DIRECTION_GPIO 			GPIOC
	#define ROTATION_DIRECTION_GPIO_PIN		6

	#define ROTATION_SPEED_GPIO 					GPIOB
	#define ROTATION_SPEED_GPIO_PIN				8

	#define ROTATION_SPEED_TIMER 					TIM4 //PB8
	#define ROTATION_SPEED_TIMER_CHANNEL	3
	#define ROTATION_SPEED_TIMER_ARR			61-1
	#define ROTATION_SPEED_TIMER_PSC			61-1


	//COMMUNICATION
	#define COMMUNICATION_UART_READ_IT_PRIORITY 0
	
	#define COMMUNICATION_UART USART3
	#define COMMUNICATION_BAUD_RATE 9600
	#define COMMUNICATION_WORD_LENGTH 0
	#define COMMUNICATION_STOP_BITS 1.0
	#define COMMUNICATION_PARITY_CONTROL 0


	#define USART1_GPIO GPIOA
	#define USART1_GPIO_RX_PIN 10
	#define USART1_GPIO_TX_PIN 9

	#define USART2_GPIO GPIOA
	#define USART2_GPIO_RX_PIN 3
	#define USART2_GPIO_TX_PIN 2

	#define USART3_GPIO GPIOB
	#define USART3_GPIO_RX_PIN 11
	#define USART3_GPIO_TX_PIN 10

	#define COMMUNICATION_ALERT_ROULIS "Detection Limite du Roulis"
	#define COMMUNICATION_ALERT_BATTERY "Tension de batterie trop faible"
	
	//BATTERIE
	#define BATTERIE_SYSTIC_PERIOD 10 //toutes les 10 ms, on mesure la batterie
	#define BATTERIE_LOW_LEVEL	80
	
	#define BATTERIE_ADC_IT_PRIORITY 1
	
	#define BATTERIE_ADC 			ADC1
	#define BATTERIE_ADC_CHANNEL	14
	#define BATTERIE_ADC_SAMPLING	7.5
	#define BATTERIE_ADC_TRIGGER 0x7
	#define BATTERIE_ADC_CC_VALUE 499
	
	#define BATTERIE_GPIO	GPIOC
	#define BATTERIE_GPIO_PIN	4
	
	//ACCELERO
	#define ACCELERO_SPI SPI1
	#define ACCELERO_GPIO_CS GPIOA
	#define ACCELERO_GPIO_CS_PIN 4
	#define ACCELERO_GPIO_CLK GPIOA
	#define ACCELERO_GPIO_CLK_PIN 5
	#define ACCELERO_GPIO_TX GPIOA
	#define ACCELERO_GPIO_TX_PIN 6
	#define ACCELERO_GPIO_RX GPIOA
	#define ACCELERO_GPIO_RX_PIN 7
	
	//BORDAGE
	#define BORDAGE_MODE 2
	#define BORDAGE_SYSTIC_PERIOD 2 //toutes les 20ms
	
	#define BORDAGE_PWM_TIMER 	TIM2
	#define BORDAGE_PWM_TIMER_CHANNEL	2
	#define BORDAGE_PWM_TIMER_DUTYCYCLE_MODE1	0.05
	#define BORDAGE_PWM_TIMER_DUTYCYCLE_MODE2	0.1
	
	#define BORDAGE_PWM_TIMER_ARR	40000-1
	#define BORDAGE_PWM_TIMER_PSC	36-1
	
	#define BORDAGE_PWM_TIMER_GPIO 	GPIOA
	#define BORDAGE_PWM_TIMER_GPIO_PIN 1
	
	//GIROUETTE
	#define GIROUETTE_SYSTIC_PERIOD 1
	
	#define  GIROUETTE_SIGNAL_A_GPIO GPIOC
	#define  GIROUETTE_SIGNAL_A_GPIO_PIN 10
	#define  GIROUETTE_SIGNAL_B_GPIO GPIOC
	#define  GIROUETTE_SIGNAL_B_GPIO_PIN 8
	#define  GIROUETTE_SIGNAL_RST_GPIO GPIOC
	#define  GIROUETTE_SIGNAL_RST_GPIO_PIN 9
#endif
