#ifndef GPIO_H
#define GPIO_H

#include "stm32l476xx.h"

//----- Macros -----
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
void pin_mode(GPIO_TypeDef *port, int pin, int mode);
void set_output_type(GPIO_TypeDef *port, int pin, int type);
void set_pull_up_down(GPIO_TypeDef *port, int pin, int pupd);
void digital_write(GPIO_TypeDef *port, int pin, int value);
int digital_read(GPIO_TypeDef *port, int pin);

#endif // GPIO_H
