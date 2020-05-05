
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

/*!< LIBRARIES */
#include <libopencm3/stm32/rcc.h>

#include "unsere_rcc.h"

/***********************************************************************/

/*!< All the instances are configurted */
void setting_clock(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz(); 
}

void rcc_setups_for_gpio(void) {
    //RCC setups for needed gpios
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
