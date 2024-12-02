#include "uart.h"
#include "stm32l476xx.h"

/*
uart specs
USART2 is connected to the ST_LINK USB interface via PA2 and PA3

configure USART2 to:
9600 baud
no hardware flow control
8 data bits
no parity
1 start bit
1 stop bit
set oversampling to 16
*/


void init_uart() {
	/* textbook main init stuff*/
	// enable the HSI
	RCC->CR |= RCC_CR_HSION; // set the clock control register to have the HSI on bit enabled
	while((RCC->CR & RCC_CR_HSIRDY) == 0); // loop until the HSI clock is ready

	// switch to HSI
	RCC->CFGR |= RCC_CFGR_SW_HSI;

	//----------GPIO init for usart2----------
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enable GPIO A clock
	// usart 2 tx-pa2 and rx-pa3
	
	//  select alternate function mode for PA2 and PA3
	// alt mode is 10
	// 1010_0000 = A0
	GPIOA->MODER &= 0xFFFFFFAF;
	GPIOA->MODER |= 0x000000A0;
	
	
	// alt function for USART 2 is 7
	
	GPIOA->AFR[0] |= 0x77 << (4*2); // this i think should set pin 2 and 3 to AF 7
	
	// GPIO speed
	GPIOA->OSPEEDR |= 0xF<<(2*2);
	
	// GPIO pullup and pulldown
	GPIOA->PUPDR &= ~(0xF<<(2*2));
	GPIOA->PUPDR |=  (0x5<<(2*2));
	
	// GPIO output type, 0=push pull, 1 = open drain
	GPIOA->OTYPER &= ~(0x3<<2);
	//---------------------------------------
	
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN; // enable uart 2 clock
	
	// select system clock usart clock source of uart 1
	RCC->CCIPR &= ~(RCC_CCIPR_USART2SEL);
	RCC->CCIPR |=  (RCC_CCIPR_USART2SEL_0);
	/* this code comes largely from the textbook. Book page 534. */
	
	// book references USARTx. I think we want USART2
	
	// Disable USART
	USART2->CR1 &= ~USART_CR1_UE;

	// set data length to 8 bits
	// 00 = 8 bits, 01 = 9 bits, and 10 = 7 data bits
	USART2->CR1 &= ~USART_CR1_M;
	
	
	// select 1 stop bit
	// 00 = 1 stop bit
	USART2->CR2 &= ~USART_CR2_STOP;
	
	// set parity as no parity
	// 0 - no parity
	USART2->CR1 &= ~USART_CR1_PCE;
	
	// Oversampling to 16
	// 0 = oversample 16, 1 = oversampling by 8
	USART2->CR1 &= ~USART_CR1_OVER8;
	
	// set baud rate to 9600
	// make sure to do calculations to verify this.
	// comes from the slides and is for 16MHz with 9600 baud rate
	// USART2->BRR = 0x257E; // 9598 in decimal
	USART2->BRR = 0x683; // needs to be actually this because APB frequency is 80 MHz
	// divide clock frequency by the baud rate (16MHz / 9600) = 1667 or something

	// enable transmission and reception
	USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	
	// Enable USART
	USART2->CR1 |= USART_CR1_UE;
	
	// verify that uart is ready for transmission
	// TEACK: Transmit Enable ACKnowledge flag. Set by hardware
	while ((USART2->ISR & USART_ISR_TEACK) == 0);

	// verify that uart is ready for reception
	// REACK: Receive Enable ACKnowledge flag. Set by hardware
	while ((USART2->ISR & USART_ISR_REACK) == 0);
	
	

	// now it should be all good to go
}


void uart_read(USART_TypeDef *USARTx, uint8_t *buffer, int nBytes) {
	int i;
	
	for (i=0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_RXNE)); // wait until hardware sets RXNE
		buffer[i] = USARTx->RDR;
	}
}

void uart_write(USART_TypeDef *USARTx, uint8_t *buffer, int nBytes) {
	int i;
	
	for (i=0; i<nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE)); // wait until hardware sets TXE
		USARTx->TDR = buffer[i] & 0xFF;					// writing to TDR clears TXE flag
	}
	
	// wait until TC bit is set
	while(!(USARTx->ISR & USART_ISR_TC));
	
	// write 1 until some things
	USARTx->ICR |= USART_ICR_TCCF; // transmission complete clear flag
	
}