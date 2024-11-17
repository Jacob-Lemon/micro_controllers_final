#ifndef SHIFT_H
#define SHIFT_H


// function prototypes

void register_step_motor_once(int motor_id); // does one step for a specific motor

void register_move_motor_degrees(int motor_id, int degrees); // moves a motor a specified number of degrees

void register_put_serial_data(int register_id, unsigned char data); // takes in 8 bits and puts it on a specific register














#endif