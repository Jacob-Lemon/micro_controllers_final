#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H




/*
state orientation of each motor (whether it is facing A, B, A', or B')
	motor_orientation_0
	...
	motor_orientation_5
*/

/*
what each motor is currently displaying
*/
extern unsigned char current_flap[6]; //changed to be an array for better looking code










extern unsigned char step_index[]; //switched from motor.c since 4-7 layout isn't needed
extern unsigned char motor_0_nibble;
extern unsigned char motor_1_nibble;
extern unsigned char motor_2_nibble;
extern unsigned char motor_3_nibble;
extern unsigned char motor_4_nibble;
extern unsigned char motor_5_nibble;


extern int motor_orientation_0;
extern int motor_orientation_1;
extern int motor_orientation_2;
extern int motor_orientation_3;
extern int motor_orientation_4;
extern int motor_orientation_5;








#endif