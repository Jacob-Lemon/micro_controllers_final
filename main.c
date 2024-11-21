#include "stm32l476xx.h"

#include "uart.h"
#include "motor.h"
#include "delay.h"
#include "gpio.h"
#include "global_variables.h"
#include "shift.h"
#include "hall.h"

/******************************************************************************
main.c
on register ids:
	an int. 0,1,2



******************************************************************************/




int main(void){
	// enable GPIO clocks
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // enable GPIO clock B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; // enable GPIO clock C
	// initializations
	// init_uart();
	// init_adc();
	
	// init_motor();
	
	init_hall_effect_sensors();
	init_shift_registers();

	delay_ms(10);
	
	// reset the motors to blank
	
	
	/*
	for (int i=0; i<2000; i++) {
		register_step_motor_once(0);
		// register_step_motor_once(1);
		delay_us(10);
	}
	*/
	
	/*
	while (
		get_hall_data(0) ||
		get_hall_data(1) 
	) {
		if(!get_hall_data(0)) {
			register_step_motor_once(0);
		}
		else if (!get_hall_data(1)) {
			register_step_motor_once(1);
		}
	} // end loop
	*/
	
	
	
	
	while (get_hall_data(1)) {
		register_step_motor_once(0);
	}
	
	for (int i=0; i<525; i++) {
		register_step_motor_once(0);
	}
	delay_ms(500);
	
	int motors_to_rotate[6] = {1, 0, 0, 0, 0, 0};
	unsigned char next[6] = {'P', ' ', ' ', ' ', ' ', ' '};
	
	move_to_flap(motors_to_rotate, next);
	
	delay_ms(300);
	
	unsigned char next_next[6] = {'A', ' ', ' ', ' ', ' ', ' '};
	
	move_to_flap(motors_to_rotate, next_next);
	
	delay_ms(300);
	
	next_next[0] = 'X';
	
	move_to_flap(motors_to_rotate, next_next);
	
	delay_ms(300);
	
	next_next[0] = 'T';
	
	move_to_flap(motors_to_rotate, next_next);
	
	delay_ms(300);
	
	next_next[0] = '0';
	
	move_to_flap(motors_to_rotate, next_next);
	
	delay_ms(300);
	
	next_next[0] = 'N';
	
	move_to_flap(motors_to_rotate, next_next);
	
	// register_put_serial_data(0, 0b01010101);
	
	/*
	for (unsigned char i=0; i < 255; i++) {
		register_put_serial_data(0, i);
		delay_ms(100);
	}
	*/
	
	
	
	
	/*
	for (int i = 0; i<80; i++) {
		step_motor_clockwise(1);
		// step_motor_counterclockwise(1);
		// delay_ms(200);
	}
	*/
	
	
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










