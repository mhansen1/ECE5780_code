#include "IMU.h"

const float32_t num_imu_samples = 300;
float32_t run_avg[3];

// _dt is the change in time in milliseconds.
// During testing, it seems that the best speed that can be obtained
// is 3ms. Any slower and the processor gets stuck reading the I2C.
// This is probably because there isn't enough time to read all the
// data before the timer runs out.
void init_imu(uint32_t _dt) {	

	#ifdef DELAY_H_
		if((SysTick->CTRL & (1 << 0)) == 0)
			delay_init();
	#endif
	// Initialize all variables;
	dt = (float32_t)(_dt*0.001); // Scale dt from milliseconds to seconds.
	
	// Initialize matrices
	uint16_t i=0;
	for(i; i < 3; i++) {
		angle_data[i] = 0;
		accel_data[i] = 0;
		vel_data[i] = 0;
		pos_data[i] = 0;
		temp1_data[i] = 0;
		temp2_data[i] = 0;
		g_vec[i] = 0;
		gyro_offset[i] = 255; // Initialize to a high number. Want to try and get the offset to converge to a small number when calibrating.
		accel_offset[i] = 255;
	}
	g_vec[2] = 9.8; // Set gravity in the matrix.

	arm_mat_init_f32(&angle_mat,3,1,(float32_t *)angle_data);
	arm_mat_init_f32(&accel_mat,3,1,(float32_t *)accel_data);
	arm_mat_init_f32(&vel_mat,3,1,(float32_t *)vel_data);
	arm_mat_init_f32(&pos_mat,3,1,(float32_t *)pos_data);
	arm_mat_init_f32(&g_mat,3,1,(float32_t *)g_vec);
	arm_mat_init_f32(&temp1_mat,3,1,(float32_t *)temp1_data);
	arm_mat_init_f32(&temp2_mat,3,1,(float32_t *)temp2_data);
	arm_mat_init_f32(&gyro_offset_mat,3,1,(float32_t *)gyro_offset);
	arm_mat_init_f32(&accel_offset_mat,3,1,(float32_t *)accel_offset);

	// Set all of the moving average values to 0.
	for(i = 0; i < num_imu_samples; i++) {
		accel_avg_x[i] = 0;
		accel_avg_y[i] = 0;
		accel_avg_z[i] = 0;
	}

	// SETUP THE IMU REGISTERS
	uint8_t send_data = 0x01;
	// Reset The signals
	writeByteToI2C(I2C_addr, 0x01, send_data);
	delay_ms(3);
	// Reset the gyro signal paths
	send_data = 0x07;
	writeByteToI2C(I2C_addr, 0x68, send_data);
	delay_ms(3);
	// Set the low pass filter for the accelerometer to a bandwidth of 460Hz
	send_data = 0x08;
	writeByteToI2C(I2C_addr, 0x1D, send_data);
	delay_ms(3);
	// Make sure that the IMU isn't in sleep mode or anything.
	send_data = 0x00;
	writeByteToI2C(I2C_addr,0x6B,send_data);
	delay_ms(3);
	
	// Get the offsets for the accelerometer.
	for(i = 0; i < 2000; i++) {
		get_imu();
		x_accel += (int16_t)((raw_data[0] << 8) | raw_data[1]);
		y_accel += (int16_t)((raw_data[2] << 8) | raw_data[3]);
		z_accel += (int16_t)((raw_data[4] << 8) | raw_data[5]); // Remove gravity
		delay_ms(3);
	}
	accel_offset[0] = (float32_t)(x_accel*coeff_accel/2000);
	accel_offset[1] = (float32_t)(y_accel*coeff_accel/2000);
	accel_offset[2] = (float32_t)(z_accel*coeff_accel/2000 - g);

	// This is meant to calibrate the accelerometer offset. Basically, keep sampling until the offset converges
	// to an acceptable range.
	// NOT WORKING YET!
//	i = 1;
//	while(((accel_offset[0] > 0.01) || (accel_offset[0] < -0.01)) &&
//		  ((accel_offset[1] > 0.01) || (accel_offset[1] < -0.01)) &&
//		  ((accel_offset[2] > 0.01) || (accel_offset[2] < -0.01))) {
//		get_imu();
//		x_accel += (int16_t)((raw_data[0] << 8) | raw_data[1]);
//		y_accel += (int16_t)((raw_data[2] << 8) | raw_data[3]);
//		z_accel += (int16_t)(((raw_data[4] << 8) | raw_data[5])); // Remove gravity
//		accel_avg_x[0] += (float32_t)(x_accel*coeff_accel);
//		accel_avg_y[0] += (float32_t)(y_accel*coeff_accel);
//		accel_avg_z[0] += (float32_t)(z_accel*coeff_accel - g);
//		accel_offset[0] = accel_avg_x[0]/i;
//		accel_offset[1] = accel_avg_y[0]/i;
//		accel_offset[2] = accel_avg_z[0]/i;
//		i++;
//	}

	// Find the offset for the gyros by averaging 2000 samples..
	for(i = 0; i < 2000; i++) {
		get_imu();
		x_gyro += (int16_t)((raw_data[8] << 8) | raw_data[9]);
		y_gyro += (int16_t)((raw_data[10] << 8) | raw_data[11]);
		z_gyro += (int16_t)((raw_data[12] << 8) | raw_data[13]);
		delay_ms(3);
	}
	x_gyro = x_gyro/2000;
	y_gyro = y_gyro/2000;
	z_gyro = z_gyro/2000;
	gyro_offset[0] = (float32_t)((x_gyro*coeff_gyro*PI*dt)/(180.0f));
	gyro_offset[1] = (float32_t)((y_gyro*coeff_gyro*PI*dt)/(180.0f));
	gyro_offset[2] = (float32_t)((z_gyro*coeff_gyro*PI*dt)/(180.0f));

	// Get the data and find out the orientation using gravity.
	// Because the accelerometer can be noisy, gather the data multiple times
	// and average the results.
	// NOTE: THIS STILL NEEDS TO BE DOUBLE CHECKED IN SOMEHING LIKE MATLAB TO 
	//            MAKE SURE THE RESULTS ARE CORRECT!
	// Magnetometer might be a better choice
	/*
	for(uint32_t i=0;i<10;i++) {
		get_data(raw_data);
		x_accel = (int16_t)((raw_data[0] << 8) | raw_data[1]);
		y_accel = (int16_t)((raw_data[2] << 8) | raw_data[3]);
		z_accel = (int16_t)((raw_data[4] << 8) | raw_data[5]);
		// Can find the angle by taking the inverse cosine using measured force and
		// the force of gravity (gravity is the hypotenuse of the triangle).
		angle_data[0] += (float32_t)acosf((float32_t)(x_accel/(coeff_accel*g)));
		angle_data[1] += (float32_t)acosf((float32_t)(y_accel/(coeff_accel*g)));
		angle_data[2] += (float32_t)acosf((float32_t)(z_accel/(coeff_accel*g)));
	}
	arm_mat_scale_f32(&angle_mat, 0.1, &angle_mat);
	*/
	
	// Set up the timer to read from the IMU and calculate data.
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; // Enable TIM5 clock.
	TIM5->PSC = 20999; // Set time clock to 250us.
	TIM5->ARR = (_dt*4)-1; // Set period of timer.
	TIM5->EGR |=  TIM_EGR_UG; // Set update generation
	TIM5->SR &= ~TIM_SR_UIF; // Make sure interrupt flag is cleared.
	TIM5->DIER |= TIM_DIER_UIE; // Enable interrupt for TIM5.
	NVIC->ISER[1] |= (1 << 18); // Enable TIM5 in NVIC.

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= (1 << 26) | (1 << 28);
	GPIOD->ODR |= (1 << 13);
	GPIOD->ODR |= (1 << 14);

	imu_update_count = 0;
	run_avg[0] = 0;
	run_avg[1] = 0;
	run_avg[2] = 0;
}

