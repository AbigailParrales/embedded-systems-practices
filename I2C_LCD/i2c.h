
/************************************************************************
* Copyright 2020 ITESM                                                  *
*                                                                       *
*                                                                       *
* LCD_I2C                                                               *
*                                                                       *
* Autorhs:                                                              *
* Jesús Enrique Luna Medina          A01632334                          *
* Daniela abigail Parrales Mejía     A01228629                          *
* Luis Cortés Leal                   A01631163                          *
*                                                                       *
* Abril 2020                                                            *
* The project titled LCD_I2C is carried out with the purpose of         *
* implementing a temperature sensing system which acquires the          *
* values from the environment and delivers a response through           *
* hardware using LEDs and software displaying on a Terminal.            *
*                                                                       *
************************************************************************/

/*!< Conditions to confirm if macro-identifier is defined */
#ifndef I2C_H
#define I2C_H
#include <stdint.h>
#include <stdbool.h>

/***********************************************************************/

enum I2C_RW{
    Read = 1,
    Write = 0
};

/***********************************************************************/

void i2c_setup(void);
void i2c_start_addr(uint8_t addr, enum I2C_RW rw);
void i2c_write_restart(uint8_t byte, uint8_t addr);
uint8_t i2c_read(bool lastf);
void i2c_stop(void);
void i2c_write(uint8_t byte);
void i2c_write_8bits(uint8_t addr, uint8_t byte);

/***********************************************************************/

#endif

/***********************************************************************/
