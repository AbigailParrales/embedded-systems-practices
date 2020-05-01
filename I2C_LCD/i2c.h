/* A stm32f103 library for user applications
 * Warren W. Gay VE3WWG
 * Sat Nov 25 11:53:53 2017
 */
#ifndef I2C_H
#define I2C_H

#include <libopencm3/stm32/i2c.h>

void i2c_configure(uint32_t i2c);
void i2c_start_addr(uint32_t i2c,uint8_t addr);
void i2c_write(uint32_t i2c,uint8_t byte);
void i2c_stop(uint32_t i2c);
void i2c_wait_busy(uint32_t i2c);

#endif // I2C_H

// End i2c.h
