#ifndef motor_h
#define motor_h
// function prototypes

void init_motor();

void step_motor_clockwise(int steps);
void step_motor_counterclockwise(int steps);
// void motor_next_flap(int flips); //maybe use?
int lookup_degree(char input);
int get_char_index(char flap_char);
int Z_is_passed(char current_char, char next_char);
int get_flap_distance(char current_flap, char next_flap);
void move_to_flap(int motors_needing_rotating[6], unsigned char next_flap[6]);
// void move_one_flap(int motor_id);




#endif