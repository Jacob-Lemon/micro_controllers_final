#include "gpio.h"

#include "stm32l476xx.h"


#define OUTPUT 1
#define INPUT 0

#define PUSHPULL 0
#define OPENDRAIN 1

#define NONE 0
#define PULLUP 1
#define PULLDOWN 2

void pinMode(GPIO_TypeDef *port, int pin, int mode) {
    //port->MODER = ...................
}

void setOutputType(GPIO_TypeDef *port, int pin, int type) {
    //port->OTYPER = ...................
}

void setPullUpDown(GPIO_TypeDef *port, int pin, int pupd) {
    //port->PUPDR = ...................
}

void digitalWrite(GPIO_TypeDef *port, int pin, int value) {
    //port->ODR = ...................
}

int digitalRead(GPIO_TypeDef *port, int pin) {
    //return ...................
}
/*
int main() {
    pinMode(GPIOA, 5, OUTPUT);
    digitalWrite(GPIOA, 5, 1);
    setOutputType(GPIOA, 5, OPENDRAIN);
    pinMode(GPIOB, 3, INPUT);
    setPullUpDown(GPIOB, 3, PULLUP)
    int value = digitalRead(GPIOB, 3);
}
*/

