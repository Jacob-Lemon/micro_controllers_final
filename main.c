#include "stm32l476xx.h"

#include "uart.h"
#include "motor.h"
#include "delay.h"
#include "gpio.h"
#include "global_variables.h"
#include "shift.h"
#include "hall.h"
#include "interrupts.h"

/******************************************************************************
main.c
on register ids:
	an int. 0,1,2

test

******************************************************************************/

int mode_int = 0;
int counts = 0;


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
	

	// welcome ui message
	#define WELCOME_SIZE 606
	unsigned char welcome_message[] = 
    "+------------------------------------------------+\n\r"
    "|                                                |\n\r"
    "|              SPLIT FLAP DISPLAY                |\n\r"
    "|                                                |\n\r"
    "+------------------------------------------------+\n\r"
    "\n\r"
    "Welcome to the Split Flap Display Interface!\n\r"
    "\n\r"
    "Please choose a mode:\n\r"
    "+--------------------------------------+\n\r"
    "| [1] Text Enter Mode                  |\n\r"
    "| [2] Clock Mode                       |\n\r"
    "| [3] Reset Split Flap Display         |\n\r"
    "| [4] Credits                          |\n\r"
    "+--------------------------------------+\n\r"
    "\n\r"
    "Enter your choice: ";
	
	
	/*
	//credits
	#define CREDITS_MESSAGE_SIZE 612
	unsigned char credits_message[] = 
    "+----------------------------------------------------------------+\n\r"
    "|                                                                |\n\r"
    "|                           CREDITS                              |\n\r"
    "|                                                                |\n\r"
    "|           Created by Easton McBeth and Jacob Lemon             |\n\r"
    "|          For ECE 3710 Microcontrollers Final Project           |\n\r"
    "|                     Completed Fall 2024                        |\n\r"
    "|                                                                |\n\r"
    "+----------------------------------------------------------------+\n\r";
	*/
	
	#define CREDITS_MESSAGE_SIZE 2
	unsigned char credits_message[] = "hi";
	
	//info for the user
	#define ESCAPE_MESSAGE_SIZE 58
	unsigned char escape_message[] = 
    "Press the escape key at any time to go back to the menu\n\r\n";
	
	//get clock format
	#define CURRENT_TIME_SIZE 139
	unsigned char current_time_message[] = 
		"Enter the current time in the format 'HHMMPM', for example if it is currently 12:34PM, enter '1234PM', or if it is 2:30AM enter '0230AM'\n\r\n";

	
	// getting input from user strings
	unsigned char temp_string[1] = {0};
	unsigned char string_to_display[6] = {' ', ' ', ' ', ' ', ' ', ' '};
	int string_to_display_IDX = 0;
	
	//to fix error of first 5 characters concatenated with last character in terminal
	int terminal_index = 0;
	

	//clear screen for resetting
	uart_write(USART2, clear, CLEAR_SIZE);
	
	
	//display resetting while the flaps are initializing
	uart_write(USART2, resetting, RESETTING_SIZE);
	
	
	delay_ms(1700); //simulated for unconnected
	//display_reset();
	
	
	
	
	
	unsigned char mode_selection[1] = {0};
	

	// main while loop that asks for word, waits until it is displayed, then repeats
	while(1) {
		if (mode_int == 0) {
			//clear screen for showing ui
			uart_write(USART2, clear, CLEAR_SIZE);
			
			//show welcome message
			uart_write(USART2, welcome_message, WELCOME_SIZE);
			
			//loop to handle user selection input and various bad user input possibilities
			temp_string[0] = 0; //reset for use in any state
			terminal_index = 0;
			mode_selection[0] = 0;
			while(mode_selection[0] != '\r') {
				uart_read(USART2, mode_selection, 1); //read from terminal
				
				// handle backspacing (127 (DEL) is what putty recognizes backspace as)
				if (mode_selection[0] == 127) {
					
					//can only backspace if a number has been entered
					if (terminal_index > 0) {
						uart_write(USART2, mode_selection, 1); //write backspace character to the terminal
						mode_int = 0;
					}
					
					//keeping don't let index flag go lower than needed
					terminal_index -= 1;
					if (terminal_index < 0) {
						terminal_index = 0;
					}
				}
				
				//must be specific number input                     prevents multiple number input
				if (mode_selection[0] >= '1' && mode_selection[0] <= '4' && terminal_index == 0) {
					mode_int = mode_selection[0] - '0'; //convert char number to int
					
					uart_write(USART2, mode_selection, 1); //show the typed number
					terminal_index += 1; //can only backspace when terminal index == 1
				}
				
				//don't rewrite ui screen if mode hasn't been selected properly (ex: mode_int still == 0)
				if ((mode_selection[0] == '\r') && (mode_int == 0)) {
					mode_selection[0] = 0;
				}
					
				
			}
			
			//delay_ms(1300);
		}
		else if (mode_int == 1) {
			uart_write(USART2, clear, CLEAR_SIZE);
						
			//show escape message at top
			uart_write(USART2, escape_message, ESCAPE_MESSAGE_SIZE);
			
			//ask for a word from user
			uart_write(USART2, ask_for_word, ASK_MESSAGE_SIZE);
			
			string_to_display_IDX = 0;
			terminal_index = 0;
			temp_string[0] = 0;
			while(temp_string[0] != '\r') {
				uart_read(USART2, temp_string, 1); //read from terminal
				
				// escape character goes back to welcome ui
				if (temp_string[0] == 27) {
					mode_int = 0;
					break; //goes to while(1)
				}
				
				// handle backspacing (127 (DEL) is what putty recognizes backspace as)
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
		else if (mode_int == 2) {
			uart_write(USART2, clear, CLEAR_SIZE);
			
			//show escape message at top
			uart_write(USART2, escape_message, ESCAPE_MESSAGE_SIZE);
			
			// instructions for clock entry
			uart_write(USART2, current_time_message, CURRENT_TIME_SIZE);
			
			//loop to handle user selection input and various bad user input possibilities
			unsigned char clock_string_display[6] = {'1', '2', '3', '4', 'P', 'M'};
			int clock_string_display_IDX = 0;
			terminal_index = 0;
			unsigned char clock_entry[1] = {0};
			while(!(clock_entry[0] == '\r' && clock_string_display_IDX == 5)) {
				uart_read(USART2, clock_entry, 1); //read from terminal
				
				// handle backspacing (127 (DEL) is what putty recognizes backspace as)
				if (clock_entry[0] == 127) {
					
					//can only backspace if a number has been entered
					if (terminal_index > 0) {
						uart_write(USART2, clock_entry, 1); //write backspace character to the terminal
					}
					
					
					//don't let index go lower than needed
					if (terminal_index <= 5) {
						clock_string_display_IDX -= 1;
					}
					if (clock_string_display_IDX < 0) {
						clock_string_display_IDX = 0;
					}
					
					//don't let index go lower than needed
					terminal_index -= 1;
					if (terminal_index < 0) {
						terminal_index = 0;
					}
				}
				
				//must be specific number input                            first UART input logic
				if (clock_entry[0] >= '0' && clock_entry[0] <= '1' && clock_string_display_IDX == 0) {
					clock_string_display[clock_string_display_IDX] = clock_entry[0];
					uart_write(USART2, clock_entry, 1); //show the typed number
					terminal_index += 1; //determine where in the UART terminal the user is
					clock_string_display_IDX += 1;
				}
				//second UART input logic
				else if (clock_string_display_IDX == 1) {
					//if first one is a 0 then can enter 1 - 9 per clock logic
					if (clock_entry[0] >= '1' && clock_entry[0] <= '9' && clock_string_display[0] == '0') {
						clock_string_display[clock_string_display_IDX] = clock_entry[0];
						uart_write(USART2, clock_entry, 1); //show the typed number
						terminal_index += 1; //determine where in the UART terminal the user is
						clock_string_display_IDX += 1;
					}
					//if first one is a 1 then can enter 0, 1, or 2 per clock logic
					else if (clock_entry[0] >= '0' && clock_entry[0] <= '2' && clock_string_display[0] == '1') {
						clock_string_display[clock_string_display_IDX] = clock_entry[0];
						uart_write(USART2, clock_entry, 1); //show the typed number
						terminal_index += 1; //determine where in the UART terminal the user is
						clock_string_display_IDX += 1;
					}
				}
				//third UART input logic
				else if (clock_entry[0] >= '0' && clock_entry[0] <= '5' && clock_string_display_IDX == 2) {
					clock_string_display[clock_string_display_IDX] = clock_entry[0];
					uart_write(USART2, clock_entry, 1); //show the typed number
					terminal_index += 1; //determine where in the UART terminal the user is
					clock_string_display_IDX += 1;
				}
				//fourth UART input logic
				else if (clock_entry[0] >= '0' && clock_entry[0] <= '9' && clock_string_display_IDX == 3) {
					clock_string_display[clock_string_display_IDX] = clock_entry[0];
					uart_write(USART2, clock_entry, 1); //show the typed number
					terminal_index += 1; //determine where in the UART terminal the user is
					clock_string_display_IDX += 1;
				}
				//fifth UART input logic
				else if ((clock_entry[0] == 'P' || clock_entry[0] == 'A') && clock_string_display_IDX == 4) {
					clock_string_display[clock_string_display_IDX] = clock_entry[0];
					uart_write(USART2, clock_entry, 1); //show the typed number
					terminal_index += 1; //determine where in the UART terminal the user is
					clock_string_display_IDX += 1;
				}
				//sixth UART input logic
				else if (clock_entry[0] == 'M' && clock_string_display_IDX == 5) {
					clock_string_display[clock_string_display_IDX] = clock_entry[0];
					uart_write(USART2, clock_entry, 1); //show the typed number
					terminal_index += 1; //determine where in the UART terminal the user is
				}
				
				
			}
			
			//tell user the displaying is happening
			uart_write(USART2, clear, CLEAR_SIZE);
			uart_write(USART2, displaying, DISPLAYING_SIZE);
			uart_write(USART2, clock_string_display, 6);
			
			move_to_flap(clock_string_display);
			
			//go to systick clock mode
			mode_int = 5;
		}
		else if (mode_int == 5) {
			uart_write(USART2, clear, CLEAR_SIZE);
			
			//show escape message at top
			uart_write(USART2, escape_message, ESCAPE_MESSAGE_SIZE);
			
			unsigned char clock[5] = {'c', 'l', 'o', 'c', 'k'};
			uart_write(USART2, clock, 5);
			
			temp_string[0] = 0;
			while (temp_string[0] != 27) {
				uart_read(USART2, temp_string, 1); //read from terminal
			}
			mode_int = 0;
		}
		else if (mode_int == 3) {
			//clear screen for resetting
			uart_write(USART2, clear, CLEAR_SIZE);
			
			//display resetting while the flaps are initializing
			uart_write(USART2, resetting, RESETTING_SIZE);
			
			delay_ms(1700); //uncomment when connected to hardware
			//display_reset();
			
			mode_int = 0;
		}
		else if (mode_int == 4) {
			//clear screen
			uart_write(USART2, clear, CLEAR_SIZE);
			
			//show escape message at top
			uart_write(USART2, escape_message, ESCAPE_MESSAGE_SIZE);
			
			//show credits
			uart_write(USART2, credits_message, CREDITS_MESSAGE_SIZE);
			
			temp_string[0] = 0;
			while (temp_string[0] != 27) {
				uart_read(USART2, temp_string, 1); //read from terminal
			}
			mode_int = 0;
		}
		else {
			uart_write(USART2, clear, CLEAR_SIZE);
			
			unsigned char invalid_input[24] = {"Invalid input, try again"};
			uart_write(USART2, invalid_input, 24);
			mode_int = 0;
			
			//have it show for long enough
			delay_ms(1700);
		}
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