#include "motor.h"
#include "stm32l476xx.h"
#include "delay.h"
#include "gpio.h"


#define steps_multiplier 57 // I beleive 57 is Easton's calculated value

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
	
	for (int i=0; i < (steps); i++) {
		// increase step
		step_idx = (step_idx + 1) % 4;
		// assign output
		GPIOB->ODR &= step_array[step_idx];
		GPIOB->ODR |= step_array[step_idx]; // this should work? because it is PB4-7
		
		GPIOC->ODR &= (step_array[step_idx] >> 4);
		GPIOC->ODR |= (step_array[step_idx] >> 4); // this is hardcoded to output to GPIOC PC0-3
		
		
		// delay_ms(1);
		delay_us(420);
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
		
		
		GPIOC->ODR &= (step_array[step_idx] >> 4);
		GPIOC->ODR |= (step_array[step_idx] >> 4); // this is hardcoded to output to GPIOC PC0-3

		
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








