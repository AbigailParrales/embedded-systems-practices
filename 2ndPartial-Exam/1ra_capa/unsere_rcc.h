
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
#ifndef UNSERE_RCC_H  
#define UNSERE_RCC_H 

/***********************************************************************/

/*!< All the settings of the components for the system are included */
void setting_clock(void);
void rcc_setups_for_gpio(void);
void rcc_setups_for_uart(void);
void rcc_setup(void);

/***********************************************************************/

#endif

/***********************************************************************/
