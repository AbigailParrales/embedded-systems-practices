
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
#include <libopencm3/stm32/rcc.h>

#include "unsere_rcc.h"

/***********************************************************************/

/*!< All the instances are configurted */
void setting_clock(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz(); 
}

void rcc_setups_for_gpio(void) {
    /*!< RCC setups for needed gpios */
}

void rcc_setups_for_uart(void) {
    
    /*!< To configure a timer it is enabled for uart to be used */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART1);
}

void rcc_setup(void) {
    setting_clock();
    rcc_setups_for_gpio();
    rcc_setups_for_uart();
}

/***********************************************************************/
