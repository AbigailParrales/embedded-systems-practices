
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
#include "../1ra_capa/unsere_gpio.h"
#include "../1ra_capa/unsere_uart.h"
#include "../1ra_capa/unsere_rcc.h"
#include "../2da_capa/button.h"
#include "stopwatch.h"

/***********************************************************************/

int time = 0;
char status = 0;

/************************************************************************
* Status is a flag that shows the timer status                          *
* Status = 0; Reset                                                     *
* Status = 1; Started                                                   *
* Status = 2; Stopped                                                   *
* Status = 3; Continued                                                 *
*                                                                       *
************************************************************************/

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

/*/!< State machine */
void run_timer(void) {
    if ((obtain_status() == 0) && BUTTON_START_READ()){ //Start
        /*!< Running */
        /*!<logear status */
        /*!< poner Led a parpadear */
        status = 1;
    }
    else if (((obtain_status() == 1)||(obtain_status() == 3)) && BUTTON_STOP_READ()){ /*!< Stop */
        /*!< logear status */
        status = 2;
    }
    else if (((obtain_status() == 1)||(obtain_status() == 2)||(obtain_status() == 3)) && BUTTON_RESET_READ()){ /*!< Reset */
        status = 0;
        time = 0;
        /*!< Running */
        /*!< logear status */
        /*!< poner Led a parpadear */
        status = 1;
    }
    else if ((obtain_status() == 2) && BUTTON_START_READ()){ /*!< Continued */
        /*!< Running */
        /*!< logear status */
        /*!< poner Led a parpadear */
        status = 3;
    }
    else if((obtain_status() == 1)||(obtain_status() == 3)){ /*!< Running */
        /*!< Running */
        /*!< logear status */
        /*!< poner Led a parpadear */
    }
    
}

/***********************************************************************/
