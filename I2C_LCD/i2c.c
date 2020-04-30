/* A stm32f103 application I2C library
 * Warren W. Gay VE3WWG
 * Sat Nov 25 11:56:51 2017
 *
 * Notes:
 *	1. Master I2C mode only
 *	2. No interrupts are used
 *	3. ReSTART I2C is not supported
 *	4. Uses PB6=SCL, PB7=SDA
 *	5. Requires GPIOB clock enabled
 *	6. PB6+PB7 must be GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN
 *	7. Requires rcc_periph_clock_enable(RCC_I2C1);
 *	8. Requires rcc_periph_clock_enable(RCC_AFIO);
 *	9. 100 kHz
 */

#include "i2c.h"

/*********************************************************************
 * Configure I2C device for 100 kHz, 7-bit addresses
 *********************************************************************/

void i2c_configure(uint32_t i2c) {
	i2c_peripheral_disable(i2c);
	i2c_reset(i2c);
	I2C_CR1(i2c) &= ~I2C_CR1_STOP;	// Clear stop
	i2c_set_standard_mode(i2c);	// 100 kHz mode
	i2c_set_clock_frequency(i2c,I2C_CR2_FREQ_36MHZ); // APB Freq
	i2c_set_trise(i2c,36);		// 1000 ns
	i2c_set_dutycycle(i2c,I2C_CCR_DUTY_DIV2);
	i2c_set_ccr(i2c,180);		// 100 kHz <= 180 * 1 /36M
	i2c_set_own_7bit_slave_address(i2c,0x23); // Necessary?
	i2c_peripheral_enable(i2c);
}

/*********************************************************************
 * Return when I2C is not busy
 *********************************************************************/

void i2c_wait_busy(uint32_t i2c) {
	while ( I2C_SR2(i2c) & I2C_SR2_BUSY ){}
}

/*********************************************************************
 * Start I2C Read/Write Transaction with indicated 7-bit address:
 *********************************************************************/

void i2c_start_addr(uint32_t i2c,uint8_t addr) {
	i2c_wait_busy(i2c);			// Block until not busy
	I2C_SR1(i2c) &= ~I2C_SR1_AF;	// Clear Acknowledge failure
	i2c_clear_stop(i2c);		// Do not generate a Stop
	
	i2c_send_start(i2c);		// Generate a Start/Restart

	// Loop until ready:
	while ( !((I2C_SR1(i2c) & I2C_SR1_SB) 
	  && (I2C_SR2(i2c) & (I2C_SR2_MSL|I2C_SR2_BUSY))) ) {}

	// Send Address & R/W flag:
	i2c_send_7bit_address(i2c,addr,I2C_WRITE);

	// Wait until completion, NAK or timeout
	while ( !(I2C_SR1(i2c) & I2C_SR1_ADDR) ) {
		if ( I2C_SR1(i2c) & I2C_SR1_AF ) {
			i2c_send_stop(i2c);
			(void)I2C_SR1(i2c);
			(void)I2C_SR2(i2c); 	// Clear flags
			// NAK Received (no ADDR flag will be set here)
		}

	}

	(void)I2C_SR2(i2c);		// Clear flags
}

/*********************************************************************
 * Send stop
 *********************************************************************/

void i2c_stop(uint32_t i2c) {
	i2c_send_stop(i2c);
}

/*********************************************************************
 * Write one byte of data
 *********************************************************************/

void i2c_write(uint32_t i2c,uint8_t byte) {
	/*
	//Send START
	i2c_send_start(i2c);

	// Loop until ready:
	while ( !((I2C_SR1(i2c) & I2C_SR1_SB)
	  && (I2C_SR2(i2c) & (I2C_SR2_MSL|I2C_SR2_BUSY))) ) {}

	// Send Address -- Write
	i2c_send_7bit_address(i2c,0x4E,I2C_WRITE);

	// Wait until completion, NAK or timeout
	while ( !(I2C_SR1(i2c) & I2C_SR1_ADDR) ) {}

	(void)I2C_SR2(i2c);		// Clear flags
	*/
	i2c_send_data(i2c,byte);

	//Wait for transmision complete
	while ( !(I2C_SR1(i2c) & (I2C_SR1_BTF)) ) {}

	//i2c_send_stop(i2c);
}

// i2c.c
