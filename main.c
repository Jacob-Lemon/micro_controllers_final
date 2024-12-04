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
	
	// getting input from user strings
	unsigned char temp_string[1] = {0};
	unsigned char string_to_display[6] = {' ', ' ', ' ', ' ', ' ', ' '};
	int string_to_display_IDX = 0;
	
	//to fix error of first 5 characters concatenated with last character in terminal
	int terminal_index = 0;
	
	
	// reset the motors to blank
	


	//display resetting while the flaps are initializing
	uart_write(USART2, resetting, RESETTING_SIZE);
	
	display_reset();

	
	
	
	// main while loop that asks for word, waits until it is displayed, then repeats
	while(1) {
		uart_write(USART2, clear, CLEAR_SIZE);
		
		//ask for a word from user
		uart_write(USART2, ask_for_word, ASK_MESSAGE_SIZE);
		
		string_to_display_IDX = 0;
		terminal_index = 0;
		temp_string[0] = 0;
		while(temp_string[0] != '\r') {
			uart_read(USART2, temp_string, 1); //read from terminal
			
			// handle backspacing (127 or DEL is what putty recognizes backspace as)
			if (temp_string[0] == 127) {
				
				// keep index in array bounds
				string_to_display_IDX -= 1;
				if (string_to_display_IDX < 0) {
					string_to_display_IDX = 0;
				}
				
				//keeping track of the location on the terminal
				terminal_index -= 1;
				if (terminal_index < 0) {
					terminal_index = 0;
				}
				
				uart_write(USART2, temp_string, 1); //write backspace character to the terminal
				
			}
			// put letters and spaces into string_to_display
			else if ((temp_string[0] >= '0' && temp_string[0] <= '9') || (temp_string[0] >= 'A' && temp_string[0] <= 'Z') || (temp_string[0] >= 'a' && temp_string[0] <= 'z') || (temp_string[0] == ' ')) {
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
				if ((input_char >= '0' && input_char <= '9') || (input_char >= 'A' && input_char <= 'Z') || (input_char == ' ')) {
					if (terminal_index == string_to_display_IDX) { //keeps the first 6 characters in the terminal display
						string_to_display[string_to_display_IDX] = input_char;
					}
				}
				else {
					string_to_display[string_to_display_IDX] = ' ';
				}
				
				//don't reset terminal index
				terminal_index += 1;
				
				//increase string_to_display index and make sure it stays in bounds
				string_to_display_IDX += 1;
				if (string_to_display_IDX > 5) {
					string_to_display_IDX = 5;
				}
				
				uart_write(USART2, temp_string, 1); //write original terminal character to the terminal
			}
			
			// fill rest of array with spaces when the enter key is pressed
			else if (temp_string[0] == '\r') {
				for (int i = string_to_display_IDX; i < 6; i++) {
					if (string_to_display_IDX != 5) { // don't replace last character of array
						string_to_display[i] = ' ';
					}
				}
			}
			
		}
		
		//tell user the displaying is happening
		uart_write(USART2, clear, CLEAR_SIZE);
		uart_write(USART2, displaying, DISPLAYING_SIZE);
		uart_write(USART2, string_to_display, 6);
		
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