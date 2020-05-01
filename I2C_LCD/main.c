/* Demo of I2C I/O with PCF8574 device
 * Warren Gay Sat Dec  9 17:36:29 2017
 *
 *	PCF8574 /INT	on PC14
 *	LED		on PC13
 *	I2C		on PB6, PB7
 */
#include <string.h>
#include <stdbool.h>
#include <setjmp.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>

#include "i2c.h"
#include "LCD.h"

//#define PCF8574_ADDR(n)		(0x20|((n)&7))	// PCF8574
#define PCF8574_ADDR(n)	(0x38|((n)&7))	// PCF8574A


int main(void) {
	//uint8_t addr = PCF8574_ADDR(0);	// I2C Address
	//uint8_t addr = 0x4E;
	uint8_t addr = 0x27;

	rcc_clock_setup_in_hse_8mhz_out_72mhz();// For "blue pill"
	rcc_periph_clock_enable(RCC_GPIOC);	// LED
	/*****/
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, 
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO5|GPIO7);
    
    /*!< LEDs are turned off */
    gpio_set(GPIOA, GPIO5);
	gpio_set(GPIOA, GPIO7);
	/*****/
	gpio_set_mode(GPIOC,
		GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO13);			// LED on PC13
	gpio_set(GPIOC,GPIO13);			// PC13 LED dark

	// Configure I2C1
	//i2c_configure(I2C1);
	i2c_setup();
	gpio_clear(GPIOA, GPIO5);

	/*********************************************
	* This example performs a write transaction,
	* followed by a separate read transaction:
	*********************************************/
	LCD_Init(addr);

	LCD_Set_Cursor(1, 1);
	
  	LCD_Write_String("aaaaaaaaaa");
  	LCD_Set_Cursor(2, 1);
  	LCD_Write_String("bbbbbbbbbb");

	
	for (;;);
	return 0;
}

// End main.c
