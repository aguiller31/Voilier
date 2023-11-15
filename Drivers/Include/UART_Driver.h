#ifndef MYUART_H
#define MYUART_H

#include "stm32f10x.h"

/**
 * @brief Structure représentant le driver UART.
 */
typedef struct UARTDriver
{
    void (*Free)(struct UARTDriver *);                   /**< Pointeur vers la fonction de libération de mémoire. */
    void (*ClearDR)(struct UARTDriver *);                /**< Pointeur vers la fonction d'effacement du registre de données. */
    void (*EnableTX)(struct UARTDriver *);               /**< Pointeur vers la fonction d'activation de la transmission. */
    void (*DisableTX)(struct UARTDriver *);              /**< Pointeur vers la fonction de désactivation de la transmission. */
    void (*EnableRX)(struct UARTDriver *);               /**< Pointeur vers la fonction d'activation de la réception. */
    void (*DisableRX)(struct UARTDriver *);              /**< Pointeur vers la fonction de désactivation de la réception. */
    void (*SetBaudRate)(struct UARTDriver *, int);       /**< Pointeur vers la fonction de configuration du taux de bauds. */
    void (*SetStopBits)(struct UARTDriver *, double);    /**< Pointeur vers la fonction de configuration du nombre de bits d'arrêts. */
    void (*SetParityControl)(struct UARTDriver *, char); /**< Pointeur vers la fonction de configuration du contrôle de parité. */
    void (*SetWordLength)(struct UARTDriver *, char);    /**< Pointeur vers la fonction de configuration de la longueur du mot. */
    void (*Start)(struct UARTDriver *);                  /**< Pointeur vers la fonction de démarrage de l'UART. */
    void (*WriteCharacter)(struct UARTDriver *, char);   /**< Pointeur vers la fonction d'écriture d'un caractère dans l'UART. */
    void (*WriteString)(struct UARTDriver *, char *);    /**< Pointeur vers la fonction d'écriture d'une chaîne de caractères dans l'UART. */
    void (*ActiveIT)(struct UARTDriver *, char, void (*IT_function)(signed char, int)); /**< Pointeur vers la fonction d'activation de l'interruption RX. */
    USART_TypeDef *UART;                                /**< Pointeur vers le registre UART associé. */
    double StopBits;                                    /**< Nombre de bits d'arrêts. */
    char Parity;                                        /**< Contrôle de parité. */
} UARTDriver;

/**
 * @brief Crée une nouvelle instance de la structure UARTDriver.
 * 
 * @param USART Pointeur vers le registre UART.
 * @return      Pointeur vers la nouvelle instance de la structure UARTDriver.
 */
UARTDriver *New_UART(USART_TypeDef *USART);

#endif
