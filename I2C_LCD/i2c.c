
/************************************************************************
* Copyright 2020 ITESM                                                  *
*                                                                       *
*                                                                       *
* LCD_I2C                                                              *
*                                                                       *
* Autorhs:                                                              *
* Jesús Enrique Luna Medina          A01632334                          *
* Daniela abigail Parrales Mejía     A01228629                          *
* Luis Cortés Leal                   A01631163                          *
*                                                                       *
* Abril 2020                                                            *
* The project titled LCD_I2C is carried out with the purpose of        *
* implementing a temperature sensing system which acquires the          *
* values from the environment and delivers a response through           *
* hardware using LEDs and software displaying on a Terminal.            *
*                                                                       *
************************************************************************/

/*!< LIBRARIES */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>
#include "i2c.h"
#include <stdbool.h>

/***********************************************************************/

/************************************************************************
* Setting up the I2C interface.                                         *
*                                                                       *
************************************************************************/

void i2c_setup(void) {
    rcc_periph_clock_enable(RCC_I2C1); /*!< I2C */
    rcc_periph_clock_enable(RCC_GPIOB);    
    gpio_set_mode(GPIOB,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
        GPIO6|GPIO7); /*!< Ports for I2C */
    gpio_set(GPIOB, GPIO6|GPIO7); /*!< Idle high */
    gpio_primary_remap(0, 0);
    i2c_peripheral_disable(I2C1);
    i2c_reset(I2C1);
    I2C_CR1(I2C1) &= ~I2C_CR1_STOP; /*!< Clear stop */
    i2c_set_standard_mode(I2C1); /*!< 100 kHz */
    i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_36MHZ);
    i2c_set_trise(I2C1, 36); /*!< 1000 ns */
    i2c_set_dutycycle(I2C1, I2C_CCR_DUTY_DIV2);
    i2c_set_ccr(I2C1, 180); /*!< 100 kHz = (180*1/36M) */
    i2c_set_own_7bit_slave_address(I2C1, 0x23);
    i2c_peripheral_enable(I2C1);
}

/***********************************************************************/

/************************************************************************
 * Sends the I2C start address and waits for Acknowledge.               *
 *  @param[in] Read or write                                            * 
 *  @param[in] Address                                                  *
 *                                                                      *
 ***********************************************************************/

void i2c_start_addr(uint8_t addr, enum I2C_RW rw){
	uint32_t reg32 __attribute__((unused));

    while ( I2C_SR2(I2C1) & I2C_SR2_BUSY ) {} /*!< Block until not busy */
    I2C_SR1(I2C1) &= ~I2C_SR1_AF; /*!< Clear Acknowledge failure */
    i2c_clear_stop(I2C1); /*!< Do not generate a Stop */
    if ( rw == Read )
        i2c_enable_ack(I2C1);
    i2c_send_start(I2C1); /*!< Generate a Re/Start */
    /*!< Loop until ready */
    while ( !((I2C_SR1(I2C1) & I2C_SR1_SB) && (I2C_SR2(I2C1) & \
    (I2C_SR2_MSL|I2C_SR2_BUSY)))) {}

    /*!<Send Address and Read and Write flag */
    i2c_send_7bit_address(I2C1, addr, rw == 1 ? I2C_READ : I2C_WRITE);
	
    /*!< Wait until completion,  NAK or timeout */
    while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR)){
		gpio_set(GPIOA, GPIO7);	
		gpio_set(GPIOA, GPIO5);	
        if (I2C_SR1(I2C1) & I2C_SR1_AF){
            i2c_send_stop(I2C1);
            (void)I2C_SR1(I2C1);
            (void)I2C_SR2(I2C1); /*!< Clear flags */
			if (I2C_SR1(I2C1)){
				gpio_clear(GPIOA, GPIO5);
			}
			if (I2C_SR1_AF){
				gpio_clear(GPIOA, GPIO7);
			}
        }
    }
    (void)I2C_SR2(I2C1); /*!< Clear flags */
}

/***********************************************************************/

/************************************************************************
 * Writes restart instruction.                                          *
 *  @param[in] Address                                                  * 
 *  @param[in] Byte                                                     *
 *                                                                      *
 ***********************************************************************/

void i2c_write_restart(uint8_t byte, uint8_t addr){
    i2c_send_data(I2C1, byte);
    /*!< Must set start before byte has written out */
    i2c_send_start(I2C1);

    /*!< Wait for transmit to complete */
    while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF))){
    }

    /*!< Loop until restart ready */
        while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
      && (I2C_SR2(I2C1) & (I2C_SR2_MSL|I2C_SR2_BUSY)))){
    }

    /*!< Send Address & Read command bit */
    i2c_send_7bit_address(I2C1, addr, I2C_READ);

    /*!< Wait until completion,  NAK or timeout */
    while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR)){
        if (I2C_SR1(I2C1) & I2C_SR1_AF){
            i2c_send_stop(I2C1);
            (void)I2C_SR1(I2C1);
            (void)I2C_SR2(I2C1); /*!< Clear flags */
        }
    }
    i2c_enable_ack(I2C1);

    (void)I2C_SR2(I2C1); /*!< Clear flags */
}

/***********************************************************************/

/************************************************************************
 * Reads a value using the I2C interface.                               *
 *  @param[out] Data                                                    * 
 *  @param[in] Is last byte                                             *
 *                                                                      *
 ***********************************************************************/

uint8_t i2c_read(bool lastf){
    if (lastf)
        i2c_disable_ack(I2C1); /*!< Reading last byte */

    while (!(I2C_SR1(I2C1) & I2C_SR1_RxNE)){}

    return i2c_get_data(I2C1);
}

/***********************************************************************/

void i2c_stop(void){
    i2c_send_stop(I2C1);
}

/***********************************************************************/

void i2c_write(uint8_t byte){
    i2c_send_data(I2C1, byte);
    while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF))){
    }
}

/***********************************************************************/

void i2c_write_8bits(uint8_t addr, uint8_t byte){
    gpio_set(GPIOA, GPIO5);
    i2c_start_addr(addr, Write);
    i2c_write(byte);
    i2c_stop();
}

/***********************************************************************/
