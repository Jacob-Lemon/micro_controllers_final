#ifndef motor_h
#define motor_h
// function prototypes


void display_reset();
int get_char_index(char flap_char);
int Z_is_passed(char current_char, char next_char);
int get_flap_distance(char current_flap, char next_flap);
void move_to_flap(unsigned char next_flaps[6]);


#endif