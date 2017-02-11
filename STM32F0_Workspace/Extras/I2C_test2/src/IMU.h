#ifndef IMU_H_
#define IMU_H_

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "arm_math.h"
//#include "I2C.h"
#include "i2c_init.h"
#include "delay.h"

#define coeff_gyro 0.007633587786f // 1/(binary->decimal)
#define coeff_accel 0.000598571777f // g/(binary->decimal)
#define I2C_addr 0x68

// Data from IMU
uint8_t raw_data[20];

// IMU raw data as 16 bits
int16_t x_accel;
int16_t y_accel;
int16_t z_accel;
int16_t x_gyro;
int16_t y_gyro;
int16_t z_gyro;

// Matrix form of data.
// As a note on the accelerometer, the Z-axis is positive in the downward direction.
// In other words, in a neutral position, the Z acceleration is 1g.
arm_matrix_instance_f32 angle_mat;
arm_matrix_instance_f32 accel_mat;
arm_matrix_instance_f32 vel_mat;
arm_matrix_instance_f32 pos_mat;
arm_matrix_instance_f32 gyro_offset_mat;
arm_matrix_instance_f32 accel_offset_mat;
float32_t angle_data[3];
float32_t accel_data[3];
float32_t vel_data[3];
float32_t pos_data[3];
float32_t gyro_offset[3];
float32_t accel_offset[3];

// Create a temporary matrix for an intermediate place holder.
// Creating it in the init to avoid the work of making the matrix
// over and over again.
arm_matrix_instance_f32 temp1_mat;
arm_matrix_instance_f32 temp2_mat;
float32_t temp1_data[3];
float32_t temp2_data[3];

// Variables for running average filter.
// const float32_t num_imu_samples = 200;
float32_t accel_avg_x[300];
float32_t accel_avg_y[300];
float32_t accel_avg_z[300];

// Constants
float32_t dt; // Time between updates
uint32_t imu_update_count;
arm_matrix_instance_f32 g_mat;
float32_t g_vec[3];
float32_t g;

// Initialize the IMU. Will calibrate offsets and start the IMU in the correct mode.
void init_imu(uint32_t dt);

// Updates the the kinetic data (linear and rotational data) using the IMU.
void imu_update();

// Finds the magnitude of a 3 element array.
float32_t get_mag(arm_matrix_instance_f32 *vector);

// Convert a 3 element matrix from the body frame of reference to the earth's.
void body_to_earth(arm_matrix_instance_f32 *result, arm_matrix_instance_f32 *angles, arm_matrix_instance_f32 *vector_data);

// Convert a 3 element matrix from the earth frame of referenc to the body's.
void earth_to_body(arm_matrix_instance_f32 *result, arm_matrix_instance_f32 *angles, arm_matrix_instance_f32 *vector_data);

// Interrupt handler for timer 5
void TIM5_IRQHandler(void);

// Starts the IMU by starting the interrupt timer.
void start_imu();

// Stop the IMU by stopping the interrupt timer.
void stop_imu();

// Pull the IMU data.
void get_imu();

#endif
