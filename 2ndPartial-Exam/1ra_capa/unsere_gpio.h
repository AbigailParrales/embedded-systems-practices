
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
#ifndef UNSERE_GPIO_H  
#define UNSERE_GPIO_H 

/***********************************************************************/

void gpio_setups_for_BUTTONs(void);
void gpio_setups_for_uart(void);
void gpio_setups_for_LEDs(void);
void gpio_setup(void);

/***********************************************************************/

#endif

/***********************************************************************/
