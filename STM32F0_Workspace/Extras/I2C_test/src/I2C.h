#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

// Used to tell if the I2C channel is running for not. Don't want to accidently get stuck
// in a loop if one the peripheral is disabled.
uint8_t I2C1_started;
uint8_t I2C2_started;
uint8_t I2C3_started;

void init_I2C(uint32_t freq, char port, uint8_t pin_set, uint8_t channel);
void I2C1_send(uint8_t slave_add, uint8_t source_reg, uint8_t num_bytes,uint8_t *data);
void I2C1_read(uint8_t slave_add, uint8_t source_reg, uint8_t num_bytes, uint8_t *data);
void I2C1_disable();
void I2C1_restart();
void I2C2_send(uint8_t slave_add, uint8_t source_reg, uint8_t num_bytes,uint8_t *data);
void I2C2_read(uint8_t slave_add, uint8_t source_reg, uint8_t num_bytes, uint8_t *data);
void I2C2_disable();
void I2C2_restart();
void I2C3_send(uint8_t slave_add, uint8_t source_reg, uint8_t num_bytes,uint8_t *data);
void I2C3_read(uint8_t slave_add, uint8_t source_reg, uint8_t num_bytes, uint8_t *data);
void I2C3_disable();
void I2C3_restart();