/*
	Update the kinematic variables. 
	This gets the raw data over I2C and calculates the 
*/
void imu_update() {
	// Collect the raw data from the IMU and put it into it's full variables
	get_imu();
	x_accel = (int16_t)((raw_data[0] << 8) | raw_data[1]);
	y_accel = (int16_t)((raw_data[2] << 8) | raw_data[3]);
	z_accel = (int16_t)((raw_data[4] << 8) | raw_data[5]);
	x_gyro = (int16_t)((raw_data[8] << 8) | raw_data[9]);
	y_gyro = (int16_t)((raw_data[10] << 8) | raw_data[11]);
	z_gyro = (int16_t)((raw_data[12] << 8) | raw_data[13]);
	
	// Scale gyro data using constant (based on the range that is set, see data sheet to 
	// know for sure what the conversion constant is), then convert from degrees (default) to radians
	// Need to multiply by dt to change from degrees/sec to degrees.
	float32_t dAngle[3];
	dAngle[0] = (float32_t)((x_gyro*coeff_gyro*PI*dt)/(180.0f) - gyro_offset[0]);
	dAngle[1] = (float32_t)((y_gyro*coeff_gyro*PI*dt)/(180.0f) - gyro_offset[1]);
	dAngle[2] = (float32_t)((z_gyro*coeff_gyro*PI*dt)/(180.0f) - gyro_offset[2]);
	
	temp2_data[0] = (float32_t)(x_accel*coeff_accel);
	temp2_data[1] = (float32_t)(y_accel*coeff_accel);
	temp2_data[2] = (float32_t)(z_accel*coeff_accel);

	angle_data[0] += dAngle[0];
	angle_data[1] += dAngle[1];
	angle_data[2] += dAngle[2];

	// Remove the gravitational component from the acceleration data.
	// First, put acceleration into the earth frame of reference.
	body_to_earth(&temp1_mat,&angle_mat,&temp2_mat);
	// Then subtract the gravity from the acceleration. 
	arm_mat_sub_f32(&temp1_mat, &g_mat, &temp2_mat);

	// Perform a running average filter. Replace the oldest value with the newest
	accel_avg_x[imu_update_count] = temp2_data[0] - accel_offset[0];
	accel_avg_y[imu_update_count] = temp2_data[1] - accel_offset[1];
	accel_avg_z[imu_update_count] = temp2_data[2] - accel_offset[2];

	// Add the new value to the running average.
	// STILL NEED TO TEST! GOING TO REPLACE SUMMING LOOP!
	//run_avg[0] += accel_avg_x[imu_update_count];
	//run_avg[1] += accel_avg_y[imu_update_count];
	//run_avg[2] += accel_avg_z[imu_update_count];

	imu_update_count++;
	if(imu_update_count == num_imu_samples)
		imu_update_count = 0;

	// Subtract the oldest value from the running average.
	// STILL NEED TO TEST! GOING TO REPLACE SUMMING LOOP!
	//run_avg[0] -= accel_avg_x[imu_update_count];
	//run_avg[1] -= accel_avg_y[imu_update_count];
	//run_avg[2] -= accel_avg_z[imu_update_count];

	uint32_t i;
	/*
	for(i = 0; i < num_imu_samples-1; i++) {
		accel_avg_x[i] = accel_avg_x[i+1];
		accel_avg_y[i] = accel_avg_y[i+1];
		accel_avg_z[i] = accel_avg_z[i+1];
	}
	accel_avg_x[199] = temp2_data[0];
	accel_avg_y[199] = temp2_data[1];
	accel_avg_z[199] = temp2_data[2];
	*/

	// Reset the acceleration data.
	temp2_data[0] = 0;
	temp2_data[1] = 0;
	temp2_data[2] = 0;
	// Sum the averaging arrays
	for(i = 0; i < num_imu_samples; i++) {
		temp2_data[0] += accel_avg_x[i];
		temp2_data[1] += accel_avg_y[i];
		temp2_data[2] += accel_avg_z[i];
	}
	// Divide by the number of samples to get the average.
	// Try to filter out when rotational movement (right now only interested in linear movement for accelerometer).
	if((dAngle[0] > 0.01) || (dAngle[1] > 0.01) || (dAngle[2] > 0.01) ||
	   (dAngle[0] < -0.01) || (dAngle[1] < -0.01) || (dAngle[2] < -0.01)) {
		accel_data[0] = 0;
		accel_data[1] = 0;
		accel_data[2] = 0;
	}
	else {
		accel_data[0] = temp2_data[0]/num_imu_samples;
		accel_data[1] = temp2_data[1]/num_imu_samples;
		accel_data[2] = temp2_data[2]/num_imu_samples;
	}
	
	// Get velocity from acceleration
	vel_data[0] += accel_data[0]*0.5*dt;
	vel_data[1] += accel_data[1]*0.5*dt;
	vel_data[2] += accel_data[2]*dt;
	
	// Get position from velocity.
	pos_data[0] += vel_data[0]*0.5*dt;
	pos_data[1] += vel_data[1]*0.5*dt;
	pos_data[2] += vel_data[2]*0.5*dt;

}

