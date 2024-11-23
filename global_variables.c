#include "global_variables.h"




unsigned char current_flaps[6] = {' ', ' ', ' ', ' ', ' ', ' '}; //changed to be an array for better looking code




unsigned char step_index[] = {0x08, 0x04, 0x02, 0x01}; //switched from motor.c since 4-7 layout isn't needed
unsigned char motor_0_nibble = 0x08;
unsigned char motor_1_nibble = 0x08;
unsigned char motor_2_nibble = 0x08;
unsigned char motor_3_nibble = 0x08;
unsigned char motor_4_nibble = 0x08;
unsigned char motor_5_nibble = 0x08;


int motor_orientation_0 = 0;
int motor_orientation_1 = 0;
int motor_orientation_2 = 0;
int motor_orientation_3 = 0;
int motor_orientation_4 = 0;
int motor_orientation_5 = 0;





