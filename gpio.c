#include "gpio.h"
#include "stm32l476xx.h"


/******************************************************************************
set_pin_mode will set the mode for a GPIO pin, like PA12.
inputs:
    pointer to a GPIO typedef
    int - pin number
    int - mode (0: Input, 1: Output, 2: Alternate Function, 3: Analog)
outputs:
    none
example call:
    pin_mode(GPIOA, 5, 1); // Configures PA5 as output mode
		pin_mode(GPIOA, 5, OUTPUT); // output is defined as a macro to be 1
******************************************************************************/
void set_pin_mode(GPIO_TypeDef *port, int pin, int mode) {
    port->MODER &= ~(0b11 << (pin * 2)); // Clear the 2 bits for the pin
    port->MODER |= (mode << (pin * 2));  // Set the mode for the pin
}


/******************************************************************************
set_output_type sets the output type for a GPIO pin.
inputs:
    pointer to a GPIO typedef
    int - pin number
    int - type (0: Push-Pull, 1: Open-Drain)
outputs:
    none
example call:
    set_output_type(GPIOA, 5, 0); // Configures PA5 as Push-Pull output
******************************************************************************/
void set_output_type(GPIO_TypeDef *port, int pin, int type) {
    port->OTYPER &= ~(1 << pin);  // Clear the bit for the pin
    port->OTYPER |= (type << pin); // Set the type for the pin
}


/******************************************************************************
set_pull_up_down sets the pull-up or pull-down resistor for a GPIO pin.
inputs:
    pointer to a GPIO typedef
    int - pin number
    int - pupd (0: No Pull-Up/Pull-Down, 1: Pull-Up, 2: Pull-Down)
outputs:
    none
example call:
    set_pull_up_down(GPIOB, 3, 1); // Configures PB3 with Pull-Up resistor
******************************************************************************/
void set_pull_up_down(GPIO_TypeDef *port, int pin, int pupd) {
    port->PUPDR &= ~(0b11 << (pin * 2)); // Clear the 2 bits for the pin
    port->PUPDR |= (pupd << (pin * 2));  // Set the pull-up/pull-down configuration
}


/******************************************************************************
digital_write sets the output state of a GPIO pin.
inputs:
    pointer to a GPIO typedef
    int - pin number
    int - value (0: Low, 1: High)
outputs:
    none
example call:
    digital_write(GPIOA, 5, 1); // Sets PA5 output to high
******************************************************************************/
void digital_write(GPIO_TypeDef *port, int pin, int value) {
    if (value) {
        port->ODR |= (1 << pin); // Set the pin
    } else {
        port->ODR &= ~(1 << pin); // Clear the pin
    }
}


/******************************************************************************
digital_read reads the input state of a GPIO pin.
inputs:
    pointer to a GPIO typedef
    int - pin number
outputs:
    int - value (0: Low, 1: High)
example call:
    int value = digital_read(GPIOB, 3); // Reads the input state of PB3
******************************************************************************/
int digital_read(GPIO_TypeDef *port, int pin) {
    return (port->IDR & (1 << pin)) ? 1 : 0; // Read the pin state
}






