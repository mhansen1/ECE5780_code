#include "I2C.h"

/*
	Setup the I2C module
	
	Inputs
		freq - The desired SCL frequency. Has a range of 20 kHz to 400 kHz.
		port - The GPIO port that is being used (A, B, C, etc...)
		pin_set - The set of two pins being used. Is in the order the set is found
		             on the port. For example, PB6 and PB7, pin_set is 0, and if itoa
					 is PB8 and PB9, then pin_set is 1, and so on.
		channel - The I2C channel. This will correspond to I2C1, I2C2, and I2C3.
*/
void init_I2C(uint32_t freq, char port, uint8_t pin_set, uint8_t channel) {
	/*
	 * Setup the pins
	 */
	 // If using port B. This has SCL on pins PB6, PB8, and PB10.
	 // SDA is on pins PB7, PB9, and PB11.
	 if(port == 'b' || port == 'B') {
		// If pin_set is outside the range, default to 0.
		if(pin_set < 0 || pin_set > 2)
			pin_set = 0;
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable port clock.
		GPIOB->MODER |= (10 << (4*pin_set + 12)); // Set to AF.
		GPIOB->OTYPER |= (3 << (2*pin_set + 6)); // Set to open drain.
		uint32_t temp = GPIOB->MODER;
		//GPIOB->PUPDR |= (5 << (4*pin_set + 8)); // Turn on pull-up resistor
		// Set AF to AF4 (I2C)
		if(pin_set) {
			GPIOB->AFR[1] |= (68 << (pin_set*8 - 8));
		}
		else {
			GPIOB->AFR[0] |= (4 << 24) | (4 << 28);
		}
	 }
	 // Use port F pins PF0(SDA) and PF1(SCL)
	 else if(port == 'f' || port == 'F') {
		if(pin_set != 0)
			pin_set = 0;
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN; // Enable port clock.
		GPIOF->MODER |= (10 << 0); // Set to AF.
		GPIOF->OTYPER |= (3 << 0); // Set to open drain.
		GPIOF->PUPDR |= (5 << 0); // Turn on pull-up resistor
		// Set AF to AF4 (I2C)
		GPIOF->AFR[0] |= (4 << 0) | (4 << 4);
	 }
	 // Use port H. Uses SCL pins PH4 and PH7, and SDA PH5 and PH8
	 else if(port == 'h' || port == 'H') {
		 if(pin_set < 0 || pin_set > 1)
			 pin_set = 0;
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN; // Enable port clock.
		GPIOH->MODER |= (10 << (6*pin_set + 8)); // Set to AF.
		GPIOH->OTYPER |= (3 << (3*pin_set + 4)); // Set to open drain.
		GPIOH->PUPDR |= (5 << (6*pin_set + 8)); // Turn on pull-up resistor
		// Set AF to AF4 (I2C)
		if(pin_set) {
			GPIOH->AFR[0] |= (4 << 24) | (4 << 28);
		}
		else {
			GPIOH->AFR[0] |= (4 << 16) | (4 << 20);
		}
	 }
	 
	 /*
	  * Setup RCC clock, and SCL in CR2, CCR, and TRISE registers.
	  */
	// Make sure that the desired frequency is within the range.
	if(freq > 400000)
		freq = 400000;
	else if(freq < 20000)
		freq = 20000;
	// If outside the range, default to channel 1.
	if(channel < 1 || channel > 3)
		channel = 1;
	
	if(channel == 1) {
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
		RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
		uint32_t i;
		for(i=0; i < 10000000;i++) {;}
		RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
		
		//Set peripheral clock to 20MHz.
		//I2C1->CR2 |= (20 << 0);
		I2C1->CR2 |= (20 << 0);
		// Configure CCR
		// If freq is greater than 100kHz, then need to set to fast mode
		// t_pclk = 1/(20e6)
		// t_clk = 1/freq 
		// t_high = (1/3)*t_clk = CCR*t_pclk
		// t_low = (2/3)*t_clk 
		// CCR = t_clk/(3*t_pclk)
		if(freq > 100000) {
			I2C1->CCR |= (1 << 15); // Turn on fast mode.
			I2C1->CCR |= (1 << 14); // Set duty cycle to 16/9.
			I2C1->CCR |= (uint32_t)((20000000/(12*freq))); // Calculate the high and low times.
			I2C1->TRISE |= 4;
		}
		else {
			// t_pclk = 1/(20e6)
			// t_clk = 1/freq
			// t_high = t_clk/2 = CCR*t_pclk
			// CCR = t_clk/(2*t_pclk)
			I2C1->CCR &= ~(1 << 15); // Turn on slow mode.
			I2C1->CCR |= (uint32_t)(20000000/(freq));
			I2C1->TRISE |= (21 << 0);
			//I2C1->CCR |= (0x28 << 0);
			//I2C1->TRISE |= (9 << 0);
		}
		
		// Enable the I2C1 peripheral.
		I2C1->CR1 |= (1 << 0);
		I2C1_started = 1;
	}
	else if(channel == 2) {
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
		RCC->APB1RSTR |= RCC_APB1RSTR_I2C2RST;
		uint32_t i;
		for(i=0; i < 10000000;i++) {;}
		RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C2RST;
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
		
		//Set peripheral clock to 20MHz.
		//I2C2->CR2 |= (20 << 0);
		I2C2->CR2 |= (20 << 0);
		// Configure CCR
		// If freq is greater than 100kHz, then need to set to fast mode
		// t_pclk = 1/(20e6)
		// t_clk = 1/freq 
		// t_high = (1/3)*t_clk = CCR*t_pclk
		// t_low = (2/3)*t_clk 
		// CCR = t_clk/(3*t_pclk)
		if(freq > 100000) {
			I2C2->CCR |= (1 << 15); // Turn on fast mode.
			I2C2->CCR |= (1 << 14); // Set duty cycle to 16/9.
			I2C2->CCR |= (uint32_t)((20000000/(12*freq))); // Calculate the high and low times.
			I2C2->TRISE |= 4;
		}
		else {
			// t_pclk = 1/(20e6)
			// t_clk = 1/freq
			// t_high = t_clk/2 = CCR*t_pclk
			// CCR = t_clk/(2*t_pclk)
			I2C2->CCR &= ~(1 << 15); // Turn on slow mode.
			I2C2->CCR |= (uint32_t)(20000000/(freq));
			I2C2->TRISE |= (21 << 0);
			//I2C2->CCR |= (0x28 << 0);
			//I2C2->TRISE |= (9 << 0);
		}
		
		// Enable the I2C2 peripheral.
		I2C2->CR1 |= (1 << 0);
		I2C2_started = 1;
	}
	else if(channel == 3) {
		RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
		RCC->APB1RSTR |= RCC_APB1RSTR_I2C3RST;
		uint32_t i;
		for(i=0; i < 10000000;i++) {;}
		RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C3RST;
		RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
		
		//Set peripheral clock to 20MHz.
		//I2C3->CR2 |= (20 << 0);
		I2C3->CR2 |= (20 << 0);
		// Configure CCR
		// If freq is greater than 100kHz, then need to set to fast mode
		// t_pclk = 1/(20e6)
		// t_clk = 1/freq 
		// t_high = (1/3)*t_clk = CCR*t_pclk
		// t_low = (2/3)*t_clk 
		// CCR = t_clk/(3*t_pclk)
		if(freq > 100000) {
			I2C3->CCR |= (1 << 15); // Turn on fast mode.
			I2C3->CCR |= (1 << 14); // Set duty cycle to 16/9.
			I2C3->CCR |= (uint32_t)((20000000/(12*freq))); // Calculate the high and low times.
			I2C3->TRISE |= 4;
		}
		else {
			// t_pclk = 1/(20e6)
			// t_clk = 1/freq
			// t_high = t_clk/2 = CCR*t_pclk
			// CCR = t_clk/(2*t_pclk)
			I2C3->CCR &= ~(1 << 15); // Turn on slow mode.
			I2C3->CCR |= (uint32_t)(20000000/(freq));
			I2C3->TRISE |= (21 << 0);
			//I2C3->CCR |= (0x28 << 0);
			//I2C3->TRISE |= (9 << 0);
		}
		
		// Enable the I2C3 peripheral.
		I2C3->CR1 |= (1 << 0);
		I2C3_started = 1;
	}
	
	return;
}


