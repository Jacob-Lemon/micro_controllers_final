#include "stm32l476xx.h"

#include "uart.h"
#include "motor.h"
#include "delay.h"
#include "gpio.h"
#include "global_variables.h"
#include "shift.h"
#include "hall.h"
#include "interrupts.h"



int mode_int = 0;
int counts = 0;


int main(void) {

	// enable GPIO clocks
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enable GPIO clock A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // enable GPIO clock B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; // enable GPIO clock C


	// initializations
	delay_ms(20);

	init_uart();

	delay_ms(20);
		
	init_hall_effect_sensors();
	
	delay_ms(20);
	
	init_shift_registers();
	
	delay_ms(20);
	
	init_systick(160000);
	
	delay_ms(20);
	
	
	
	//UART Messages
	
	//for clearing terminal and going to top left
	unsigned char clear[] = "\033[2J ";
	clear[5] = 12;
	#define CLEAR_SIZE 7
	
	//clear the current terminal display
	uart_write(USART2, clear, CLEAR_SIZE);

	//message that asks user for a word
	#define ASK_MESSAGE_SIZE 47
	unsigned char ask_for_word[ASK_MESSAGE_SIZE] = "Enter a 6 letter or less word and press enter: ";
	
	// various messages
	#define DISPLAYING_SIZE 12
	unsigned char displaying[DISPLAYING_SIZE] = "Displaying: ";
	
	#define RESETTING_SIZE 12
	unsigned char resetting[RESETTING_SIZE] = "Resetting...";
	

	// welcome ui message
	#define WELCOME_SIZE 501
	unsigned char welcome_message[WELCOME_SIZE] = 
    "+---------------------------------+\n\r"
    "|                                 |\n\r"
    "|       SPLIT FLAP DISPLAY        |\n\r"
    "|                                 |\n\r"
    "+---------------------------------+\n\r"
    "\n\r"
    "Welcome to the Split Flap Display Interface!\n\r"
    "\n\r"
    "Please choose a mode:\n\r"
    "+---------------------------------+\n\r"
    "| [1] Text Enter Mode             |\n\r"
    "| [2] Clock Mode                  |\n\r"
    "| [3] Reset Split Flap Display    |\n\r"
    "| [4] Credits                     |\n\r"
    "+---------------------------------+\n\r"
    "\n\r"
    "Enter your choice: ";
	
	
	//credits message
	#define CREDITS_MESSAGE_SIZE 159
	unsigned char credits_message[CREDITS_MESSAGE_SIZE] = 
    "                    CREDITS\n\r\n"
    "    Created by Easton McBeth and Jacob Lemon\n\r"
    "   For ECE 3710 Microcontrollers Final Project\n\r"
    "              Completed Fall 2024\n\r";
	
	//info for the user
	#define ESCAPE_MESSAGE_SIZE 46
	unsigned char escape_message[ESCAPE_MESSAGE_SIZE] = 
    "Press the escape key to go back to the menu\n\r\n";
	
	//get clock format
	#define CURRENT_TIME_SIZE 147
	unsigned char current_time_message[CURRENT_TIME_SIZE] = 
		"Enter the current time in the format 'HHMMPM'\n\r"
		"For example if it's 12:34PM, enter '1234PM', or if it's 2:30AM enter '0230AM'\n\r\n"
		"Enter current time: ";
	
	//current time message
	#define DISPLAY_CURRENT_TIME_SIZE 24
	unsigned char display_current_time_message[DISPLAY_CURRENT_TIME_SIZE] = 
		"Current time displayed: ";
	






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
	
	//delay_ms(1700); //simulated for unconnected
	display_reset();
	
	
	
	
	
	//to store the users mode selection
	unsigned char mode_selection[1] = {0};
	

	// main while loop that switches between different display modes
	while(1) {


		// mode for showing the ui message
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
			
		}


		// mode for getting words from the user
		else if (mode_int == 1) {
			uart_write(USART2, clear, CLEAR_SIZE);
						
			//show escape message at top
			uart_write(USART2, escape_message, ESCAPE_MESSAGE_SIZE);
			
			//ask for a word from user
			uart_write(USART2, ask_for_word, ASK_MESSAGE_SIZE);
			
			//reset string to display array
			for (int i = 0; i < 6; i++) {
				string_to_display[i] = ' ';
			}
			//reset variables used 
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
					if (terminal_index <= 5) {
						string_to_display_IDX -= 1;
					}
					//string_to_display_IDX -= 1;
					if (string_to_display_IDX < 0) {
						string_to_display_IDX = 0;
					}
					
					string_to_display[string_to_display_IDX] = ' ';
					
					// prevent backspacing ui text
					if (terminal_index > 0) {
						uart_write(USART2, temp_string, 1); //write backspace character to the terminal
					}
					
					//keeping track of the location on the terminal
					terminal_index -= 1;
					if (terminal_index < 0) {
						terminal_index = 0;
					}					
					
				}
				// put letters and spaces into string_to_display
				else if ((temp_string[0] >= '0' && temp_string[0] <= '9') || 
								 (temp_string[0] >= 'A' && temp_string[0] <= 'Z') || 
								 (temp_string[0] >= 'a' && temp_string[0] <= 'z') || 
							   (temp_string[0] == ' ')) {
									 
					char input_char = temp_string[0];
				
					//lowercase to uppercase
					if (input_char >= 'a' && input_char <= 'z') {
						input_char -= 32; // Convert to uppercase
					}
					
					//O to zero
					if (input_char == 'O') {
						input_char = '0'; //change any Os to 0s
					}
					
					//put edited character into display string or display blank for invalid
					if (((input_char >= '0' && input_char <= '9') || (input_char >= 'A' && input_char <= 'Z') || (input_char == ' ')) && (terminal_index <= 5)) {
						string_to_display[string_to_display_IDX] = input_char;
						uart_write(USART2, temp_string, 1);
						terminal_index += 1;
						if (string_to_display_IDX != 5) {
							string_to_display_IDX += 1;
						}
					}
				}							
			}
			
			//don't move flaps if going back to welcome ui
			if (mode_int != 0) {
				//tell user the displaying is happening
				uart_write(USART2, clear, CLEAR_SIZE);
				uart_write(USART2, displaying, DISPLAYING_SIZE);
				uart_write(USART2, string_to_display, 6);
				
				//move to inputted string
				move_to_flap(string_to_display);
			}
		}



		// mode for getting clock mode ready
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
				
				// escape character goes back to welcome ui
				if (clock_entry[0] == 27) {
					mode_int = 0;
					break; //goes to while(1)
				}
				
				// handle backspacing (127 (DEL) is what putty recognizes backspace as)
				if (clock_entry[0] == 127) {
					
					//can only backspace if a number has been entered
					if (terminal_index > 0) {
						uart_write(USART2, clock_entry, 1); //write backspace character to the terminal
					}
					
					
					//don't let index go lower than needed, and keep terminal index and string index in sync
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
				else if (((clock_entry[0] == 'P' || clock_entry[0] == 'p') || (clock_entry[0] == 'A' || clock_entry[0] == 'a')) && clock_string_display_IDX == 4) {
					//lowercase to uppercase
					if (clock_entry[0] == 'p' || clock_entry[0] == 'a') {
						clock_entry[0] -= 32; // Convert to uppercase
					}
					clock_string_display[clock_string_display_IDX] = clock_entry[0];
					uart_write(USART2, clock_entry, 1); //show the typed number
					terminal_index += 1; //determine where in the UART terminal the user is
					clock_string_display_IDX += 1;
				}
				//sixth UART input logic
				else if ((clock_entry[0] == 'M' || clock_entry[0] == 'm') && clock_string_display_IDX == 5) {
					//lowercase to uppercase
					if (clock_entry[0] == 'm') {
						clock_entry[0] -= 32; // Convert to uppercase
					}
					clock_string_display[clock_string_display_IDX] = clock_entry[0];
					uart_write(USART2, clock_entry, 1); //show the typed number
					terminal_index += 1; //determine where in the UART terminal the user is
				}
				
				
			}
			
			//if mode_int is 0 then its going back to welcome ui
			if (mode_int != 0) {
				//tell user the displaying is happening
				uart_write(USART2, clear, CLEAR_SIZE);
				uart_write(USART2, displaying, DISPLAYING_SIZE);
				uart_write(USART2, clock_string_display, 6);
				
				move_to_flap(clock_string_display);
				
				//go to systick clock mode
				mode_int = 5;
			}
		}


		// mode for clock mode
		else if (mode_int == 5) {
			uart_write(USART2, clear, CLEAR_SIZE);
			
			//show escape message at top
			uart_write(USART2, escape_message, ESCAPE_MESSAGE_SIZE);
			
			//show time in uart display as well
			uart_write(USART2, display_current_time_message, DISPLAY_CURRENT_TIME_SIZE);
			
			//show curent time
			uart_write(USART2, current_flaps, 6); 
			
			temp_string[0] = 0;
			while (temp_string[0] != 27) {
				uart_read(USART2, temp_string, 1); //read from terminal
			}
			mode_int = 0;
		}


		// mode for resetting flaps to blank
		else if (mode_int == 3) {
			//clear screen for resetting
			uart_write(USART2, clear, CLEAR_SIZE);
			
			//display resetting while the flaps are initializing
			uart_write(USART2, resetting, RESETTING_SIZE);
			
			//delay_ms(1700); //uncomment when connected to hardware
			display_reset();
			
			mode_int = 0;
		}


		// mode for displaying simple credits
		else if (mode_int == 4) {
			//clear screen
			uart_write(USART2, clear, CLEAR_SIZE);
			
			//show escape message at top
			uart_write(USART2, escape_message, ESCAPE_MESSAGE_SIZE);
			
			//show credits
			uart_write(USART2, credits_message, CREDITS_MESSAGE_SIZE);
			
			unsigned char easton[] = "EAST0N";
			unsigned char and_message[] = "AND   ";
			unsigned char jacob[] = "JAC0B ";
			
			move_to_flap(easton);
			delay_ms(1000);
			move_to_flap(and_message);
			delay_ms(1000);
			move_to_flap(jacob);
			
			temp_string[0] = 0;
			while (temp_string[0] != 27) {
				uart_read(USART2, temp_string, 1); //read from terminal
			}
			mode_int = 0;
		}

		
		// if mode_int gets off somehow
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