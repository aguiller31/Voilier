#include "UART_Driver.h"
#include "stm32f10x.h"
#include <stdlib.h>

/**
 * @brief Pointeur de fonction pour stocker les fonctions de rappel d'interruption RX.
 */
typedef void (*UART_Callback_RXpointer)(signed char, int);
UART_Callback_RXpointer UART_Callbacks[3];

/**
 * @brief Efface le registre de données de l'UART.
 * 
 * @param This Pointeur vers la structure UARTDriver.
 */
void UARTDriver_ClearDR(UARTDriver *This)
{
    This->UART->DR &= ~0xFF;
}

/**
 * @brief Active la transmission de l'UART.
 * 
 * @param This Pointeur vers la structure UARTDriver.
 */
void UARTDriver_EnableTX(UARTDriver *This)
{
    This->UART->CR1 |= USART_CR1_TE;
}

/**
 * @brief Désactive la transmission de l'UART.
 * 
 * @param This Pointeur vers la structure UARTDriver.
 */
void UARTDriver_DisableTX(UARTDriver *This)
{
    This->UART->CR1 &= ~USART_CR1_TE;
    This->ClearDR(This);
}

/**
 * @brief Active la réception de l'UART.
 * 
 * @param This Pointeur vers la structure UARTDriver.
 */
void UARTDriver_EnableRX(UARTDriver *This)
{
    This->UART->CR1 |= USART_CR1_RE;
}

/**
 * @brief Désactive la réception de l'UART.
 * 
 * @param This Pointeur vers la structure UARTDriver.
 */
void UARTDriver_DisableRX(UARTDriver *This)
{
    This->UART->CR1 &= ~USART_CR1_RE;
    This->ClearDR(This);
}

/**
 * @brief Définit le taux de bauds de l'UART.
 * 
 * @param This Pointeur vers la structure UARTDriver.
 * @param br   Taux de bauds.
 */
void UARTDriver_SetBaudRate(UARTDriver *This, int br)
{
    This->UART->BRR=(72.0 * 1000000.0) / (((This->UART == USART1) ? 1.0 : 2.0)  * (float)br);
	
}

/**
 * @brief Définit le nombre d'arrêts de bits de l'UART.
 * 
 * @param This Pointeur vers la structure UARTDriver.
 * @param sb   Nombre d'arrêts de bits.
 */
void UARTDriver_SetStopBits(UARTDriver *This, double sb)
{
    This->UART->CR2 &= ~USART_CR2_STOP;
    if (sb == 0.5)
    {
        This->UART->CR2 |= USART_CR2_STOP_0;
    }
    else if (sb == 2.0)
    {
        This->UART->CR2 |= USART_CR2_STOP_1;
    }
    else if (sb == 1.5)
    {
        This->UART->CR2 |= USART_CR2_STOP;
    }
}

/**
 * @brief Définit le contrôle de parité de l'UART.
 * 
 * @param This Pointeur vers la structure UARTDriver.
 * @param p    Contrôle de parité.
 */
void UARTDriver_SetParityControl(UARTDriver *This, char p)
{
    if (p == 0)
    {
        This->UART->CR1 &= ~USART_CR1_PCE;
    }
    else
    {
        This->UART->CR1 |= USART_CR1_PCE;
    }
}

/**
 * @brief Définit la longueur du mot de l'UART.
 * 
 * @param This Pointeur vers la structure UARTDriver.
 * @param wl   Longueur du mot.
 */
void UARTDriver_SetWordLength(UARTDriver *This, char wl)
{
    if (wl == 0)
    {
        This->UART->CR1 &= ~USART_CR1_M;
    }
    else
    {
        This->UART->CR1 |= USART_CR1_M;
    }
}

/**
 * @brief Démarre l'UART.
 * 
 * @param This Pointeur vers la structure UARTDriver.
 */
void UARTDriver_Start(UARTDriver *This)
{
    This->UART->CR1 |= USART_CR1_UE;
}

/**
 * @brief Écrit un caractère dans l'UART.
 * 
 * @param This      Pointeur vers la structure UARTDriver.
 * @param character Caractère à écrire.
 */
void UARTDriver_WriteCharacter(UARTDriver *This, char character)
{
    This->UART->DR = character;
    while ((This->UART->SR & USART_SR_TXE) != USART_SR_TXE)
    {
    } // Attente que le registre de données de transmission soit prêt
}

