#include "motor.h"
#include "stm32l476xx.h"
#include "delay.h"
#include "gpio.h"
#include "shift.h"
#include "hall.h"

#include "global_variables.h"


#define steps_multiplier 57 // for moving 10 degrees
unsigned char step_array[] = {0x80, 0x40, 0x20, 0x10}; // normal way. original way!
const char flap_order[] = " 0123456789ABCDEFGHIJKLMNPQRSTUVWXYZ"; // order of flaps

void init_motor() {
	// Make sure GPIO clock B is enabled
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // enable GPIO clock B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; // enable GPIO clock C

	
	for (int pin=4; pin<=7; pin++) {
		set_pin_mode(GPIOB, pin, OUTPUT); // sets PB4-7 to output mode 
	}
	
	for (int pin=0; pin <=3; pin++) {
		set_pin_mode(GPIOC, pin, OUTPUT); // sets PC0-3 to output mode
	}
	
	// this should be sufficient to initialize the motor GPIO pins
}



void display_reset() {
	//code that resets all the motors to blank
	int reset_values[6] = {570, 510, 500, 600, 630, 540};
	int motor_reset_values[6] = {0};
	int magnet_detected[6] = {0};
	int motor_completed_reset[6] = {0};

	
	while (!(motor_completed_reset[0] && motor_completed_reset[1] && motor_completed_reset[2] 
		  && motor_completed_reset[3] && motor_completed_reset[4] && motor_completed_reset[5])) {	
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
}






// unsigned char steps[] = {0x0A, 0x09, 0x05, 0x06};
// dr phillips way
// A B A' B'
// 1010,
// 1001,
// 0101,
// 0110

/*
My way?

{A B A' B'} = PB4-7

so,
PB4 = A
PB5 = B
PB6 = A'
PB7 = B'


1000 - 0x08
0100 - 0x04
0010 - 0x02
0001 - 0x01

ODR gets
1000_0000 - 0x80
0100_0000 - 0x40
0010_0000 - 0x20
0001_0000 - 0x10

*/




//may not be necessary
/*
int lookupDegree(char input) {
	// Handle blank space
	if (input == ' ') {
		return 0;
	}

	// Handle numeric characters ('0' to '9')
	if (input >= '0' && input <= '9') {
		return (input - '0' + 1) * 10;
	}

	// Handle alphabetic characters ('A' to 'Z')
	if (input >= 'A' && input <= 'Z') {
		int baseDegrees = 110; // Starting degrees for 'A'

		// Skip 'O' between 'N' and 'P'
		if (input >= 'P') {
			return baseDegrees + (input - 'A' - 1) * 10;
		} else {
			return baseDegrees + (input - 'A') * 10;
		}
	}
}
*/



// need to protect against bad input?
// returns which flap number a given character is

int get_char_index(char flap_char) {
	// find the index of the character
	for (int i = 0; i < 36; i++) {
		if (flap_order[i] == flap_char) {
			return i;
		}
	}
	return 0;
}


// returns 1 if Z will be passed, and 0 if not
int Z_is_passed(char current_char, char next_char) {
	int current_index = get_char_index(current_char);
	int next_index = get_char_index(next_char);
	int ZIndex = 35;
	
	// Check if we pass through Z 
					// full normal pass through Z                      edge case
	if ((next_index < current_index && next_index < ZIndex) || (current_index == ZIndex)) {
		//assuming current == next means no flipping else change to next_index <= current_index
		return 1; // true
	}
	return 0; // false
}


//finds the number of flips needed to get between two specific flaps
int get_flap_distance(char current_flap, char next_flap) {
    // Get the indices of both characters
    int current_flap_index = get_char_index(current_flap);
    int next_flap_index = get_char_index(next_flap);
    
    // Calculate the distance
    int distance = next_flap_index - current_flap_index;
    if (distance < 0) { //add || current_flap_index == next_flap_index if we want to flip if cur == next
        distance += 36; // Wrap around to the start if negative
    }
    
    return distance;
}

//not sure we need this
// void move_one_flap(int motor_id) {
// 	for(int i = 0; i < steps_multiplier; i++) {
// 		register_step_motor_once(motor_id);
// 	}
// }


// function to allow motors to all turn together
void move_to_flap(unsigned char next_flaps[6]) {
	int flap_distance_in_steps[6] = {0}; //flap distance for each motor_id
	int Z_is_passed_flag = 0; //if step distance needs to be adjusted
	int biggest_flap_change = 0; //for keeping track of which motor has to rotate the most

    //update flap distance for each motor
    for (int motor_id = 0; motor_id < 6; motor_id++) {
        if (current_flaps[motor_id] != next_flaps[motor_id]) { // only update motors that need to rotate
            Z_is_passed_flag = Z_is_passed(current_flaps[motor_id], next_flaps[motor_id]);
            flap_distance_in_steps[motor_id] = get_flap_distance(current_flaps[motor_id], next_flaps[motor_id]) * 57;
            if (Z_is_passed_flag) {
                flap_distance_in_steps[motor_id] -= 4; //rotation correction
            }
        }

        // Update biggest flap distance so we don't loop more than needed when updating motors
        if (flap_distance_in_steps[motor_id] > biggest_flap_change) {
            biggest_flap_change = flap_distance_in_steps[motor_id];
        }
    } 
		
		
	// this loop will initialize which motors are all moving at once
	int motor_delay_value = 0;
	for (int motor_id = 0; motor_id < 6; motor_id++) {
			if (current_flaps[motor_id] != next_flaps[motor_id]) {
					motor_delay_value += 1;
			}
	}
	

    //for loop to do steps for all motors that need to move
    for (int i = 0; i < biggest_flap_change; i++) {   //max amount of motor rotations
        for (int motor_id = 0; motor_id < 6; motor_id++) {  //go through each motor
            if (i < flap_distance_in_steps[motor_id]) { //only do ones that have to rotate
				register_step_motor_once(motor_id); // move motors one right after another
				
				// change how many motors are moving
				if (i + 1 == flap_distance_in_steps[motor_id]) { // motor will stop after this step
						motor_delay_value -= 1; // adjust in software the number of motors moving
				}
            }
						
			// temp fix, it might matter which 3 are moving, but that's a problem for another time
			switch (motor_delay_value) {
			case 1:
				delay_us(305); //305 is consistent for 1 moving at the same time, 301 limit
				break;
			case 2:
				delay_us(295); //295 is consistent for 2 moving at the same time, 292 limit
				break;
			case 3:
				delay_us(275); //275 is consistent for 3 moving at the same time, 271 limit
				break;
			case 4:
				delay_us(250); //250 is consistent for 4 moving at the same time, 247 limit
				break;
			case 5:
				delay_us(240); //240 is consistent for 5 moving at the same time, 236 limit
				break;
			case 6:
				delay_us(230); //230 is consistent for 6 moving at the same time, 228 limit
				break;
			default:
				delay_us(1000); // in case things break it'll go real slow
				break;
			}
        }
    }
    
    //update current flaps 
    for (int motor_id = 0; motor_id < 6; motor_id++) {
        current_flaps[motor_id] = next_flaps[motor_id];
    }

}






/*
shift register reqs
power and ground
Q0-Q7 which are parallel outputs

6 total pins for everything
data 0
data 1
data 2




*/








