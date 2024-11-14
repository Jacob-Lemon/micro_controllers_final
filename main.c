#include "stm32l476xx.h"
#include "uart.h"
#include "motor.h"
#include "delay.h"


/******************************************************************************
* main.c
* this is a comment
******************************************************************************/


int main(void){
	// initializations
	// init_uart();
	// init_adc();
	
	init_motor();
	
	while (1) {
		step_motor_clockwise(1);
		delay_ms(2000);
	}
	step_motor_clockwise(1);
	step_motor_counterclockwise(1);
	
	return 0;
}