/**
 * @brief Écrit une chaîne de caractères dans l'UART.
 * 
 * @param This Pointeur vers la structure UARTDriver.
 * @param str  Chaîne de caractères à écrire.
 */
void UARTDriver_WriteString(UARTDriver *This, char *str)
{
    while (*str != '\0')
    {
        This->WriteCharacter(This, (*str));
        str++;
    }
    while (!(This->UART->SR & USART_SR_TC))
    {
    } // Attente que tous les octets soient transmis
}

/**
 * @brief Active l'interruption RX de l'UART.
 * 
 * @param This       Pointeur vers la structure UARTDriver.
 * @param Prio       Priorité de l'interruption.
 * @param IT_function Fonction de rappel d'interruption.
 */
void UARTDriver_ActiveIT(UARTDriver *This, char Prio, void (*IT_function)(signed char, int))
{
    // Interruption RX
    IRQn_Type IRQ_pin;
    This->UART->CR1 |= USART_CR1_RXNEIE;
    if (This->UART == USART1)
    {
        IRQ_pin = USART1_IRQn;
        UART_Callbacks[0] = IT_function;
    }
    else if (This->UART == USART2)
    {
        IRQ_pin = USART2_IRQn;
        UART_Callbacks[1] = IT_function;
    }
    else if (This->UART == USART3)
    {
        IRQ_pin = USART3_IRQn;
        UART_Callbacks[2] = IT_function;
    }

    NVIC_EnableIRQ(IRQ_pin);
    NVIC_SetPriority(IRQ_pin, Prio);
}

/**
 * @brief Initialise la structure UARTDriver.
 * 
 * @param This Pointeur vers la structure UARTDriver.
 */
static void UARTDriver_Init(UARTDriver *This)
{
    This->ClearDR = UARTDriver_ClearDR;
    This->EnableTX = UARTDriver_EnableTX;
    This->DisableTX = UARTDriver_DisableTX;
    This->EnableRX = UARTDriver_EnableRX;
    This->DisableRX = UARTDriver_DisableRX;
    This->SetBaudRate = UARTDriver_SetBaudRate;
    This->SetStopBits = UARTDriver_SetStopBits;
    This->SetParityControl = UARTDriver_SetParityControl;
    This->SetWordLength = UARTDriver_SetWordLength;
    This->Start = UARTDriver_Start;
    This->WriteCharacter = UARTDriver_WriteCharacter;
    This->WriteString = UARTDriver_WriteString;
    This->ActiveIT = UARTDriver_ActiveIT;
}

/**
 * @brief Libère la mémoire allouée pour la structure UARTDriver.
 * 
 * @param This Pointeur vers la structure UARTDriver.
 */
void UARTDriver_New_Free(UARTDriver *This)
{
    free(This);
}

/**
 * @brief Crée une nouvelle instance de la structure UARTDriver.
 * 
 * @param USART Pointeur vers le registre UART.
 * @return      Pointeur vers la nouvelle instance de la structure UARTDriver.
 */
UARTDriver *New_UART(USART_TypeDef *USART)
{
    UARTDriver *This = malloc(sizeof(UARTDriver));
    if (!This)
        return NULL;
    UARTDriver_Init(This);
    This->Free = UARTDriver_New_Free;
    This->UART = USART;
    if (USART == USART1)
    {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    }
    else if (USART == USART2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    }
    else if (USART == USART3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    }
    return This;
}

/**
 * @brief Gestionnaire d'interruption pour USART1.
 */
void USART1_IRQHandler(void)
{
    if ((USART1->SR & USART_SR_RXNE) == USART_SR_RXNE)
    {
        // RX
        (*UART_Callbacks[0])((signed char)USART1->DR, 1);
    }
}

/**
 * @brief Gestionnaire d'interruption pour USART2.
 */
void USART2_IRQHandler(void)
{
    if ((USART2->SR & USART_SR_RXNE) == USART_SR_RXNE)
    {
        // RX
        (*UART_Callbacks[1])((signed char)USART2->DR, 2);
    }
}

/**
 * @brief Gestionnaire d'interruption pour USART3.
 */
void USART3_IRQHandler(void)
{
    if ((USART3->SR & USART_SR_RXNE) == USART_SR_RXNE)
    {
        // RX
        (*UART_Callbacks[2])((signed char)USART3->DR, 3);
    }
}
