#include "hall.h"

#include "gpio.h"


void init_hall_effect_sensors() {
	// init the hall effect sensors 0-5
	// set_pin_mode(HALL_0_PORT, HALL_0_PIN, OUTPUT);
	set_pin_mode(HALL_1_PORT, HALL_1_PIN, INPUT);
	// set_pull_up_down(HALL_1_PORT, HALL_1_PIN, 1); // set to pull up
	
	
	// set_pin_mode(HALL_2_PORT, HALL_2_PIN, OUTPUT);
	// set_pin_mode(HALL_3_PORT, HALL_3_PIN, OUTPUT);
	// set_pin_mode(HALL_4_PORT, HALL_4_PIN, OUTPUT);
	// set_pin_mode(HALL_5_PORT, HALL_5_PIN, OUTPUT);
}

int get_hall_data(int hall_id) {
	switch (hall_id) {
		case 0: {
			return digital_read(HALL_0_PORT, HALL_0_PIN); // get data from hall effect sensor 0 then return it
		} break;
		case 1: {
			return digital_read(HALL_1_PORT, HALL_1_PIN);
		} break;
		case 2: {
			return digital_read(HALL_2_PORT, HALL_2_PIN);
		} break;
		case 3: {
			return digital_read(HALL_3_PORT, HALL_3_PIN);
		} break;
		case 4: {
			return digital_read(HALL_4_PORT, HALL_4_PIN);
		} break;
		case 5: {
			return digital_read(HALL_5_PORT, HALL_5_PIN);
		} break;
		
	} // end switch
	
	// else, there is some grave error, this should never happen
	return 0;
	
}