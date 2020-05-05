  
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
#ifndef UNSERE_LOG_H
#define UNSERE_LOG_H

/************************************************************************/

void log_timer_status(char state);
void log_message(char* message);
void log_3digs_number(int number);
void log_ASCII(char ascii);
void log_enter(void);
void log_char(char ch);

/************************************************************************/

#endif

/************************************************************************/
