#ifndef motor_h
#define motor_h
// function prototypes

void init_motor();

// void motor_next_flap(int flips); //maybe use?
int lookup_degree(char input);
int get_char_index(char flap_char);
int Z_is_passed(char current_char, char next_char);
int get_flap_distance(char current_flap, char next_flap);
void move_to_flap(unsigned char next_flaps[6]);
void motor_reset();
// void move_one_flap(int motor_id);




#endif