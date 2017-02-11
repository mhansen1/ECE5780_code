#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include <stdlib.h>

// Used to tell if the I2C channel is running for not. Don't want to accidently get stuck
// in a loop if one the peripheral is disabled.
uint8_t I2C1_started;
uint8_t I2C2_started;
uint8_t I2C3_started;

void init_I2C(uint32_t freq, char port, uint8_t pin_set, uint8_t channel, uint8_t max_bytes);
void I2C1_read(uint8_t slave_addr, uint8_t dest_addr, uint8_t num_bytes, uint8_t *data);
void I2C1_write(uint8_t slave_addr, uint8_t dest_addr, uint8_t num_bytes, uint8_t *data);

/** TODO
 *
 * Create a destruction function so that the memory for the I2C data struct can be released.
 *
 */

