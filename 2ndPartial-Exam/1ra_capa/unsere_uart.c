
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
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/adc.h>
#include "unsere_uart.h"
#include "../2da_capa/unsere_log.h"

/**********************************************************************/

/********************************************************************
* The creation of a message, will an specificed limit, the message  *
* lenght is configurated too.                                       *
*                                                                   *
* The configuration for UART in order to write and read the serial  *
* data.                                                             *
*                                                                   *
********************************************************************/

void uart_setup(void) {
    usart_set_baudrate(USART1, 9600); /*!< Baudrate configurated at 9600 */
    usart_set_databits(USART1, 8); /*!<  Data will be 8 bit long  */
    usart_set_stopbits(USART1, USART_STOPBITS_1); 
    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    usart_enable_rx_interrupt(USART1); /*!< Local enable of interruption */
    usart_enable(USART1);
}

/*!< Return if the ASCII is a number from 0 to 9. */
int is_number(char ch) {
    if((ch >= 48) && (ch<=57)){
        return 1;
    } else {
        return 0;
    }
}

char* number_2_string(int num) {
    int tmp_num = num;

    static char tmp_s[4];

    char centenas = 0;
    char decenas = 0;
    char unidades = 0;

    unidades = tmp_num%10;
    decenas = ((tmp_num%100) - unidades) / 10;
    centenas = ((tmp_num%1000)-((decenas*10)+unidades))/100;

    tmp_s[0] = centenas + 48;
    tmp_s[1] = decenas + 48;
    tmp_s[2] = unidades + 48;

    uart_send(tmp_s);

    return tmp_s;
}
