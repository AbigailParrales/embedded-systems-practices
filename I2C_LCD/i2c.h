/* A stm32f103 library for user applications
 * Warren W. Gay VE3WWG
 * Sat Nov 25 11:53:53 2017
 */
#ifndef I2C_H
#define I2C_H

#include <stdbool.h>
#include <setjmp.h>

#include <libopencm3/stm32/i2c.h>

enum I2C_RW {
	Read = 1,
	Write = 0
};

typedef struct {
	uint32_t	device;		// I2C device
	uint32_t	timeout;	// Ticks
} I2C_Control;

void i2c_configure(uint32_t i2c);
void i2c_start_addr(uint32_t i2c,uint8_t addr);
void i2c_write(uint32_t i2c,uint8_t byte);
void i2c_stop(uint32_t i2c);
void i2c_wait_busy(uint32_t i2c);

#endif // I2C_H

// End i2c.h