/*
	Find the magnitude of a vector.
*/
float32_t get_mag(arm_matrix_instance_f32 *vector) {
	float32_t result, temp = 0;
	uint8_t i=0;
	for(i;i<3;i++)
		temp += vector->pData[i]*vector->pData[i];
	arm_sqrt_f32(temp,&result);
	return result;
}

/*
	Convert a vector from the body from of reference to the earth frame of reference.
*/
void body_to_earth(arm_matrix_instance_f32 *result, arm_matrix_instance_f32 *angles, arm_matrix_instance_f32 *vector_data) {
	// Get the sine and cosine of the angles.
	float32_t roll_cos,
			  roll_sin,
			  pitch_cos,
			  pitch_sin,
			  yaw_cos,
			  yaw_sin;
	roll_cos = arm_cos_f32(angles->pData[0]);
	roll_sin = arm_sin_f32(angles->pData[0]);
	pitch_cos = arm_cos_f32(angles->pData[1]);
	pitch_sin = arm_sin_f32(angles->pData[1]);
	yaw_cos = arm_cos_f32(angles->pData[2]);
	yaw_sin = arm_sin_f32(angles->pData[2]);

	
	// Create the transformation matrix
	arm_matrix_instance_f32 R;
	float32_t R_data[9] = {
						   yaw_cos*pitch_cos, yaw_cos*roll_sin*pitch_sin - roll_cos*yaw_sin, roll_sin*yaw_sin + roll_cos*yaw_cos*pitch_sin,
						   pitch_cos*yaw_sin, roll_cos*yaw_cos + roll_sin*yaw_sin*pitch_sin, roll_cos*yaw_sin*pitch_sin - yaw_cos*roll_sin,
						   -pitch_sin,                pitch_cos*roll_sin,                                               roll_cos*pitch_cos,
						   };
	arm_mat_init_f32(&R, 3, 3, (float32_t *)R_data);
	
	// Compute the transformation.
	arm_mat_mult_f32(&R,vector_data,result);
	return;
}

