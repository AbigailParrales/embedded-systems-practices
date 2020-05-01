/* A stm32f103 library for user applications
 * Warren W. Gay VE3WWG
 * Sat Nov 25 11:53:53 2017
 */
#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>

enum I2C_RW {
    Read = 1,
    Write = 0
};

void i2c_setup(void);
void i2c_start_addr(uint8_t addr, enum I2C_RW rw);
void i2c_write_restart(uint8_t byte, uint8_t addr);
uint8_t i2c_read(bool lastf);
void i2c_stop(void);
void i2c_write(uint8_t byte);
void i2c_write_8bits(uint8_t addr, uint8_t byte);

#endif// I2C_H

// End i2c.h
