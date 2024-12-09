#ifndef GPIO_H
#define GPIO_H

#include "stm32l476xx.h"

//---------- Macros for use with the GPIO pins ----------
// these should be up to data with the easy wiring
// DATA0 - PB12
#define DATA_0_PORT GPIOB
#define DATA_0_PIN  12

// STCP0 - PA11
#define STCP_0_PORT GPIOA
#define STCP_0_PIN  11

// SHCP0 - PA12
#define SHCP_0_PORT GPIOA
#define SHCP_0_PIN  12

// DATA1 - PB9
#define DATA_1_PORT GPIOB
#define DATA_1_PIN  9

// STCP1 - PB8
#define STCP_1_PORT GPIOB
#define STCP_1_PIN  8

// SHCP1 - PC9
#define SHCP_1_PORT GPIOC
#define SHCP_1_PIN  9

// DATA2 - PC5
#define DATA_2_PORT GPIOC
#define DATA_2_PIN  5

// STCP2 - PC6
#define STCP_2_PORT GPIOC
#define STCP_2_PIN  6

// SHCP2 - PC8
#define SHCP_2_PORT GPIOC
#define SHCP_2_PIN  8


//-----hall effect sensors-----
// hall 0 - PA10
#define HALL_0_PORT GPIOA
#define HALL_0_PIN  10

// hall 1 - PB3
#define HALL_1_PORT GPIOB
#define HALL_1_PIN  3

// hall 2 - PB5
#define HALL_2_PORT GPIOB
#define HALL_2_PIN  5

// hall 3 - PB4
#define HALL_3_PORT GPIOB
#define HALL_3_PIN  4

// hall 4 - PB10
#define HALL_4_PORT GPIOB
#define HALL_4_PIN  10

// hall 5 - PA8
#define HALL_5_PORT GPIOA
#define HALL_5_PIN  8

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
