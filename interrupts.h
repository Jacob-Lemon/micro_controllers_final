#ifndef interrupts_h
#define interrupts_h

extern int mode_int; // need to be declared in main.c, but outside of main
extern int counts;


void init_systick(int ticks);
void SysTick_Handler();
void increment_time(unsigned char current_time[6]);

#endif
