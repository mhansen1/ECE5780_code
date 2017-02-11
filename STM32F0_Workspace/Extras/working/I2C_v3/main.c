
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "I2C.h"
#include "delay.h"
#include "led_init.h"

// Used for the callback versions of the I2C.
void test_callback();
int8_t test_var;

/*
 * Example use of the I2C.h file
 */
int main(void)
{
	led_init(0x0F);

	// Use delays to allow the I2C to finish before using a break point. If the transmission hasn't finished and a breakpoint is hit,
	// the transmission will stop (uses interrupts, so even after the function is called, the transmission hasn't finished by the next instruction.
	delay_init();
	// Initialize the I2C1
	init_I2C(400000, 'B', 0, 1,16);
	// Data to store.
	uint8_t test_data[14];
	test_var = 20; // Initialize the callback test variable to 20.
	// Data to be sent.
	uint8_t send_data[3] = {(3 << 3), (2 << 3), (4 << 0) | (1 << 3)};
	// I2C functions.
	I2C1_read_cb(0x68, 0x3B, 14, test_data, (callback)test_callback);
	delay_ms(5);
	I2C1_read(0x68,0x3B,14,test_data);
	delay_ms(5);
	I2C1_write(0x68,0x1B,3,send_data);
	delay_ms(5);
	test_var = 20; // Reset the callback test variable back to 20.
	I2C1_write_cb(0x68,0x1B,3,send_data, (callback)test_callback);
	delay_ms(5);

	while (1)
	{
	}
}

// The callback function to be used. Just changes the test variable value to keep things simple.
void test_callback() {
	test_var = -20;
	return;
}


