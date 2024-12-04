#include "interrupts.h"
#include "stm32l476xx.h"
#include "delay.h"
#include "motor.h"

#define SysTick_CTRL_CLKSOURCE 0x00000007
//#define SysTick_CTRL_TICKINT 0x00000007
//#define SysTick_CTRL_ENABLE 1


void init_systick(int ticks) {
	SysTick->CTRL = 0; 					// disable systick
	SysTick->LOAD = ticks - 1; 	// Set reload register
	
	
	// Set interrupt priority of SysTick to least urgency (i.e., largest priority value)
	//-NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	SysTick->VAL = 0; // Reset the SysTick counter value
	// Select processor clock: 1 = processor clock; 0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;
	//SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;
	
	// Enables SysTick interrupt, 1 = Enable, 0 = Disable
	//SysTick->CTRL |= SysTick_CTRL_TICKINT;
	// Enable SysTick
	//SysTick->CTRL |= SysTick_CTRL_ENABLE;
}

/*
init exti
*/
void init_exti() {
	// enable GPIO clock
	// RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	// enable PB0-2 as inputs
	// GPIOB->MODER &= 0b11000000; // set PB0-2
	GPIOB->MODER &= 0xFFFFFFC0;
	GPIOB->MODER |= 0x00000000; // sets PB0-2 as inputs
	
	// set pullup resistors
	GPIOB->PUPDR &= 0xFFFFFFD5;
	GPIOB->PUPDR |= 0x00000015;
	
	//-----EXTI 0-----
	// enable interrupt
	NVIC_EnableIRQ(EXTI0_IRQn);
	// connect External line to GPI
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
	SYSCFG->EXTICR[0] |=  SYSCFG_EXTICR1_EXTI0_PB;
	// interrupt mask register
	EXTI->IMR1 |= EXTI_IMR1_IM0;
	//rising edge trigger selection
	EXTI->RTSR1 |= EXTI_RTSR1_RT0;
	
	//-----EXTI 1-----
	// enable interrupt
	NVIC_EnableIRQ(EXTI1_IRQn);
	// connect External line to GPI
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1;
	SYSCFG->EXTICR[0] |=  SYSCFG_EXTICR1_EXTI1_PB;
	// interrupt mask register
	EXTI->IMR1 |= EXTI_IMR1_IM1;
	//rising edge trigger selection
	EXTI->RTSR1 |= EXTI_RTSR1_RT1;
	
	//-----EXTI 2-----
	// enable interrupt
	NVIC_EnableIRQ(EXTI2_IRQn);
	// connect External line to GPI
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI2;
	SYSCFG->EXTICR[0] |=  SYSCFG_EXTICR1_EXTI2_PB;
	// interrupt mask register
	EXTI->IMR1 |= EXTI_IMR1_IM2;
	//rising edge trigger selection
	EXTI->RTSR1 |= EXTI_RTSR1_RT2;
	
}
/******************************************************************************
systick interrupt pseudocode

if (in counting mode)
	update the timer counter
	display count to the LCD
endif

******************************************************************************/

//volatile int mode = 0;
//volatile int counts = 0;
/*
mode:
1=counting
0=paused
*/

void SysTick_Handler() {
	if (mode) {
		if (counts > 0) {
			step_motor_counterclockwise(1);
			counts -= 1;
			// delay_ms(1);
		}
		else {
			mode = 0;
		}
	}
}
/******************************************************************************
exti interrupt pseudocode

if (green button is pressed)
	set the timer to counting mode
else if (yellow button is pressed)
	set the timer to paused mode
else if (red button is pressed)
	set timer to paused mode
	reset the timer value
	display zero time
endif

******************************************************************************/

// EXTI 0 is the green button being pressed

/* comment out the EXTI handlers
void EXTI0_IRQHandler() {
	delay_ms(1);
	if ((EXTI->PR1 & EXTI_PR1_PIF0) != 0) {
		// upon green button being pressed, make the state to counting mode
		state = 1; // 1 is counting mode
	
			
		// clear the interrupt
		EXTI->PR1 |= EXTI_PR1_PIF0;
	}
}

// EXTI 1 is the yellow button

void EXTI1_IRQHandler() {
	delay_ms(1);
	if ((EXTI->PR1 & EXTI_PR1_PIF1) != 0) {
		// upon the yellow button being pressed, make the state to paused mode
		state = 0; // 0 is paused mode
	
			
		// clear the interrupt
		EXTI->PR1 |= EXTI_PR1_PIF1;
	}
}

// EXTI 2 is the red button

void EXTI2_IRQHandler() {
	delay_ms(1);
	if ((EXTI->PR1 & EXTI_PR1_PIF2) != 0) {
		// upon the red button being pressed
		// leave the state
		// reset the time
		
		
		// clear the interrupt
		EXTI->PR1 |= EXTI_PR1_PIF2;
	}
}

*/





