/************************************************************************
* Copyright 2020 ITESM                                                  *
*                                                                       *
*                                                                       *
* Second Partial Exam                                                   *
*                                                                       *
* Autorhs:                                                              *
* Jesús Enrique Luna Medina          A01632334                          *
* Daniela abigail Parrales Mejía     A01228629                          *
* Luis Cortés Leal                   A01631163                          *
* Victor Joseph Guerrero             A01375994                          *
*                                                                       *
* Mayo  2020                                                            *
* This program is a cronometer which includes three buttons             *
* in order to control the functions of: Reset, Start and Stop           *
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
