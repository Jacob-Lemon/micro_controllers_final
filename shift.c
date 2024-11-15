#include "shift.h"







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
	register_put_serial_data(,)
******************************************************************************/
void register_put_serial_data(int register_id, char data) {
	for (int i=0; i<8; i++) {
		
		// assign serial output
		switch (register_id) {
			case 0: {
				// assign output
				
			} break;
			
			case 1: {
				// assign output
			} break;
			
			case 2: {
				// assign output
			} break;
		}
		
		// pulse clock
		
		
		
		
		
		// shift the register by pulsing the clock
		
		// output the serial data
	}
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








