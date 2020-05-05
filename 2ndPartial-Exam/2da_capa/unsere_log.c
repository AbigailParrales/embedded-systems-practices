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
#include <libopencm3/stm32/usart.h>
#include "unsere_log.h"
#include "../1ra_capa/unsere_uart.h"
#include "../3ra_capa/stopwatch.h"

/***********************************************************************/

/*!< Log the char received */
void log_char(char ch) {
    usart_send_blocking(USART1, ch);
}

/*!< Log the ASCII character of the char received */
void log_ASCII(char ascii) {
    log_char(ascii + 48);
}

/*!< Log the ASCII form of a three digit number */
void log_3digs_number(int number) {
    char centenas = 0;
    char decenas = 0;
    char unidades = 0;

    int tmp_num = number;

    unidades = tmp_num%10;
    decenas = ((tmp_num%100) - unidades) / 10;
    centenas = ((tmp_num%1000)-((decenas*10)+unidades))/100;

    log_ASCII(centenas);
    log_ASCII(decenas);
    log_ASCII(unidades);
}

/*!< Logs a message */
void log_message(char* message) {
    while (*message != '\0') {
        usart_send_blocking(USART1, *message++);
    }
}

/*!< Logs the line jump and return to begining of the line */
void log_enter(void) {
    log_message("\r\n");
}

/*!< Display on terminal temperature message */
void log_timer_status(char state) {

    log_message("\r\nTimer: ");

    if(state == 0){ /*!< Reset */
        log_message("Reset.");
    }
    else if(state == 1){ /*!< Started */
        log_message("Started.");
    }
    else if(state == 2){ /*!< Stopped */
        log_message("Stopped");
        log_message(", time is: ");
        log_3digs_number(obtain_time());
        log_message(" seconds");
    }
    else if(state == 3){ /*!< Continued */
        log_message("Continued.");
    }

    log_message("\r\n");
}

/***********************************************************************/
