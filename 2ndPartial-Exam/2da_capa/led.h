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