/*
	Convert a vector from the earth frame of reference to body frame of reference.
*/

void earth_to_body(arm_matrix_instance_f32 *result, arm_matrix_instance_f32 *angles, arm_matrix_instance_f32 *vector_data) {
	// Get the sine and cosine of the angles.
	float32_t roll_cos,
			  roll_sin,
			  pitch_cos,
			  pitch_sin,
			  yaw_cos,
			  yaw_sin;
	roll_cos = arm_cos_f32(angles->pData[0]);
	roll_sin = arm_sin_f32(angles->pData[0]);
	pitch_cos = arm_cos_f32(angles->pData[1]);
	pitch_sin = arm_sin_f32(angles->pData[1]);
	yaw_cos = arm_cos_f32(angles->pData[2]);
	yaw_sin = arm_sin_f32(angles->pData[2]);
	
	// Create the transformation matrix	
	arm_matrix_instance_f32 R;
	float32_t R_data[9] = {
						   yaw_cos*pitch_cos,                                            pitch_cos*yaw_sin,                                             -pitch_sin,
						   yaw_cos*roll_sin*pitch_sin - roll_cos*yaw_sin, roll_cos*yaw_cos + roll_sin*yaw_sin*pitch_sin, pitch_cos*roll_sin,
						   roll_sin*yaw_sin + roll_cos*yaw_cos*pitch_sin, roll_cos*yaw_sin*pitch_sin - yaw_cos*roll_sin, roll_cos*pitch_cos,
						   };
	arm_mat_init_f32(&R, 3, 3, (float32_t *)R_data);
	
	// Compute the transformation.
	arm_mat_mult_f32(&R,vector_data,result);
	return;
}


void TIM5_IRQHandler(void) {
	// Check to see if interrupt has occured.
	// No idea why it's needed for this board, but it doesn't work unless the
	// UIF bit is checked.
	if((TIM5->SR & TIM_SR_UIF) != 0) {
		imu_update();
		//GPIOD->ODR ^= (1 << 13);
		//GPIOD->ODR ^= (1 << 14);
	}
	TIM5->SR &= ~TIM_SR_UIF;
}

/*
 * Start the timer for the IMU.
 */
void start_imu() {
	TIM5->CR1 |=  (1 << 0); // Start the timer.
}

/*
 * Stop the timer for the IMU.
 */
void stop_imu() {
	TIM5->CR1 &= ~(1 << 0); // Stop the timer.
}

/*
 * Function to get the data from the IMU
 */
void get_imu() {
	//I2C1_read(I2C_addr, 0x3B, 14, raw_data);
	get_data(raw_data);
}


































