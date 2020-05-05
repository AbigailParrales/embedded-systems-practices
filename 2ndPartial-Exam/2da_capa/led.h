
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

/*!< Conditions to confirm if macro-identifier is defined */
#ifndef UNSERE_LED_H
#define UNSERE_LED_H
#include <stdint.h>

/***********************************************************************/

void LED_ON(uint32_t gpioport, uint16_t gpios);
void LED_OFF(uint32_t gpioport, uint16_t gpios);
void LED_TOGGLE(uint32_t gpioport, uint16_t gpios);

/***********************************************************************/

#endif

/***********************************************************************/
