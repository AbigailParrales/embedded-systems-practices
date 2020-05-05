
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
