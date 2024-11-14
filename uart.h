#ifndef uart_h
#define uart_h

#include "stm32l476xx.h"

// function prototypes

void init_uart();

void uart_read(USART_TypeDef *USARTx, uint8_t *buffer, int nBytes);

void uart_write(USART_TypeDef *USARTx, uint8_t *buffer, int nBytes);

void init_adc();


/*
Create a uart_read and uart_write functions based on 
book page 536 of the textbook
*/

#endif