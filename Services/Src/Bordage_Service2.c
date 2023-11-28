#include "Bordage_Service2.h"

void BordageService_Setup(BordageService *This)
{
	MyGPIO_Init(BORDAGE_PWM_TIMER_GPIO, BORDAGE_PWM_TIMER_GPIO_PIN, AltOut_Ppull );
	MyTimer_PWM_init( BORDAGE_PWM_TIMER, BORDAGE_PWM_TIMER_CHANNEL, BORDAGE_PWM_TIMER_ARR, BORDAGE_PWM_TIMER_PSC );
	MyTimer_PWM_dutyCycle(BORDAGE_PWM_TIMER , BORDAGE_PWM_TIMER_CHANNEL, 0.080);
}
void BordageService_Change(BordageService *This, int angle_girouette)
{
	if(BORDAGE_MODE == 1){
		if(angle_girouette <45 || angle_girouette > 315){
				MyTimer_PWM_dutyCycle(BORDAGE_PWM_TIMER , BORDAGE_PWM_TIMER_CHANNEL, BORDAGE_PWM_TIMER_DUTYCYCLE_MODE1); //Régler pour augmenter au max
			} else {
				if(angle_girouette < 180){
					This->teta = angle_girouette*(0.667) -30;
				} else {
					This->teta = 180 - (angle_girouette*(0.667) - 30); 
				}
				This->duty_cycle = 0.05 +(This->teta * (0.05/90));
				MyTimer_PWM_dutyCycle(BORDAGE_PWM_TIMER , BORDAGE_PWM_TIMER_CHANNEL, This->duty_cycle); 
			}
		} else {
			if(angle_girouette <45 || angle_girouette > 315){
				MyTimer_PWM_dutyCycle(BORDAGE_PWM_TIMER , BORDAGE_PWM_TIMER_CHANNEL, BORDAGE_PWM_TIMER_DUTYCYCLE_MODE2);
			} else {
				if(angle_girouette < 180){
					This->teta = angle_girouette*(0.667) -30;
				} else {
					This->teta = 180 - (angle_girouette*(0.667) - 30); 
				}
				This->duty_cycle = 0.1 -(This->teta * (0.05/90));
				MyTimer_PWM_dutyCycle(BORDAGE_PWM_TIMER , BORDAGE_PWM_TIMER_CHANNEL, This->duty_cycle);
			}
		}
}


void BordageService_Lacher(BordageService *This){
	#if BORDAGE_MODE == 1
		MyTimer_PWM_dutyCycle(BORDAGE_PWM_TIMER , BORDAGE_PWM_TIMER_CHANNEL, BORDAGE_PWM_TIMER_DUTYCYCLE_MODE2); 
	#else
		MyTimer_PWM_dutyCycle(BORDAGE_PWM_TIMER , BORDAGE_PWM_TIMER_CHANNEL, BORDAGE_PWM_TIMER_DUTYCYCLE_MODE1); 
	#endif
	}

int BordageService_GetTeta(BordageService *This){
	return This->teta;
}
/**
 * @brief Initialise l'instance du service de Bordage.
 * 
 * @param This Pointeur vers l'instance du service de Bordage.
 */
static void BordageService_Init(BordageService *This)
{
	This->Setup = BordageService_Setup;
	This->Change = BordageService_Change;
	This->GetTeta = BordageService_GetTeta;
	This->Lacher = BordageService_Lacher;
}

/**
 * @brief Libère la mémoire allouée pour l'instance du service de Bordage.
 * 
 * @param This Pointeur vers l'instance du service de Bordage.
 */
void BordageService_New_Free(BordageService *This)
{
    free(This);        
}

/**
 * @brief Crée une nouvelle instance du service de Bordage.
 * @details Alloue la mémoire et initialise l'instance du service de Bordage.
 * 
 * @return Pointeur vers la nouvelle instance du service de Bordage.
 */
BordageService *New_Bordage()
{
    BordageService *This = malloc(sizeof(BordageService));
    if(!This) return NULL;
    BordageService_Init(This);
    This->Free = BordageService_New_Free;
		This->teta=0;
		This->duty_cycle=0;
    return This;
}