/*
	Send data to an I2C device. Uses I2C1 peripheral
	
	Inputs:
		slave_add - This is the slave address of the device you want to write to.
		source_reg - The register address that you want to write to in the destination device.
		num_bytes - Number of bytes being transmitted.
		data - The data you want to write into the source register.
		
	Outputs:
		NONE
*/
void I2C1_send(uint8_t slave_add, uint8_t source_reg, uint8_t num_bytes,uint8_t *data) {
	if(I2C1_started) {
		I2C1->CR1 |= (1 << 8); // Set start condition.
		while((I2C1->SR1 & I2C_SR1_SB) == 0) {;} // Wait until start condition is set.
		I2C1->DR = (uint8_t)((slave_add << 1) & ~(1 << 0)); // Write slave address and reset read/write bit.
		while((I2C1->SR1 & I2C_SR1_ADDR) == 0) {;} // Wait until the address is sent
		uint32_t temp = I2C1->SR2; // Need to read from SR2 to continue.
		I2C1->DR = (uint8_t)source_reg; // Set destination register address.
		while((I2C1->SR1 & I2C_SR1_TXE) == 0) {;} // Wait until byte transmits
		//I2C1->CR1 |= I2C_CR1_START; // RESTART the transmission.
		//while((I2C1->SR1 & I2C_SR1_SB) == 0) {;};
		//I2C1->DR = (uint8_t)((slave_add << 1) & ~(1 << 0)); // Re-send slave address.
		//while((I2C1->SR1 & I2C_SR1_ADDR)==0) {;} // Wait for start condition to be valid
		//temp = I2C1->SR2; // Read from SR2 to continue.
		for(temp = 0; temp < num_bytes; temp++) {
			I2C1->DR = (uint8_t) data[temp]; // write data to data register.
			while((I2C1->SR1 & I2C_SR1_TXE) == 0) {;} // Wait until the byte has been sent.
		}
		I2C1->CR1 |= I2C_CR1_STOP; // Stop the I2C communication.
	}
	return;	
}

