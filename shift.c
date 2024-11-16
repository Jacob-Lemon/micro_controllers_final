#include "shift.h"
#include "gpio.h"





/******************************************************************************
register_step_motor will cause the motor of the motor_id to step once.


I am envisioning a nibble the corresponds to each motor. We then take that
nibble, shift just it, and concatenate it with the other thing.
This


******************************************************************************/
void register_step_motor(int motor_id) {
	
}

void register_move_motor_degrees(int motor_id, int degrees) {
	
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
******************************************************************************/
void register_put_serial_data(int register_id, unsigned char data) {
	for (int i=0; i<8; i++) {
		// get the current bit we want to serial output
		int serial_bit = (data >> (7 - i));
		
		
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
	} // end loop
	// now, the shift register has the right data,
	// so we enable the storage clock to set the output
	// trigger positive edge of STCP
	switch (register_id) {
		case 0: {
			digital_write(STCP_0_PORT, STCP_0_PIN, 0); // clock goes low
			digital_write(STCP_0_PORT, STCP_0_PIN, 1); // clock goes high
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


/*
single shift register specs
needs power and ground
inputs:
	serial data input
	shift register clock
	storage register clock
	output enable - doesn't need a pin, just goes straight to ground
	
outputs:
	8 parallel data bits
	
usage for our purposes
output enable will be held to a constant logic zero
	this will mean that output is updated and instantly pushed out upon positive edge of STCP (storage register clock)
	
0100_0100
0010_0010

*/








