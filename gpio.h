#ifndef GPIO_H
#define GPIO_H

#include "stm32l476xx.h"
//---------- Macros for use with the GPIO pins ----------
// PB0 - DATA0
#define DATA_0_PORT GPIOC
#define DATA_0_PIN  0

// PB1 - SHCP0
#define SHCP_0_PORT GPIOC
#define SHCP_0_PIN  1

// PB2 - STCP0
#define STCP_0_PORT GPIOC
#define STCP_0_PIN  2

// PB3 - DATA1
#define DATA_1_PORT GPIOB
#define DATA_1_PIN  3

// PB4 - SHCP1
#define SHCP_1_PORT GPIOB
#define SHCP_1_PIN  4

// PB5 - STCP1
#define STCP_1_PORT GPIOB
#define STCP_1_PIN  5

// PB6 - DATA2
#define DATA_2_PORT GPIOB
#define DATA_2_PIN  6

// PB7 - SHCP2
#define SHCP_2_PORT GPIOB
#define SHCP_2_PIN  7

// PB8 - STCP2
#define STCP_2_PORT GPIOB
#define STCP_2_PIN  8

//----- end macros for use with GPIO pins



//----- Macros for use with the functions -----
// for use with pin_mode()
#define OUTPUT 1
#define INPUT 0

// for use with set_output_type()
#define PUSHPULL 0
#define OPENDRAIN 1

// for use with set_pull_up_down
#define NONE 0
#define PULLUP 1
#define PULLDOWN 2

//----- Function Prototypes -----
void set_pin_mode(GPIO_TypeDef *port, int pin, int mode);
void set_output_type(GPIO_TypeDef *port, int pin, int type);
void set_pull_up_down(GPIO_TypeDef *port, int pin, int pupd);
void digital_write(GPIO_TypeDef *port, int pin, int value);
int digital_read(GPIO_TypeDef *port, int pin);

#endif // GPIO_H
