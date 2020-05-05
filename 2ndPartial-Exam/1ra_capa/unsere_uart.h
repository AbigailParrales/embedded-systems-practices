
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
#ifndef UNSERE_UART_H
#define UNSERE_UART_H
#include <stdint.h>

/**********************************************************************/

void uart_setup(void);
void uart_send_char(char ch);
void uart_send(char * txt);
void parse_message(char *message_expected, char char_received);
int is_number(char ch);

/**********************************************************************/

#endif

/**********************************************************************/