/*
	Read data from an I2C device. Uses I2C1 peripheral
	
	Inputs:
		slave_add - This is the slave address of the device you want to write to.
		source_reg - The register address that you want to read from in the destination device.
		num_bytes - The number of bytes being read
		*data - Pointer to the data. Can be a singly byte of an array. This is the data being returned
		
	Outputs:
		Returns data in the pinter *data.
*/
void I2C1_read(uint8_t slave_add, uint8_t source_reg, uint8_t num_bytes, uint8_t *data) {
	if(I2C1_started) {
		I2C1->CR1 |= (1 << 8); // Set start condition.
		while((I2C1->SR1 & I2C_SR1_SB) == 0) {;} // Wait until start condition is set.
		I2C1->DR = (uint8_t)((slave_add << 1) & ~(1 << 0)); // Write slave address and reset read/write bit.
		while((I2C1->SR1 & I2C_SR1_ADDR) == 0) {;} // Wait until the address is sent
		uint32_t temp = I2C1->SR2; // Need to read from SR2 to continue.
		I2C1->DR = (uint8_t)source_reg; // Set destination register address.
		while((I2C1->SR1 & I2C_SR1_TXE) == 0) {;} // Wait until byte transmits
		I2C1->CR1 |= I2C_CR1_START; // RESTART the transmission.
		while((I2C1->SR1 & I2C_SR1_SB) == 0) {;}
		I2C1->DR = (uint8_t)((slave_add << 1) | (1 << 0)); // Re-send slave address.
		while((I2C1->SR1 & I2C_SR1_ADDR)==0) {;} // Wait for start condition to be valid
		temp = I2C1->SR2; // Read from SR2 to continue.
		for(temp = 0; temp < num_bytes; temp++) {
			if(temp == num_bytes-1)
				I2C1->CR1 &= ~I2C_CR1_ACK;
			else
				I2C1->CR1 |= I2C_CR1_ACK;
			while((I2C1->SR1 & I2C_SR1_RXNE) == 0) {;} // Wait for all data to be read in.
			data[temp] = I2C1->DR;
		}
		I2C1->CR1 |= I2C_CR1_STOP; // Stop transmitting.
		return; // Return the recieved data.
	}
	else
		return;
}

/*
	Turns off the I2C1 peripheral
*/
void I2C1_disable() {
	I2C1->CR1 &= ~(1 << 0); // Turn off the peripheral.
	RCC->APB1ENR &= ~RCC_APB1ENR_I2C1EN;
	RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
	I2C1_started = 0;
	return;
}

/*
	Restarts the I2C1 peripheral. Does not configure the peripheral.
*/
void I2C1_restart() {
	RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	I2C1->CR1 |= (1 << 0);
	I2C1_started = 1;
	return;
}

