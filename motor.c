#include "motor.h"
#include "stm32l476xx.h"
#include "delay.h"
#include "gpio.h"


#define steps_multiplier 34 // used to be 34
// 45 makes it go slightly more than 5 seconds

void init_motor() {
	// Make sure GPIO clock B is enabled
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // enable GPIO clock B
	
	// for now, lets use Pins PB 4-7
	// set pins to output mode - 01 is output, 00 is input
	// 0101_0101 = 55
	
	// old way
	// GPIOB->MODER &= 0xFFFF55FF;
	// GPIOB->MODER |= 0x00005500;
	// I don't think we need to modify the output type or any PUPDR
	
	
	/* new way */
	// set_pin_mode(GPIOB, 4, OUTPUT);
	// set_pin_mode(GPIOB, 5, OUTPUT);
	// set_pin_mode(GPIOB, 6, OUTPUT);
	// set_pin_mode(GPIOB, 7, OUTPUT);
	
	for (int pin=4; pin<=7; pin++) {
		set_pin_mode(GPIOB, pin, OUTPUT); // sets PB4-7 to output mode 
	}
	
	// this should be sufficient to initialize the motor GPIO pins
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


unsigned char step_array[] = {0x80, 0x40, 0x20, 0x10}; // normal way. original way!

unsigned int step_idx = 0;

void step_motor_clockwise(int steps) {
	steps *= steps_multiplier; // 34 right now
	//double loops = steps*steps_multiplier;
	
	// step_idx = 0;
	
	for (int i=0; i < (steps + 34); i ++) {
		// increase step
		step_idx = (step_idx + 1) % 4;
		// assign output
		GPIOB->ODR &= step_array[step_idx];
		GPIOB->ODR |= step_array[step_idx]; // this should work? because it is PB4-7
		// delay_ms(1);
		delay_us(500);
	}
}



void step_motor_counterclockwise(int steps) {
	// scale steps up because of the gear ratio of the stepper motor
	steps *= steps_multiplier; // 34 right now
	// steps *= 35;
	
	// step_idx = 0;
	
	for (int i=0; i < (steps + 1); i ++) {
		// decrease step
		step_idx = (step_idx > 0)? step_idx-1 : 3;
		// assign output
		GPIOB->ODR &= step_array[step_idx];
		GPIOB->ODR |= step_array[step_idx]; // this should work? because it is PB4-7
		delay_ms(1);
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








