
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
#include "button.h"

/***********************************************************************/

/*!< Obtaining the status of Start Button */
char BUTTON_START_READ(void){
    /*!< Obtaining the status of Start Button */
    return 0;
}

/*!< Obtaining the status of Stop Button */
char BUTTON_STOP_READ(void) {
    /*!< Obtaining the status of Stop Button */
    return 0;
}

/*!< Obtaining the status of Reset Button */
char BUTTON_RESET_READ(void) {
    /*!< Obtaining the status of Reset Button */
    return 0;
}

/***********************************************************************/
