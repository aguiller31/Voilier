#ifndef MYUART_H
#define MYUART_H

#include "stm32f10x.h"

/**
 * @brief Structure repr�sentant le driver UART.
 */
typedef struct UARTDriver
{
    void (*Free)(struct UARTDriver *);                   /**< Pointeur vers la fonction de lib�ration de m�moire. */
    void (*ClearDR)(struct UARTDriver *);                /**< Pointeur vers la fonction d'effacement du registre de donn�es. */
    void (*EnableTX)(struct UARTDriver *);               /**< Pointeur vers la fonction d'activation de la transmission. */
    void (*DisableTX)(struct UARTDriver *);              /**< Pointeur vers la fonction de d�sactivation de la transmission. */
    void (*EnableRX)(struct UARTDriver *);               /**< Pointeur vers la fonction d'activation de la r�ception. */
    void (*DisableRX)(struct UARTDriver *);              /**< Pointeur vers la fonction de d�sactivation de la r�ception. */
    void (*SetBaudRate)(struct UARTDriver *, int);       /**< Pointeur vers la fonction de configuration du taux de bauds. */
    void (*SetStopBits)(struct UARTDriver *, double);    /**< Pointeur vers la fonction de configuration du nombre de bits d'arr�ts. */
    void (*SetParityControl)(struct UARTDriver *, char); /**< Pointeur vers la fonction de configuration du contr�le de parit�. */
    void (*SetWordLength)(struct UARTDriver *, char);    /**< Pointeur vers la fonction de configuration de la longueur du mot. */
    void (*Start)(struct UARTDriver *);                  /**< Pointeur vers la fonction de d�marrage de l'UART. */
    void (*WriteCharacter)(struct UARTDriver *, char);   /**< Pointeur vers la fonction d'�criture d'un caract�re dans l'UART. */
    void (*WriteString)(struct UARTDriver *, char *);    /**< Pointeur vers la fonction d'�criture d'une cha�ne de caract�res dans l'UART. */
    void (*ActiveIT)(struct UARTDriver *, char, void (*IT_function)(signed char, int)); /**< Pointeur vers la fonction d'activation de l'interruption RX. */
    USART_TypeDef *UART;                                /**< Pointeur vers le registre UART associ�. */
    double StopBits;                                    /**< Nombre de bits d'arr�ts. */
    char Parity;                                        /**< Contr�le de parit�. */
} UARTDriver;

/**
 * @brief Cr�e une nouvelle instance de la structure UARTDriver.
 * 
 * @param USART Pointeur vers le registre UART.
 * @return      Pointeur vers la nouvelle instance de la structure UARTDriver.
 */
UARTDriver *New_UART(USART_TypeDef *USART);

#endif
