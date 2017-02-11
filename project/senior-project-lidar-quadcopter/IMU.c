#include "IMU.h"

// _dt is the change in time in milliseconds.
void init_imu(uint32_t _dt) {	
	// Initialize all variables;
	dt = (float32_t)(_dt*0.001); // Scale dt from milliseconds to seconds.
	
	// Initialize matrices
	uint8_t i=0;
	for(i; i < 3; i++) {
		angle_data[i] = 0;
		accel_data[i] = 0;
		vel_data[i] = 0;
		pos_data[i] = 0;
		temp1_data[i] = 0;
		temp2_data[i] = 0;
		g_vec[i] = 0;
	}
	g_vec[2] = g;

	arm_mat_init_f32(&angle_mat,3,1,(float32_t *)angle_data);
	arm_mat_init_f32(&accel_mat,3,1,(float32_t *)accel_data);
	arm_mat_init_f32(&vel_mat,3,1,(float32_t *)vel_data);
	arm_mat_init_f32(&pos_mat,3,1,(float32_t *)pos_data);
	arm_mat_init_f32(&g_mat,3,1,(float32_t *)g_vec);
	arm_mat_init_f32(&temp1_mat,3,1,(float32_t *)temp1_data);
	arm_mat_init_f32(&temp2_mat,3,1,(float32_t *)temp2_data);
	
	// Get the data and find out the orientation using gravity.
	// Because the accelerometer can be noisy, gather the data multiple times
	// and average the results.
	// NOTE: THIS STILL NEEDS TO BE DOUBLE CHECKED IN SOMEHING LIKE MATLAB TO 
	//            MAKE SURE THE RESULTS ARE CORRECT!
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
	angle_data[0] += (float32_t)(x_gyro*coeff_gyro*PI*dt)/(180.0f);
	angle_data[1] += (float32_t)(y_gyro*coeff_gyro*PI*dt)/(180.0f);
	angle_data[2] += (float32_t)(z_gyro*coeff_gyro*PI*dt)/(180.0f);
	
	accel_data[0] = (float32_t)(x_accel*coeff_accel);
	accel_data[1] = (float32_t)(y_accel*coeff_accel);
	accel_data[2] = (float32_t)(z_accel*coeff_accel);
	// Remove the gravitational component from the acceleration data.
	// First, but acceleration into the earth frame of reference.
	body_to_earth(&temp1_mat,&angle_mat,&accel_mat);
	// Then subtract the gravity from the acceleration. 
	arm_mat_sub_f32(&temp1_mat, &g_mat, &accel_mat);
	
	// Get velocity from acceleration
	vel_data[0] += accel_data[0]*dt;
	vel_data[1] += accel_data[1]*dt;
	vel_data[2] += accel_data[2]*dt;
	
	// Get position from velocity.
	pos_data[0] += accel_data[0]*dt*dt*0.5;
	pos_data[1] += accel_data[1]*dt*dt*0.5;
	pos_data[2] += accel_data[2]*dt*dt*0.5;
}

/*
	Find the magnitude of a vector.
*/
void get_mag(float32_t *vector, float32_t *result) {
	float32_t temp = 0; 
	uint8_t i=0;
	for(i;i<3;i++)
		temp += vector[i]*vector[i];
	arm_sqrt_f32(temp,result);
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
	float32_t temp36 = vector_data->pData[2];
	float32_t temp24 = result->pData[2];
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
		GPIOD->ODR ^= (1 << 13);
		GPIOD->ODR ^= (1 << 14);
	}
	TIM5->SR &= ~TIM_SR_UIF;
}


void start_imu() {
	TIM5->CR1 |=  (1 << 0); // Start the timer.
}

void stop_imu() {
	TIM5->CR1 &= ~(1 << 0); // Stop the timer.
}

void get_imu() {
	I2C1_read(I2C_addr, 0x3B, 14, raw_data);
}


































