#ifndef interrupts_h
#define interrupts_h

// volatile int mode = 0;
// volatile int counts = 0;
extern int mode_int; // need to be declared in main.c, but outside of main
extern int counts;


void init_systick(int ticks);
// void init_exti();

void SysTick_Handler();
// void EXTI0_IRQHandler();
// void EXTI1_IRQHandler();
// void EXTI2_IRQHandler();

void increment_time(unsigned char current_time[6]);

#endif
