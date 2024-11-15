#include "stm32l476xx.h"

#include "uart.h"
#include "motor.h"
#include "delay.h"
#include "gpio.h"

/******************************************************************************
* main.c
* this is a comment
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
