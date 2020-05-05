
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
#ifndef STOPWATCH_H
#define STOPWATCH_H

/***********************************************************************/

extern int time;
extern char status;

int obtain_time(void);
char obtain_status(void);

void stopwatch_init(void);
void run_timer(void);

/***********************************************************************/

#endif

/***********************************************************************/
