/*
 * i2c_init.c
 *
 *  Created on: Mar 8, 2016
 *      Author: Benjamin
 */


#include "i2c_init.h"
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "delay.h"



static uint16_t MPU_Gyro_Accel = 0x68; // 1101000
//static uint16_t MPU_Mag = 0x0C;

/*
 * Initializes B8 and B9 to AF Mode; I2C1 Assignment and Init.
 */
void i2c_init(void){

	//initialize RCC for GPIOB and I2C1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	//struct for GPIO settings:
	GPIO_InitTypeDef GPIO_InitStruct_I2C;

	//GPIO settings for I2C
	GPIO_InitStruct_I2C.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct_I2C.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStruct_I2C.GPIO_Pin = (GPIO_Pin_8 | GPIO_Pin_9);
	GPIO_InitStruct_I2C.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct_I2C.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct_I2C);


	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);	// SDA = PB9
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1); // SCL = PB8

	//initialize RCC I2C1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);


	//I2C_DeInit(I2C1);

	//I2C struct for protocol settings
	I2C_InitTypeDef I2C_InitStruct;

	//I2C communication protocol settings:
	I2C_StructInit(&I2C_InitStruct);
	I2C_InitStruct.I2C_ClockSpeed = 400000;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x68;
	I2C_Init(I2C1, &I2C_InitStruct);

	//I2C enable:
	I2C_Cmd(I2C1, ENABLE);

	//If I2C bus is stuck toggle lines to clear it up
	delay_ms(150);
	//while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)){
		//delay_ms(150);
		//I2C_GenerateSTART(I2C1, ENABLE);
		//I2C_GenerateSTOP(I2C1, ENABLE);
	//}

}

/*
 * Helper method to write a byte via I2C
 */
void writeByteToI2C(uint16_t address, uint16_t REG, uint16_t VAL) {

	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	I2C_GenerateSTART(I2C1, ENABLE);

	// wait for I2C1 EV5 --> Slave has acknowledged start condition
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	// Send slave Address for write
	I2C_Send7bitAddress(I2C1, (uint8_t) address<<1, I2C_Direction_Transmitter);

	STM_EVAL_LEDOn(LED5);

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C1, (uint8_t) (REG));

	STM_EVAL_LEDOff(LED5);

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C1, (uint8_t) (VAL));

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTOP(I2C1, ENABLE);
}



/*
 * read number of bytes from I2C from address and compute angle using accelerometer and gyroscope readings
 *
 * For MCU6050 read from I2C slave address (SAD 0x68) sequentially from reg 0x3B for 14 bytes: accel has six registers (32 byte reading),
 * gyro has six more registers, and temperature has 2 and is in between accel and gyro.  I just read it
 * because skipping it on I2C would take many different I2C transfers, while reading sequentially happens
 * automatically.
 *
 * */
void get_data(uint8_t *data){

	/*
	 * Part 1: Get Data from Gyro and Accel
	 */
	int i = 0;

	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	I2C_AcknowledgeConfig(I2C1, ENABLE);

	I2C_GenerateSTART(I2C1, ENABLE);

	// wait for I2C1 EV5 --> Slave has acknowledged start condition
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	// Send slave Address for write
	I2C_Send7bitAddress(I2C1, MPU_Gyro_Accel<<1, I2C_Direction_Transmitter);

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C1, (uint8_t) (0x3B));

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTART(I2C1, ENABLE);

	// wait for I2C1 EV5 --> Slave has acknowledged start condition
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	// Send slave Address for read
	I2C_Send7bitAddress(I2C1, MPU_Gyro_Accel<<1, I2C_Direction_Receiver);

	for(i = 0; i<13; i++){
		// wait until one byte has been received
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
		//read data:
		data[i] = I2C_ReceiveData(I2C1);
	}

	//MPU uses NACK for last read
	I2C_AcknowledgeConfig(I2C1, DISABLE);

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));

	//read last byte
	data[13] = I2C_ReceiveData(I2C1);

	I2C_GenerateSTOP(I2C1, ENABLE);


	/*
	 * Part 2: Get Data from Compass
	 */
//	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
//
//	I2C_AcknowledgeConfig(I2C1, ENABLE);
//
//	I2C_GenerateSTART(I2C1, ENABLE);
//
//	// wait for I2C1 EV5 --> Slave has acknowledged start condition
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//
//	// Send slave Address for write
//	I2C_Send7bitAddress(I2C1, MPU_Mag<<1, I2C_Direction_Transmitter);
//
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//
//	I2C_SendData(I2C1, (uint8_t) (0x03));
//
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//
//	I2C_GenerateSTART(I2C1, ENABLE);
//
//	// wait for I2C1 EV5 --> Slave has acknowledged start condition
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//
//	// Send slave Address for read
//	I2C_Send7bitAddress(I2C1, MPU_Mag<<1, I2C_Direction_Receiver);
//
//	for(i = 14; i<18; i++){
//		// wait until one byte has been received
//		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
//		//read data:
//		data[i] = I2C_ReceiveData(I2C1);
//	}
//
//	//MPU uses NACK for last read
//	I2C_AcknowledgeConfig(I2C1, DISABLE);
//
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
//
//	//read last byte
//	data[19] = I2C_ReceiveData(I2C1);
//
//	I2C_GenerateSTOP(I2C1, ENABLE);
}



/*
 * Configure Accel and Gyro for Initialization (sample rate and filter  control).
 */
void i2c_init_gyro(){
	writeByteToI2C(MPU_Gyro_Accel, 0x6B, 0x01); //clock select- 1 = auto select best source available (pll if ready, else internal osc)
	writeByteToI2C(MPU_Gyro_Accel, 0x19, 0x00); //sample rate
	writeByteToI2C(MPU_Gyro_Accel, 0x1A, 0x05); //on-board filter control (0 is low - 6 is high)
	writeByteToI2C(MPU_Gyro_Accel, 0x1B, 0x00);
	writeByteToI2C(MPU_Gyro_Accel, 0x1C, 0x00);
	writeByteToI2C(MPU_Gyro_Accel, 0x1D, 0x00);
	writeByteToI2C(MPU_Gyro_Accel, 0x1E, 0x00);
	writeByteToI2C(MPU_Gyro_Accel, 0x1F, 0x00);
	writeByteToI2C(MPU_Gyro_Accel, 0x20, 0x00);
	writeByteToI2C(MPU_Gyro_Accel, 0x21, 0x00);
	writeByteToI2C(MPU_Gyro_Accel, 0x22, 0x00);
	writeByteToI2C(MPU_Gyro_Accel, 0x23, 0x00);
	writeByteToI2C(MPU_Gyro_Accel, 0x68, 0x00);
	writeByteToI2C(MPU_Gyro_Accel, 0x69, 0x00);
	writeByteToI2C(MPU_Gyro_Accel, 0x6A, 0x00);
	writeByteToI2C(MPU_Gyro_Accel, 0x6B, 0x00);

	//writeByteToI2C(MPU_Mag, 0x0A, 0x06);


}
