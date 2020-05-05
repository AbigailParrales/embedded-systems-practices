
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
#include "unsere_gpio.h"

/***********************************************************************/

/**
* We are using Specifically UART1
*/
void gpio_setups_for_uart(void) {
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG,
    GPIO0);

    /**  
     *Enable UART TX (PA9) 
     * In order to use the pin A9 to transmit the information. 
     */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, 
		  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

    /**  
     * Enable UART RX (PA10) 
     * In order to use the pin A10 to read the information. 
     */
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN,
                  GPIO_USART1_RX);
}

void gpio_setups_for_LEDs(void) {
    /*!< GPIO conf for wanted LEDs */
}

void gpio_setups_for_BUTTONs(void) {
    /*!< GPIO conf for wanted BUTTONs */
}

/************************************************************************
* To configurate GPIO it has to be enabled the correct clock if a       *
* necessary clock is not enabled, the system will not work, must be     *
* careful working with these.                                           *
* The clock are related to power consumption, it is very important      *
* for the state off the battery                                         *
*                                                                       *
************************************************************************/

void gpio_setup(void) {
    gpio_setups_for_uart();
    gpio_setups_for_LEDs();
    gpio_setups_for_BUTTONs();
}
    
/***********************************************************************/
