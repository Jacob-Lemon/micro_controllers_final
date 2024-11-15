#include "stm32l476xx.h"

#include "uart.h"
#include "motor.h"
#include "delay.h"
#include "gpio.h"

/******************************************************************************
main.c
on register ids:
	an int. 0,1,2



******************************************************************************/


int main(void){
	// initializations
	// init_uart();
	// init_adc();
	
	init_motor();
	
	for (int i = 0; i<25; i++) {
		step_motor_clockwise(1);
		// delay_ms(200);
	}
	
	return 0;
}



















/*
pin layout

9 for shift registers - PB
3
	serial data
	SHCP
	STCP
	reg0:
		PB0 - data0
		PB1 - SHCP0
		PB2 - STCP0
	reg1:
		PB3 - data1
		PB4 - SHCP1
		PB4 - STCP1



6 for all hall effect sensors - PC




*/










