#include "I2C.h"

typedef enum {
	idle, // I2C isn't doing anything.
	dest_reg, // Sending the destination register.
	transm // Transmitting data (reading or writing).
} state_enum;

typedef struct {
	uint8_t slave_addr; // The slave address
	uint8_t dest_reg; // The destination register address in the slave
	uint8_t num_bytes; // Number of bytes transmitting.
	uint8_t *read_data; // The data that's read in.
	uint8_t *write_data; // The data that's being sent.
	uint8_t rd_wrt; // Says whether the current transmission is a read or write.
	                // If 0, reading, if 1, writing.
	state_enum state; // Current state of the bus.
	uint8_t byte_count; // Current number of bytes sent/received.
	uint8_t max_bytes; // Maximum number of bytes able to transmit.
} I2C_data;


volatile I2C_data *I2C1_data;

void I2C1_EV_IRQHandler(void) {
	// If the start bit has just been set.
	if(I2C1->SR1 & I2C_SR1_SB) {
		// If writing data or sending the destination register...
		if(I2C1_data->rd_wrt || (I2C1_data->state == dest_reg))
			I2C1->DR = (uint8_t)((I2C1_data->slave_addr << 1) & ~(1 << 0));
		// Else, reading...
		else
			I2C1->DR = (uint8_t)((I2C1_data->slave_addr << 1) | (1 << 0));
	}
	// If the slave address has just been sent...
	if(I2C1->SR1 & I2C_SR1_ADDR) {
		uint32_t useless_temp = I2C1->SR2; // Need to read from SR2 in order to continue.
		// If sending the destination register...
		if(I2C1_data->state == dest_reg)
			I2C1->DR = (uint8_t)I2C1_data->dest_reg; // Set destination register address.
		// If transmitting data, set the data.
		else if(I2C1_data->rd_wrt)
			I2C1->DR = (uint8_t)(*I2C1_data->write_data);
	}
	// If the data has been sent...
	if(I2C1->SR1 & I2C_SR1_TXE) {
		// If the destination register has been sent, re-start and change the state.
		if(I2C1_data->state == dest_reg) {
			I2C1->CR1 |= I2C_CR1_START;
			I2C1_data->state = transm;
			I2C1->CR1 |= I2C_CR1_ACK;
		}
		// Else, done writing data.
		else if(I2C1_data->rd_wrt) {
			if(I2C1_data->byte_count == (I2C1_data->num_bytes+1))
				I2C1->CR1 |= I2C_CR1_STOP;
			else {
				I2C1->DR = (uint8_t)(*(I2C1_data->write_data+I2C1_data->byte_count-1));
				I2C1_data->byte_count++;
			}
		}
	}
	// If just received data, read in the data, and stop the transmission.
	if(I2C1->SR1 & I2C_SR1_RXNE) {
		if((I2C1_data->byte_count == (I2C1_data->num_bytes-2)) || (I2C1_data->byte_count == (I2C1_data->max_bytes-2))) {
			I2C1->CR1 &= ~I2C_CR1_ACK;
			I2C1->CR1 |= I2C_CR1_STOP;
		}
		else
			I2C1->CR1 |= I2C_CR1_ACK;
		*(I2C1_data->read_data+I2C1_data->byte_count) = (uint8_t)I2C1->DR;
		I2C1_data->byte_count++;
	}


}

void I2C1_read(uint8_t slave_addr, uint8_t dest_addr, uint8_t num_bytes, uint8_t *data) {
	// Set state to "dest_reg" to transmit destination register.
	I2C1_data->state = dest_reg;

	// Set the data in the struct.
	I2C1_data->slave_addr = slave_addr;
	I2C1_data->dest_reg = dest_addr;
	I2C1_data->num_bytes = num_bytes;
	I2C1_data->read_data = &data[0]; // Point the read data to the address of the first element.
	I2C1_data->rd_wrt = 0;
	I2C1_data->byte_count = 0;

	// Set the start bit.
	I2C1->CR1 |= I2C_CR1_START;

	// Return and wait for the interrupts.
	return;
}

void I2C1_write(uint8_t slave_addr, uint8_t dest_addr, uint8_t num_bytes, uint8_t *data) {
	// Set state to "dest_reg" to transmit destination register.
	I2C1_data->state = dest_reg;

	// Set the data in the struct
	I2C1_data->slave_addr = slave_addr;
	I2C1_data->dest_reg = dest_addr;
	I2C1_data->num_bytes = num_bytes;
	I2C1_data->read_data = NULL;
	I2C1_data->rd_wrt = 1;
	I2C1_data->byte_count = 0;
	uint8_t i;
	uint8_t temp[3];

	*(I2C1_data->write_data+0) = data[0];
	temp[0] = (I2C1_data+0)->write_data;

	*(I2C1_data->write_data+1) = data[1];
	temp[1] = (I2C1_data+1)->write_data;

	*(I2C1_data->write_data+2) = data[2];
	temp[2] = (I2C1_data+2)->write_data;
//	for(i = 0; i < 3; i++) {
//		*((I2C1_data+i)->write_data) = (uint8_t)data[i];
//		temp[i] = (I2C1_data+i)->write_data;
//	}

	// Start the I2C
	I2C1->CR1 |= I2C_CR1_START;

	return;
}
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
void init_I2C(uint32_t freq, char port, uint8_t pin_set, uint8_t channel, uint8_t max_bytes) {
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
		GPIOB->PUPDR |= (5 << (4*pin_set + 8)); // Turn on pull-up resistor
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
		// Reset the but in case it's being locked out by the slave
		I2C1->CR1 |= (1 << 15);
		for(i = 0; i < 1000; i++) {;}
		I2C1->CR1 &= ~(1 << 15);
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
		// Enable the interrupt.
		I2C1->CR2 |= (1 << 9);
		I2C1->CR2 |= (1 << 10);
		NVIC->ISER[0] |= (1 << 31);
		I2C1_started = 1;
		I2C1_data->state = idle;
		I2C1_data->max_bytes = max_bytes;
		I2C1_data = malloc(sizeof(I2C_data) + sizeof(uint8_t)*I2C1_data->max_bytes);
	}
	else if(channel == 2) {
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
		RCC->APB1RSTR |= RCC_APB1RSTR_I2C2RST;
		uint32_t i;
		for(i=0; i < 10000000;i++) {;}
		RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C2RST;
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
		// Reset the but in case it's being locked out by the slave
		I2C2->CR1 |= (1 << 15);
		for(i = 0; i < 1000; i++) {;}
		I2C2->CR1 &= ~(1 << 15);
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
		// Reset the but in case it's being locked out by the slave
		I2C3->CR1 |= (1 << 15);
		for(i = 0; i < 1000; i++) {;}
		I2C3->CR1 &= ~(1 << 15);
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






















