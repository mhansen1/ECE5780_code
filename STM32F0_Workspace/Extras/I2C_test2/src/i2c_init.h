/*
 * i2c_init.h
 *
 *  Created on: Mar 8, 2016
 *      Author: Benjamin
 */

#ifndef I2C_INIT_H_
#define I2C_INIT_H_

#include "stm32f4xx.h"


void i2c_init(void);
void i2c_init_gyro();
void writeByteToI2C(uint16_t SAD, uint16_t REG, uint16_t VAL);
void get_data(uint8_t *data);


#endif /* I2C_INIT_H_ */
