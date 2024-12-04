#ifndef interrupts_h
#define interrupts_h

// volatile int mode = 0;
// volatile int counts = 0;
extern volatile int mode;
extern volatile int counts;


void init_systick(int ticks);
void init_exti();

void SysTick_Handler();
void EXTI0_IRQHandler();
void EXTI1_IRQHandler();
void EXTI2_IRQHandler();



#endif
