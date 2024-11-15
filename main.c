#include "stm32l476xx.h"
#include "uart.h"
#include "motor.h"
#include "delay.h"

//-----macros for using gpio.c---------
#define OUTPUT 1
#define INPUT 0

#define PUSHPULL 0
#define OPENDRAIN 1

#define NONE 0
#define PULLUP 1
#define PULLDOWN 2
//-------------------------------------
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
