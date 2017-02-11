#include <stdio.h>
#include <stdlib.h>
//#include "diag/Trace.h"
#include "stm32f4xx.h"
#include "I2C.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int
main_v1(int argc, char* argv[]) {
	volatile uint32_t data = 105;
	volatile uint32_t data2 = 1;
	//3fc4
	/*
	 * Enable the GPIO pins
	 */
	// Set PB6 SCL to and PB7 to SDA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable port clock.
	GPIOB->OTYPER |= (1 << 6) | (1 << 7); // Set to open drain
	GPIOB->MODER |= (2 << 12) | (2 << 14); // Set to AF
	//GPIOB->OSPEEDR |= (3 << 12) | (2 << 14); // Set the very high speed.
	GPIOB->PUPDR |= (1 << 12) | (1 << 14); // Turn on pull up resistors
	GPIOB->AFR[0] |= (4 << 24) | (4 << 28); // Set to AF4 (I2C1)

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= (1 << 26) | (1 << 28);
	GPIOD->ODR |= (1 << 13);
	GPIOD->ODR |= (1 << 14);

	/*
	 * Enable the clock in RCC
	 */
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
	uint32_t i;
	for(i=0; i < 10000000;i++) {;}
	RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	/*
	 * Setup the input clock in CR2
	 */
	I2C1->CR2 |= (8 << 0); // Set peripheral clock

	/*
	 * Configure CCR
	 */
	I2C1->CCR |= (0x28 << 0); // Set to SCL is 100kHz

	/*
	 * Configure rise time register
	 */
	I2C1->TRISE |= (9 << 0);

	/*
	 * Enable the peripheral in CR1
	 */
	I2C1->CR1 |= (1 << 0);

	for(i=0;i < 10000;i++) {;}

	while(1) {
		/*
		 * Set START signal in CR1
		 */
		I2C1->CR1 |= (1 << 8);

		/*
		 * Wait until the SB bit is set
		 */
		while((I2C1->SR1 & I2C_SR1_SB) == 0) {;}

		/*
		 * Write slave address to DR with the LSB reset (transmit)
		 */
		I2C1->DR = (uint8_t)((0x68 << 1) & ~(1 << 0));

		/*
		 * Wait until the slave address has been sent. ADDR bit will be set.
		 * Then read SR2
		 */
		while((I2C1->SR1 & I2C_SR1_ADDR) == 0){;}
		data2 = I2C1->SR2;

		/*
		 * Write register address to DR.
		 * 0111 0001
		 */
		//while((I2C1->SR1 & I2C_SR1_TXE) == 0) {;}
		I2C1->DR = 0x75;

		/*
		 * Wait until the DR is empty
		 */
		while((I2C1->SR1 & I2C_SR1_TXE) == 0) {;}
		//I2C1->CR1 |= I2C_CR1_STOP;

		/*
		 * Re-send the slave address and set to receive
		 */
		I2C1->CR1 |= (1 << 8);
		while((I2C1->SR1 & I2C_SR1_SB) == 0) {;}
		I2C1->DR = (uint8_t)((0x68 << 1) | (1 << 0));
		while((I2C1->SR1 & I2C_SR1_ADDR) == 0){;}
		data2 = I2C1->SR2;

		/*
		 * Get the data
		 */
		while((I2C1->SR1 & I2C_SR1_RXNE) == 0) {;}
		data = I2C1->DR;
		i++;

		/*
		 * Stop transmitting
		 */
		I2C1->CR1 |= I2C_CR1_STOP;

	}
}

#pragma GCC diagnostic pop
