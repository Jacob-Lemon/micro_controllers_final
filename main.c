#include "stm32l476xx.h"

#include "uart.h"
#include "motor.h"
#include "delay.h"
#include "gpio.h"
#include "global_variables.h"

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
	
	
	for (int i = 0; i<80; i++) {
		step_motor_clockwise(1);
		// step_motor_counterclockwise(1);
		// delay_ms(200);
	}
	
	return 0;
}



















/*
pin layout

9 for shift registers - PB0-8
3 for each:
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
		PB5 - STCP1
	reg2:
		PB6 - data2
		PB7 - SHCP2
		PB8 - STCP2



6 for all hall effect sensors - PC0-7
1 for each display:
	magnet_detected

	Hall 0:
		PC0
	Hall 1;
		PC1
	Hall 2:
		PC2
	Hall 3:
		PC3
	Hall 4;
		PC4
	Hall 5:
		PC5
	Hall 6:
		PC6
	Hall 7;
		PC7






*/










