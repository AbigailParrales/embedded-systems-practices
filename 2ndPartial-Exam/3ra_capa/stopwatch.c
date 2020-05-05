
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
#include "../1ra_capa/unsere_gpio.h"
#include "../1ra_capa/unsere_uart.h"
#include "../1ra_capa/unsere_rcc.h"
#include "../2da_capa/button.h"
#include "stopwatch.h"

int time = 0;
char status = 0;
/**
Status is a flag that shows the timer status
Status = 0; Reset
Status = 1; Started
Status = 2; Stopped
Status = 3; Continued
*/

/***********************************************************************/

int obtain_time(void) {
    return time;
}

char obtain_status(void){
    return status;
}

void stopwatch_init(void) {
    
    /*!< All the set up are included in order to make the system works */
    rcc_setup();
    gpio_setup();
    uart_setup();
}

//State machine
void run_timer(void) {
    if ((obtain_status() == 0) && BUTTON_START_READ()){ //Start
        //Running
        //logear status
        //poner Led a parpadear
        status = 1;
    }
    else if (((obtain_status() == 1)||(obtain_status() == 3)) && BUTTON_STOP_READ()) { //Stop
        //logear status
        status = 2;
    }
    else if (((obtain_status() == 1)||(obtain_status() == 2)||(obtain_status() == 3)) && BUTTON_RESET_READ()) {     //Reset
        status = 0;
        time = 0;
        //Running
        //logear status
        //poner Led a parpadear
        status = 1;
    }
    else if ((obtain_status() == 2) && BUTTON_START_READ()) {     //Continued
        //Running
        //logear status
        //poner Led a parpadear
        status = 3;
    }
    else if((obtain_status() == 1)||(obtain_status() == 3)) {   // Running
        //Running
        //logear status
        //poner Led a parpadear
    }
    
}

/***********************************************************************/
