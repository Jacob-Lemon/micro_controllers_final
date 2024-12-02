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
	
	
	///*
	
	//uart things
	
	// initialize uart
	init_uart();
	
	//for clearing terminal and going to top left
	unsigned char clear[] = "\033[2Ja";
	clear[5] = 12;
	#define CLEAR_SIZE 7
	
	//clear the current terminal display
	uart_write(USART2, clear, CLEAR_SIZE);

	//message that asks user for a word
	#define ASK_MESSAGE_SIZE 51
	unsigned char ask_for_word[ASK_MESSAGE_SIZE] = "Enter in a 6 letter or less word and press enter:\n\r";
	
	// various messages
	#define DISPLAYING_SIZE 13
	unsigned char displaying[DISPLAYING_SIZE] = "Displaying...";
	
	#define RESETTING_SIZE 12
	unsigned char resetting[RESETTING_SIZE] = "Resetting...";
	
	
	// reset the motors to blank
	


	//display resetting while the flaps are initializing
	uart_write(USART2, resetting, RESETTING_SIZE);
	
	
	//*/

	//code that might reset all of them

	
	int reset_values[6] = {570, 510, 500, 600, 630, 540};
	int motor_reset_values[6] = {0};
	int magnet_detected[6] = {0};
	int motor_completed_reset[6] = {0};

	
	while (!(motor_completed_reset[0] && motor_completed_reset[1] && motor_completed_reset[2] 
		  && motor_completed_reset[3] && motor_completed_reset[4] && motor_completed_reset[5])) {
	
	
		
	//while (!(motor_completed_reset[0] && motor_completed_reset[1])) {
	
	
	//while (!(motor_completed_reset[4])) {
	
	/*
	while (!(motor_completed_reset[0] && motor_completed_reset[1] && motor_completed_reset[2] 
		  && motor_completed_reset[3] && motor_completed_reset[4])) {
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
			delay_us(220); // delay needed for reset sequence, can go down to like 180 with stuttering but still works
		}
	}
	
	
	
	// getting input from user strings
	unsigned char temp_string[1] = {0};
	unsigned char string_to_display[6] = {' ', ' ', ' ', ' ', ' ', ' '};
	
	// main while loop that asks for word, waits until it is displayed, then repeats
	while(1) {
		uart_write(USART2, clear, CLEAR_SIZE);
		
		//ask for a word from user
		uart_write(USART2, ask_for_word, ASK_MESSAGE_SIZE);
		
		// loop that reads character, puts it in display string, then displays to terminal
		for (int i=0; i<6; i++) {
			uart_read(USART2, temp_string, 1); //read from terminal
			
			char input_char = temp_string[0];
			
			//bad input correction (need to add backspace/enter support later)
			//lowercase to uppercase
			if (input_char >= 'a' && input_char <= 'z') {
				input_char -= 32; // Convert to uppercase
			}
			
			//O to zero
			if (input_char == 'O') {
				input_char = '0'; //change any Os to 0s
			}
			
			//put edited character into display string or display blank for invalid
			if ((input_char >= '0' && input_char <= '9') || (input_char >= 'A' && input_char <= 'Z')) {
        		string_to_display[i] = input_char;
			} else {
				string_to_display[i] = ' '; //invalid characters get blank
			}
			
			uart_write(USART2, temp_string, 1); //write original terminal character to the terminal
		}
		
		//tell user the displaying is happening
		uart_write(USART2, clear, CLEAR_SIZE);
		uart_write(USART2, displaying, DISPLAYING_SIZE);
		
		//move to inputted string
		move_to_flap(string_to_display);
	}
	


	/*
	
	delay_ms(300);
	unsigned char next[6] = {' ', ' ', ' ', ' ', ' ', ' '};
	
	move_to_flap(next);
	
	
	delay_ms(500);
	
	next[0] = '2';
	next[1] = 'K';
	next[2] = 'W';
	next[3] = '8';
	next[4] = 'P';
	next[5] = 'D';

	move_to_flap(next);

	delay_ms(500);

	next[0] = '3';
	next[1] = 'H';
	next[2] = 'G';
	next[3] = '9';
	next[4] = 'V';
	next[5] = 'J';

	move_to_flap(next);

	delay_ms(500);

	next[0] = '6';
	next[1] = 'R';
	next[2] = 'B';
	next[3] = 'Z';
	next[4] = '4';
	next[5] = 'Y';

	move_to_flap(next);


	*/
	
	
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