/*
	Send data to an I2C device. Uses I2C2 peripheral
	
	Inputs:
		slave_add - This is the slave address of the device you want to write to.
		source_reg - The register address that you want to write to in the destination device.
		num_bytes - Number of bytes being transmitted.
		data - The data you want to write into the source register.
		
	Outputs:
		NONE
*/
void I2C2_send(uint8_t slave_add, uint8_t source_reg, uint8_t num_bytes,uint8_t *data) {
	if(I2C2_started) {
		I2C2->CR1 |= (1 << 8); // Set start condition.
		while((I2C2->SR1 & I2C_SR1_SB) == 0) {;} // Wait until start condition is set.
		I2C2->DR = (uint8_t)((slave_add << 1) & ~(1 << 0)); // Write slave address and reset read/write bit.
		while((I2C2->SR1 & I2C_SR1_ADDR) == 0) {;} // Wait until the address is sent
		uint32_t temp = I2C2->SR2; // Need to read from SR2 to continue.
		I2C2->DR = (uint8_t)source_reg; // Set destination register address.
		while((I2C2->SR1 & I2C_SR1_TXE) == 0) {;} // Wait until byte transmits
		//I2C2->CR1 |= I2C_CR1_START; // RESTART the transmission.
		//while((I2C2->SR1 & I2C_SR1_SB) == 0) {;};
		//I2C2->DR = (uint8_t)((slave_add << 1) & ~(1 << 0)); // Re-send slave address.
		//while((I2C2->SR1 & I2C_SR1_ADDR)==0) {;} // Wait for start condition to be valid
		//temp = I2C2->SR2; // Read from SR2 to continue.
		for(temp = 0; temp < num_bytes; temp++) {
			I2C2->DR = (uint8_t) data[temp]; // write data to data register.
			while((I2C2->SR1 & I2C_SR1_TXE) == 0) {;} // Wait until the byte has been sent.
		}
		I2C2->CR1 |= I2C_CR1_STOP; // Stop the I2C communication.
	}
	return;	
}

/*
	Read data from an I2C device. Uses I2C2 peripheral
	
	Inputs:
		slave_add - This is the slave address of the device you want to write to.
		source_reg - The register address that you want to read from in the destination device.
		num_bytes - The number of bytes being read
		*data - Pointer to the data. Can be a singly byte of an array. This is the data being returned
		
	Outputs:
		Returns data in the pinter *data.
*/
void I2C2_read(uint8_t slave_add, uint8_t source_reg, uint8_t num_bytes, uint8_t *data) {
	if(I2C2_started) {
		I2C2->CR1 |= (1 << 8); // Set start condition.
		while((I2C2->SR1 & I2C_SR1_SB) == 0) {;} // Wait until start condition is set.
		I2C2->DR = (uint8_t)((slave_add << 1) & ~(1 << 0)); // Write slave address and reset read/write bit.
		while((I2C2->SR1 & I2C_SR1_ADDR) == 0) {;} // Wait until the address is sent
		uint32_t temp = I2C2->SR2; // Need to read from SR2 to continue.
		I2C2->DR = (uint8_t)source_reg; // Set destination register address.
		while((I2C2->SR1 & I2C_SR1_TXE) == 0) {;} // Wait until byte transmits
		I2C2->CR1 |= I2C_CR1_START; // RESTART the transmission.
		while((I2C2->SR1 & I2C_SR1_SB) == 0) {;}
		I2C2->DR = (uint8_t)((slave_add << 1) | (1 << 0)); // Re-send slave address.
		while((I2C2->SR1 & I2C_SR1_ADDR)==0) {;} // Wait for start condition to be valid
		temp = I2C2->SR2; // Read from SR2 to continue.
		for(temp = 0; temp < num_bytes; temp++) {
			if(temp == num_bytes-1)
				I2C2->CR1 &= ~I2C_CR1_ACK;
			else
				I2C2->CR1 |= I2C_CR1_ACK;
			while((I2C2->SR1 & I2C_SR1_RXNE) == 0) {;} // Wait for all data to be read in.
			data[temp] = I2C2->DR;
		}
		I2C2->CR1 |= I2C_CR1_STOP; // Stop transmitting.
		return; // Return the recieved data.
	}
	else
		return;
}

/*
	Turns off the I2C2 peripheral
*/
void I2C2_disable() {
	I2C2->CR1 &= ~(1 << 0); // Turn off the peripheral.
	RCC->APB1ENR &= ~RCC_APB1ENR_I2C2EN;
	RCC->APB1RSTR |= RCC_APB1RSTR_I2C2RST;
	I2C2_started = 0;
	return;
}

/*
	Restarts the I2C2 peripheral. Does not configure the peripheral.
*/
void I2C2_restart() {
	RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C2RST;
	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	I2C2->CR1 |= (1 << 0);
	I2C2_started = 1;
	return;
}

