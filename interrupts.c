#include "interrupts.h"

#include "stm32l476xx.h"
#include "delay.h"
#include "motor.h"
#include "global_variables.h"


#define SysTick_CTRL_CLKSOURCE 0x00000007
//#define SysTick_CTRL_TICKINT 0x00000007
//#define SysTick_CTRL_ENABLE 1


void init_systick(int ticks) {
	SysTick->CTRL = 0; 					// disable systick
	SysTick->LOAD = ticks - 1; 	// Set reload register
	
	
	// Set interrupt priority of SysTick to least urgency (i.e., largest priority value)
	//-NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	SysTick->VAL = 0; // Reset the SysTick counter value
	// Select processor clock: 1 = processor clock; 0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;
	//SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;
	
	// Enables SysTick interrupt, 1 = Enable, 0 = Disable
	//SysTick->CTRL |= SysTick_CTRL_TICKINT;
	// Enable SysTick
	//SysTick->CTRL |= SysTick_CTRL_ENABLE;
}


/******************************************************************************
systick interrupt pseudocode

if (in counting mode)
	update the timer counter
	display count to the LCD
endif

******************************************************************************/

//volatile int mode = 0;
//volatile int counts = 0;
/*
mode:
1=counting
0=paused
*/

/*
mode:
0 = text display from uart
1 = clock mode

*/



void SysTick_Handler() {
	if (mode == 1) {
		// we are in clock mode, and count for 60 seconds
		if (counts > 60) {
			// do the action and reset counts
			counts = 0; // reset counts
			
			// time_array = current_flaps
			unsigned char time_array[6] = {0};
			for (int i=0; i<6; i++) {
				time_array[i] = current_flaps[i];
			}
			
			// time_array++;
			// time array should look something like 1248PM
			increment_time(time_array);
			
			
		}
		else {
			counts++;
		}
	}
	else if (mode == 0) {
		// we are in text display mode, we do not need interrupts in this mode
		counts = 0; // just to reset it
	}
	
	
	
}



void increment_time(unsigned char current_time[6]) {
	// increment minutes
	current_time[3]++;
	// handle carry for minutes ones place
	if (current_time[3] > '9') {
		current_time[3] = '0';
		current_time[2]++;
	}

	// handle carry for minutes tens place
	if (current_time[2] > '5') {
		current_time[2] = '0';
		// current_time[1]++; // increment hours 1's place
		// handle carry for hours, need to handle
		// 09 -> 10
		// 12 -> 01
		if (current_time[0] == '0' && current_time[1] == '9') {
			// 09 -> 10
			current_time[0] = '1';
			current_time[1] = '0';
		}
		else if (current_time[0] == '1' && current_time[1] == '2') {
			// 12 -> 01
			current_time[0] = '0';
			current_time[1] = '1';
		}
		else {
			// we aren't in a special case, we just increment hours ones place
			// before incrementing, we see if we are in hour 11. 11:59pm -> 12:00am
			// this is the transition to and from am and pm
			if (current_time[0] == '1' && current_time[1] == '1') {
				// if in pm go to am
				if (current_time[4] == 'P') {
					current_time[4] = 'A';
				}
				// if in am go to pm
				else if (current_time[4] == 'A') {
					current_time[4] = 'P';
				}
			}
			// then increment hours
			current_time[1]++;
		}
	}
}











