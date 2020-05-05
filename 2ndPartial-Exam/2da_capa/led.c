
/************************************************************************
* Copyright 2020 ITESM                                                  *
*                                                                       *
*                                                                       *
* ADC_UART                                                              *
*                                                                       *
* Autorhs:                                                              *
* Jesús Enrique Luna Medina          A01632334                          *
* Daniela abigail Parrales Mejía     A01228629                          *
* Luis Cortés Leal                   A01631163                          *
*                                                                       *
* Abril 2020                                                            *
* The project titled ADC_UART is carried out with the purpose of        *
* implementing a temperature sensing system which acquires the          *
* values from the environment and delivers a response through           *
* hardware using LEDs and software displaying on a Terminal.            *
*                                                                       *
************************************************************************/

/*!< LIBRARIES */
#include <libopencm3/stm32/gpio.h>

#include "led.h"

/************************************************************************
* In order to get more structurated the way of turning on or off the    *
* LEDs, a succesfull way is to have organized with related names so     *
* the user find it more easy to understand                              *
*                                                                       *
************************************************************************/

void LED_ON(uint32_t gpioport, uint16_t gpios) {
    gpio_clear(gpioport, gpios);
}

void LED_OFF(uint32_t gpioport, uint16_t gpios) {
    gpio_set(gpioport, gpios);
}

void LED_TOGGLE(uint32_t gpioport, uint16_t gpios) {
    gpio_toggle(gpioport, gpios);
}


/************************************************************************/