/*
	Send data to an I2C device. Uses I2C3 peripheral
	
	Inputs:
		slave_add - This is the slave address of the device you want to write to.
		source_reg - The register address that you want to write to in the destination device.
		num_bytes - Number of bytes being transmitted.
		data - The data you want to write into the source register.
		
	Outputs:
		NONE
*/
void I2C3_send(uint8_t slave_add, uint8_t source_reg, uint8_t num_bytes,uint8_t *data) {
	if(I2C3_started) {
		I2C3->CR1 |= (1 << 8); // Set start condition.
		while((I2C3->SR1 & I2C_SR1_SB) == 0) {;} // Wait until start condition is set.
		I2C3->DR = (uint8_t)((slave_add << 1) & ~(1 << 0)); // Write slave address and reset read/write bit.
		while((I2C3->SR1 & I2C_SR1_ADDR) == 0) {;} // Wait until the address is sent
		uint32_t temp = I2C3->SR2; // Need to read from SR2 to continue.
		I2C3->DR = (uint8_t)source_reg; // Set destination register address.
		while((I2C3->SR1 & I2C_SR1_TXE) == 0) {;} // Wait until byte transmits
		//I2C3->CR1 |= I2C_CR1_START; // RESTART the transmission.
		//while((I2C3->SR1 & I2C_SR1_SB) == 0) {;};
		//I2C3->DR = (uint8_t)((slave_add << 1) & ~(1 << 0)); // Re-send slave address.
		//while((I2C3->SR1 & I2C_SR1_ADDR)==0) {;} // Wait for start condition to be valid
		//temp = I2C3->SR2; // Read from SR2 to continue.
		for(temp = 0; temp < num_bytes; temp++) {
			I2C3->DR = (uint8_t) data[temp]; // write data to data register.
			while((I2C3->SR1 & I2C_SR1_TXE) == 0) {;} // Wait until the byte has been sent.
		}
		I2C3->CR1 |= I2C_CR1_STOP; // Stop the I2C communication.
	}
	return;	
}

/*
	Read data from an I2C device. Uses I2C3 peripheral
	
	Inputs:
		slave_add - This is the slave address of the device you want to write to.
		source_reg - The register address that you want to read from in the destination device.
		num_bytes - The number of bytes being read
		*data - Pointer to the data. Can be a singly byte of an array. This is the data being returned
		
	Outputs:
		Returns data in the pinter *data.
*/
void I2C3_read(uint8_t slave_add, uint8_t source_reg, uint8_t num_bytes, uint8_t *data) {
	if(I2C3_started) {
		I2C3->CR1 |= (1 << 8); // Set start condition.
		while((I2C3->SR1 & I2C_SR1_SB) == 0) {;} // Wait until start condition is set.
		I2C3->DR = (uint8_t)((slave_add << 1) & ~(1 << 0)); // Write slave address and reset read/write bit.
		while((I2C3->SR1 & I2C_SR1_ADDR) == 0) {;} // Wait until the address is sent
		uint32_t temp = I2C3->SR2; // Need to read from SR2 to continue.
		I2C3->DR = (uint8_t)source_reg; // Set destination register address.
		while((I2C3->SR1 & I2C_SR1_TXE) == 0) {;} // Wait until byte transmits
		I2C3->CR1 |= I2C_CR1_START; // RESTART the transmission.
		while((I2C3->SR1 & I2C_SR1_SB) == 0) {;}
		I2C3->DR = (uint8_t)((slave_add << 1) | (1 << 0)); // Re-send slave address.
		while((I2C3->SR1 & I2C_SR1_ADDR)==0) {;} // Wait for start condition to be valid
		temp = I2C3->SR2; // Read from SR2 to continue.
		for(temp = 0; temp < num_bytes; temp++) {
			if(temp == num_bytes-1)
				I2C3->CR1 &= ~I2C_CR1_ACK;
			else
				I2C3->CR1 |= I2C_CR1_ACK;
			while((I2C3->SR1 & I2C_SR1_RXNE) == 0) {;} // Wait for all data to be read in.
			data[temp] = I2C3->DR;
		}
		I2C3->CR1 |= I2C_CR1_STOP; // Stop transmitting.
		return; // Return the recieved data.
	}
	else
		return;
}

/*
	Turns off the I2C3 peripheral
*/
void I2C3_disable() {
	I2C3->CR1 &= ~(1 << 0); // Turn off the peripheral.
	RCC->APB1ENR &= ~RCC_APB1ENR_I2C3EN;
	RCC->APB1RSTR |= RCC_APB1RSTR_I2C3RST;
	I2C3_started = 0;
	return;
}

/*
	Restarts the I2C3 peripheral. Does not configure the peripheral.
*/
void I2C3_restart() {
	RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C3RST;
	RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
	I2C3->CR1 |= (1 << 0);
	I2C3_started = 1;
	return;
}
















