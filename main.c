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

test

******************************************************************************/




int main(void){
	// set the clock to HSI, 16MHz, doesn't do anything
	//RCC->CR |= RCC_CR_HSION; // set the clock control register to have the HSI on bit enabled
	//while((RCC->CR & RCC_CR_HSIRDY) == 0); // loop until the HSI clock is ready
	
	// enable GPIO clocks
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enable GPIO clock A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // enable GPIO clock B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; // enable GPIO clock C
	// initializations
	// init_uart();
	// init_adc();
	
	// init_motor();
	
	init_hall_effect_sensors();
	
	delay_ms(20);
	
	init_shift_registers();
	
	delay_ms(20);
	
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
	
	



	//code that might reset all of them

	
	int reset_values[6] = {555, 570, 550, 550, 575, 575};
	int motor_reset_values[6] = {0};
	int magnet_detected[6] = {0};
	int motor_completed_reset[6] = {0};

	/*
	while (!(motor_completed_reset[0] && motor_completed_reset[1] && motor_completed_reset[2] 
		  && motor_completed_reset[3] && motor_completed_reset[4] && motor_completed_reset[5])) {
	*/
	
		
	while (!(motor_completed_reset[0] && motor_completed_reset[1])) {
	
	
	/*
	while (!(motor_completed_reset[0] && motor_completed_reset[1] && motor_completed_reset[2] 
		  && motor_completed_reset[3] && motor_completed_reset[4] && motor_completed_reset[5])) {
	*/
	
		for (int motor_id = 0; motor_id < 6; motor_id++) { //go through each motor
			if (!motor_completed_reset[motor_id]) {
				if (get_hall_data(motor_id) && !magnet_detected[motor_id]) { //keep rotating until the magnet is detected
					register_step_motor_once(motor_id);
				}
				else if (motor_reset_values[motor_id] < reset_values[motor_id]) { //move to blank
					register_step_motor_once(motor_id);
					magnet_detected[motor_id] = 1;
					motor_reset_values[motor_id] += 1;
				}
				else if (motor_reset_values[motor_id] == reset_values[motor_id]) { //motor_id index in the reset position so set flag
					motor_completed_reset[motor_id] = 1;
					// once all motors have completed going to blank the while loop will stop
				}
			}
			//delay_us(1); // reset sequence doesn't need delay with 2 motors apparently
		}
	}




	delay_ms(300);
	unsigned char next[6] = {'H', 'I', ' ', ' ', ' ', ' '};
	
	move_to_flap(next);
	
	delay_ms(250);
	
	next[0] = 'J';
	next[1] = 'A';
	
	move_to_flap(next);
	
	delay_ms(250);
	
	next[0] = 'C';
	next[1] = '0';
	
	move_to_flap(next);
	
	delay_ms(250);
	
	next[0] = 'B';
	next[1] = ' ';
	
	move_to_flap(next);
	
	
	// delay_ms(300);
	/*
	next[0] = 'S';
	
	move_to_flap(next);
	
	delay_ms(300);
	
	next[0] = 'A';
	
	move_to_flap(next);
	
	delay_ms(300);
	
	next[0] = 'A';
	
	move_to_flap(next);
	
	delay_ms(300);
	
	next[0] = 'A';
	
	move_to_flap(next);
	
	delay_ms(300);
	
	next[0] = 'A';
	
	move_to_flap(next);
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










