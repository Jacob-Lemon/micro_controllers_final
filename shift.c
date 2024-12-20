#include "shift.h"
#include "gpio.h"
#include "delay.h"
#include "global_variables.h"


// setup the ports associated with the shift registers
void init_shift_registers() {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // enable GPIO clock B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; // enable GPIO clock C
	
	// shift register 0
	set_pin_mode(DATA_0_PORT, DATA_0_PIN, OUTPUT);
	set_pin_mode(SHCP_0_PORT, SHCP_0_PIN, OUTPUT);
	set_pin_mode(STCP_0_PORT, STCP_0_PIN, OUTPUT);
	
	// shift register 1
	set_pin_mode(DATA_1_PORT, DATA_1_PIN, OUTPUT);
	set_pin_mode(SHCP_1_PORT, SHCP_1_PIN, OUTPUT);
	set_pin_mode(STCP_1_PORT, STCP_1_PIN, OUTPUT);
	
	// shift register 2
	set_pin_mode(DATA_2_PORT, DATA_2_PIN, OUTPUT);
	set_pin_mode(SHCP_2_PORT, SHCP_2_PIN, OUTPUT);
	set_pin_mode(STCP_2_PORT, STCP_2_PIN, OUTPUT);
}


//takes in a motor id and steps it one time based on its current orientation
void register_step_motor_once(int motor_id) {  //do we need the step level? maybe for more precision?
	// increase motor_id orientation
	unsigned char data_to_register = 0;
	// write a switch statement
	switch (motor_id) {
		case 0: {
			// increase motor_id orientation
			motor_orientation_0 += 1;
			//motor_orientation reset behavior for array indexing
			if (motor_orientation_0 > 3) {
				motor_orientation_0 = 0;
			}
			// update the nibble
			motor_0_nibble = step_index[motor_orientation_0];
			//motor 1 is higher 4 bits, motor 0 is lower 4 bits for register 0
			data_to_register = (motor_1_nibble << 4) | motor_0_nibble;
			// delay_us(200);
			register_put_serial_data(0, data_to_register);
			// delay_us(200);
		} break;
		
		case 1: {
			// increase motor_id orientation
			motor_orientation_1 += 1;
			//motor_orientation reset behavior for array indexing
			if (motor_orientation_1 > 3) {
				motor_orientation_1 = 0;
			}
			// update the nibble
			motor_1_nibble = step_index[motor_orientation_1];
			//motor 1 is higher 4 bits, motor 0 is lower 4 bits for register 0
			data_to_register = (motor_1_nibble << 4) | motor_0_nibble;
			// delay_us(200);
			register_put_serial_data(0, data_to_register);
			// delay_us(200);
		} break;
		
		case 2: {
			// increase motor_id orientation
			motor_orientation_2 += 1;
			//motor_orientation reset behavior for array indexing
			if (motor_orientation_2 > 3) {
				motor_orientation_2 = 0;
			}
			// update the nibble
			motor_2_nibble = step_index[motor_orientation_2];
			//motor 3 is higher 4 bits, motor 2 is lower 4 bits for register 1
			data_to_register = (motor_3_nibble << 4) | motor_2_nibble;
			register_put_serial_data(1, data_to_register);
		} break;

		case 3: {
			// increase motor_id orientation
			motor_orientation_3 += 1;
			//motor_orientation reset behavior for array indexing
			if (motor_orientation_3 > 3) {
				motor_orientation_3 = 0;
			}
			// update the nibble
			motor_3_nibble = step_index[motor_orientation_3];
			//motor 3 is higher 4 bits, motor 2 is lower 4 bits for register 1
			data_to_register = (motor_3_nibble << 4) | motor_2_nibble;
			register_put_serial_data(1, data_to_register);
		} break;
		
		case 4: {
			// increase motor_id orientation
			motor_orientation_4 += 1;
			//motor_orientation reset behavior for array indexing
			if (motor_orientation_4 > 3) {
				motor_orientation_4 = 0;
			}
			// update the nibble
			motor_4_nibble = step_index[motor_orientation_4];
			//motor 5 is higher 4 bits, motor 4 is lower 4 bits for register 2
			data_to_register = (motor_5_nibble << 4) | motor_4_nibble;
			register_put_serial_data(2, data_to_register);
		} break;
		
		case 5: {
			// increase motor_id orientation
			motor_orientation_5 += 1;
			//motor_orientation reset behavior for array indexing
			if (motor_orientation_5 > 3) {
				motor_orientation_5 = 0;
			}
			// update the nibble
			motor_5_nibble = step_index[motor_orientation_5];
			//motor 5 is higher 4 bits, motor 4 is lower 4 bits for register 2
			data_to_register = (motor_5_nibble << 4) | motor_4_nibble;
			register_put_serial_data(2, data_to_register);
		} break;
	} // end switch
}



/******************************************************************************
register_put_serial_data will put put 8 bits to a single shift register in
serial.

pseudocode:
	loop 8 times (one per bit)
		shift the shift register
		output the serial data
	endloop
	enable output
	disable output // here, we disable immediately because we only need the positive edge of the clock
example call:
	// writes 00000000 to shift register 0
	register_put_serial_data(0,0b00000000);
quality assurance testing:
	we tested this very carefully and found not a single error. none. This works
	exactly as we want with no bugs. This is a perfect function. Programmers
	dream of writing functions as perfect as this.
******************************************************************************/
void register_put_serial_data(int register_id, unsigned char data) {
	for (int i=0; i<8; i++) {
		// get the current bit we want to serial output
		int serial_bit = (data >> (7 - i)) & 1;
		
		// assign serial output
		switch (register_id) {
			case 0: {
				// assign output
				digital_write(DATA_0_PORT, DATA_0_PIN, serial_bit); // write the data bit
				digital_write(SHCP_0_PORT, SHCP_0_PIN, 0); 					// shift clock goes low
				digital_write(SHCP_0_PORT, SHCP_0_PIN, 1); 					// shift clock goes high
			} break;
			
			case 1: {
				// assign output
				digital_write(DATA_1_PORT, DATA_1_PIN, serial_bit); // write the data bit
				digital_write(SHCP_1_PORT, SHCP_1_PIN, 0); 					// shift clock goes low
				digital_write(SHCP_1_PORT, SHCP_1_PIN, 1); 					// shift clock goes high
			} break;
			
			case 2: {
				// assign output
				digital_write(DATA_2_PORT, DATA_2_PIN, serial_bit); // write the data bit
				digital_write(SHCP_2_PORT, SHCP_2_PIN, 0); 					// shift clock goes low
				digital_write(SHCP_2_PORT, SHCP_2_PIN, 1); 					// shift clock goes high
			} break;
		} // end switch


	}
	// now, the shift register has the right data,
	// so we enable the storage clock to set the output
	
	// trigger positive edge of STCP
	switch (register_id) {
		case 0: {
			digital_write(STCP_0_PORT, STCP_0_PIN, 0); // clock goes low
			// delay_us(wait_us);
			digital_write(STCP_0_PORT, STCP_0_PIN, 1); // clock goes high
			// delay_us(wait_us);
		} break;
		case 1: {
			digital_write(STCP_1_PORT, STCP_1_PIN, 0); // clock goes low
			digital_write(STCP_1_PORT, STCP_1_PIN, 1); // clock goes high
		} break;
		case 2: {
			digital_write(STCP_2_PORT, STCP_2_PIN, 0); // clock goes low
			digital_write(STCP_2_PORT, STCP_2_PIN, 1); // clock goes high
		} break;
	} // end switch
}
