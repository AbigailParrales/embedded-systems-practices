
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
#ifndef UNSERE_UART_H
#define UNSERE_UART_H
#include <stdint.h>

/**********************************************************************/


/**********************************************************************/

void uart_setup(void);
void uart_send_char(char ch);
void uart_send(char * txt);
void parse_message(char *message_expected, char char_received);
int is_number(char ch);
char* number_2_string(int num);

/**********************************************************************/

#endif

/**********************************************************************/
