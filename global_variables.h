#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H




/*
state orientation of each motor (whether it is facing A, B, A', or B')
	motor_orientation_0
	...
	motor_orientation_5
*/
int motor_orientation_0 = 0;
int motor_orientation_1 = 0;
int motor_orientation_2 = 0;
int motor_orientation_3 = 0;
int motor_orientation_4 = 0;
int motor_orientation_5 = 0;

/*
what each motor is currently displaying
*/
unsigned char current_flap[6] = {' ', ' ', ' ', ' ', ' ', ' '}; //changed to be an array for better looking code



















#endif