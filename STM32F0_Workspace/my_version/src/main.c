/**
 *****************************************************************************
 **
 **	 Quadcopter Flight
 **	 Spring 2016
 **
 **  Environment : Atollic TrueSTUDIO(R)
 **                STMicroelectronics STM32F4xx Standard Peripherals Library
 **
 *****************************************************************************
 **/

/* Includes */
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "IMU.h"
#include "arm_math.h"
#include "pwm_init.h"
#include "i2c_init.h"
#include "delay.h"

volatile int heartbeat_counter = 0;
volatile int heartbeat_color = 0;

uint8_t data[20] =
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/*
 * Accelerometer Data:
 */
int16_t accel_x;
int16_t accel_y;
int16_t accel_z;

float accel_x_normalized;
float accel_y_normalized;
float accel_z_normalized;

float angle_x_accel;
float angle_y_accel;

float angle_x_accel_last;
float angle_y_accel_last;

float accel_filter_level = .8; // percent last reading by between 0 and 1

/*
 * Gyroscope Data:
 */
int16_t gyro_x;
int16_t gyro_y;
int16_t gyro_z;

int16_t x_offset_sum;
int16_t y_offset_sum;

float gyro_x_normalized;
float gyro_y_normalized;
float gyro_z_normalized;

float gyro_x_normalized_last;
float gyro_y_normalized_last;

float angle_x_gyro = 0;
float angle_y_gyro = 0;

float gyro_filter_level = .7; // percent last reading by between 0 and 1

/*
 * Accelerometer and Gyroscope Combined:
 */
float angle_x_combined = 0;
float angle_y_combined = 0;

float percent_accel = .9; //percent of angle calculated from accelerometer between 0 and 1

/*
 * Motor Control Data:
 */
uint32_t throttle; // RAW CCRx Value: See notes on CCRx_throttle_val() for info

float throttle_value_perc; // PERCENTAGE: value between 0 and 100

int ch4level = 25000;
int ch3level = 25000;
int ch2level = 25000;
int ch1level = 25000;

arm_pid_instance_f32 PIDx;
arm_pid_instance_f32 PIDy;

//x and y angle desired for PID error calculation: 0 for hovering
float desired_x_angle = 0;
float desired_y_angle = 0;

//PID Constants for x and y
float xy_PID_kp;
float xy_PID_ki;
float xy_PID_kd;

//PID results
float pidx;
float pidy;

/*
 * Cycles the four LEDs every 1000th call for a heart beat
 */
void heartbeat() {
	if (heartbeat_counter < 500) {
		heartbeat_counter++;
	} else {
		heartbeat_counter = 0;
		if (heartbeat_color == 0) {
			heartbeat_color = 1;
			STM_EVAL_LEDOff(LED6);
			STM_EVAL_LEDOn(LED5);
		} else if (heartbeat_color == 1) {
			heartbeat_color = 2;
			STM_EVAL_LEDOff(LED5);
			STM_EVAL_LEDOn(LED3);
		} else if (heartbeat_color == 2) {
			heartbeat_color = 3;
			STM_EVAL_LEDOff(LED3);
			STM_EVAL_LEDOn(LED4);
		} else {
			heartbeat_color = 0;
			STM_EVAL_LEDOff(LED4);
			STM_EVAL_LEDOn(LED6);
		}
	}
}

/*
 * Initializes the PIDs for x and y coordinates.
 * input reset = 1 if resetting PID or 0 if maintaining current state
 * and only changing Kp, Ki, or Kd
 */
void pid_init(int reset) {
	PIDx.Kp = xy_PID_kp;
	PIDx.Ki = xy_PID_ki;
	PIDx.Kd = xy_PID_kd;

	PIDy.Kp = xy_PID_kp;
	PIDy.Ki = xy_PID_ki;
	PIDy.Kd = xy_PID_kd;

	if (reset == 1) {
		arm_pid_init_f32(&PIDx, 1);
		arm_pid_init_f32(&PIDy, 1);
	}
	if ((reset == 0) && (throttle != 23980)) {
		arm_pid_init_f32(&PIDx, 0);
		arm_pid_init_f32(&PIDy, 0);
	}
}

/*
 * Gyroscope has constant drift offsets that need to be calculated for accurate readings.
 * Reads 1000 gyroscope samples and calculates the offsets for x and y. Currently Run once at startup.
 */
void determine_gyro_offset_constants() {
	int samples = 1000;
	int sample_count = 0;
	int16_t x_offset = 0;
	int16_t y_offset = 0;
	for (sample_count = 0; sample_count <= samples; sample_count++) {
		get_data(data);
		y_offset = (int16_t) ((data[8] << 8) | (data[9]));
		y_offset_sum += y_offset;
		y_offset_sum = y_offset_sum / (int16_t) 2;
		x_offset = (int16_t) ((data[10] << 8) | (data[11]));
		x_offset_sum += x_offset;
		x_offset_sum = x_offset_sum / (int16_t) 2;
	}
}

/*
 * Initialize the ESCs and motors, which require 1-2 seconds of zero throttle.
 * All LEDs turn on to signal that motors are initializing.
 */
void init_motors(uint32_t delay_for_ms) {
	//ESCs need 1-2 seconds of 0% throttle (23980) to initialize
	throttle = 23980;
	TIM_SetCompare4(TIM1, throttle);
	TIM_SetCompare3(TIM1, throttle);
	TIM_SetCompare2(TIM1, throttle);
	TIM_SetCompare1(TIM1, throttle);

	//indicate that setup is complete and throttle is being held low for ESC initialization
	STM_EVAL_LEDOn(LED3);
	STM_EVAL_LEDOn(LED4);
	STM_EVAL_LEDOn(LED5);
	STM_EVAL_LEDOn(LED6);
	delay_ms(delay_for_ms);
	STM_EVAL_LEDOff(LED3);
	STM_EVAL_LEDOff(LED4);
	STM_EVAL_LEDOff(LED5);
	STM_EVAL_LEDOff(LED6);
}

/*
 * Init TIM2 and interrupt (NVIC) enable;
 */
void tim2_data_sample_timer_init() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	NVIC_InitTypeDef nvicStruct;
	nvicStruct.NVIC_IRQChannel = TIM2_IRQn;
	nvicStruct.NVIC_IRQChannelCmd = ENABLE;
	nvicStruct.NVIC_IRQChannelPreemptionPriority = 1;
	nvicStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvicStruct);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 999;
	timerInitStructure.TIM_Prescaler = 83;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

/*
 * Initializes all required STM32F4Discovery peripherals, as well as MCU6050.
 * One LED turns on per peripheral.  These can be used to debug if it gets stuck (GREEN = I2C).
 */
void init_peripherals() {
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);

	delay_init();

	STM_EVAL_LEDOn(LED6);

	i2c_init();

	STM_EVAL_LEDOff(LED6);
	STM_EVAL_LEDOn(LED3);

	tim1_pwm_init();

	STM_EVAL_LEDOff(LED3);
	STM_EVAL_LEDOn(LED5);

	pid_init(1);

	//user button config for kill switch:
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);

	STM_EVAL_LEDOff(LED5);
	STM_EVAL_LEDOn(LED4);

	i2c_init_gyro();

	//TODO: Need to check gyro offset constants; should increase accuracy
	determine_gyro_offset_constants();

	STM_EVAL_LEDOff(LED4);
}

/*
 * Process raw data from the 6050
 * 	 1. Process data and calculate angles from accelerometer
 *	 2. Process data and calculate angles from gyroscope
 *   3. Combine gyro and accel data
 */
void calculate_angle() {
	/*
	 *1. Data and calculations for accelerometer
	 */
	//a. combine high and low bits of raw data
	accel_x = (int16_t) ((data[0] << 8) | (data[1]));
	accel_y = (int16_t) ((data[2] << 8) | (data[3]));
	accel_z = (int16_t) ((data[4] << 8) | (data[5]));

	//b. normalize with constant from datasheet
	accel_x_normalized = (accel_x / 16384.0f);
	accel_y_normalized = (accel_y / 16384.0f);

	//c. store last result for complementary filter
	angle_x_accel_last = angle_x_accel;
	angle_y_accel_last = angle_y_accel;

	//d. some trig to find the angle
	angle_x_accel = (180.0f
			* atan(accel_x / sqrt(accel_y * accel_y + accel_z * accel_z))
			/ 3.14f);
	angle_y_accel = (180.0f
			* atan(accel_y / sqrt(accel_x * accel_x + accel_z * accel_z))
			/ 3.14f);

	//e. apply complementary filter
	angle_x_accel = (accel_filter_level) * angle_x_accel
			+ (1 - accel_filter_level) * angle_x_accel_last;
	angle_y_accel = (accel_filter_level) * angle_y_accel
			+ (1 - accel_filter_level) * angle_y_accel_last;

	/*
	 * 2. Data and calculations for gyroscope
	 */
	//a. combine high and low bits of raw data, apply offset calculated at system boot
	gyro_y = ((int16_t) ((data[8] << 8) | (data[9]))) - y_offset_sum;
	gyro_x = ((int16_t) ((data[10] << 8) | (data[11]))) - x_offset_sum;
	gyro_z = (int16_t) ((data[12] << 8) | (data[13]));

	//b. normalize with constant from datasheet
	gyro_x_normalized = (gyro_x / (131.0f * 2120.7f));
	gyro_y_normalized = (gyro_y / (131.0f * 2120.7f));

	//c. store last result for complementary filter
	gyro_x_normalized_last = gyro_x_normalized;
	gyro_y_normalized_last = gyro_y_normalized;

	//c. apply complementary filter
	if ((gyro_x_normalized > .001) | (gyro_x_normalized < -.001)) {
		angle_x_gyro -= (gyro_filter_level) * gyro_x_normalized
				+ (1 - gyro_filter_level) * gyro_x_normalized_last;
	}
	if ((gyro_y_normalized > .001) | (gyro_y_normalized < -.001)) {
		angle_y_gyro += (gyro_filter_level) * gyro_y_normalized
				+ (1 - gyro_filter_level) * gyro_y_normalized_last;
	}

	//d. if the gyro is getting too much drift, correct it by bouncing it towards the accelerometer
	if ((angle_x_gyro - angle_x_accel) > 2.0f) {
		angle_x_gyro -= 1.5f;
	}
	if ((angle_y_gyro - angle_y_accel) > 2.0f) {
		angle_y_gyro -= 1.5f;
	}
	if ((angle_x_gyro - angle_x_accel) < -2.0f) {
		angle_x_gyro += 1.5f;
	}
	if ((angle_y_gyro - angle_y_accel) < -2.0f) {
		angle_y_gyro += 1.5f;
	}

	/*
	 * 3. Combine the results from the gyro and accelerometer
	 */
	angle_x_combined = (percent_accel) * angle_x_accel
			+ (1 - percent_accel) * angle_x_gyro;
	angle_y_combined = (percent_accel) * angle_y_accel
			+ (1 - percent_accel) * angle_y_gyro;
}

/*
 * Should only be called at a frequency of 1kHz
 * controlled by TIM2 NVIC interrupt
 *
 * reads data from 6050
 * 1. reads 6050 data registers over I2C, stores results in data[] array
 * 2. processes the raw data from 6050 which calculates angle_x_combined
 * 3. input error into PIDS
 * 4. apply PID output to PWM levels
 */
void sample_and_calculate() {
	//1. read data from the 6050
	get_data(data);

	//2. assign data from accelerometer to variables
	calculate_angle();

	//3. feed angle into PID
	pidx = arm_pid_f32(&PIDx, (desired_x_angle - angle_x_combined));
	pidy = arm_pid_f32(&PIDy, (desired_y_angle - angle_y_combined));

	//4. calculate PWM CCR values for each motor with PID results and balancing offsets
	ch4level = throttle - pidx;
	ch3level = throttle + pidx;
	ch2level = throttle - pidy;
	ch1level = throttle + pidy;
}

/*
 * Sample 6050 and calculate angles at 1kHz; also drives heartbeat
 */
void TIM2_IRQHandler() {
	heartbeat();
	sample_and_calculate();
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

/*
 * Calculates the value to put in the CCRx register from a throttle percentage value
 * between 0 and 100.
 *
 * Throttle Notes:
 *		Theoretically:
 *		 		0% Throttle: PWM CCRx @ 23980 / 25000 (1.002ms pulse @ 40.7Hz)
 *				100% Throttle: PWM CCRx @ 22960 / 25000 (2.004ms pulse @ 40.7Hz)
 *				CCRx Throttle Range: 23980 - 22960 = 1020
 *
 * *NOTE that these calculations assume the PWM has the following settings:
 * 		TIM1 on APB2
 * 		Pre-scaler: 164
 * 		ARR: 25000
 */
int CCRx_throttle_val(float throttle_percentage) {
	//don't go over max throttle:
	//TODO: Can we increase the frequency of the PWM? Some ESCs can go up to 440Hz.
	int throttle_with_upper_bound = (23980 - (throttle_percentage * 9.0f));
	if ((throttle <= 22960) || (throttle_with_upper_bound <= 22960)) {
		return 22960;
	}
	return throttle_with_upper_bound;
}

/*
 * Does heavy lifting of throttling: gets IMU data, calculates the angle, feeds the error from desired into
 * PID controllers, then sets each motor to the correct PWM value.
 */
void set_throttle() {
	//NOTE chXlevel calculated by TIM2_IRQnHandler handler at freq of 1khz (same frequency as MPU6050);

	//set PWM CCR registers
	//TODO Turn motors on and off here for testing with current PID results:
	TIM_SetCompare4(TIM1, ch4level);
	TIM_SetCompare3(TIM1, ch3level);
	TIM_SetCompare2(TIM1, ch2level);
	TIM_SetCompare1(TIM1, ch1level);

	/*
	 * Uncomment the following and comment the above set of SetCompareX to test throttle levels
	 */
	//	TIM_SetCompare4(TIM1, throttle);
	//	TIM_SetCompare3(TIM1, throttle);
	//	TIM_SetCompare2(TIM1, throttle);
	//	TIM_SetCompare1(TIM1, throttle);
}

/*
 * Kill-switch
 * interrupt currently called by USER_BUTTON
 * If button has been pressed never exits and reset required to start again.
 */
void EXTI0_IRQHandler() {
	//if kill button hit shut off motors and loop
	STM_EVAL_LEDOff(LED3);
	STM_EVAL_LEDOff(LED4);
	STM_EVAL_LEDOff(LED5);
	STM_EVAL_LEDOff(LED6);

	TIM_SetCompare4(TIM1, 23980);
	TIM_SetCompare3(TIM1, 23980);
	TIM_SetCompare2(TIM1, 23980);
	TIM_SetCompare1(TIM1, 23980);

	STM_EVAL_LEDOn(LED5);
	STM_EVAL_LEDOn(LED6);
	while (1) {
		STM_EVAL_LEDToggle(LED3);
		STM_EVAL_LEDToggle(LED4);
		STM_EVAL_LEDToggle(LED5);
		STM_EVAL_LEDToggle(LED6);
		//TODO: Kill-switch is freezing here; was thinking toggle all the lights every half second
		//delay_ms(500) isn't looping; going to try delay with loop:
		volatile int i = 0;
		while (i < 5000) {
			i++;
		}
	}
}

/*
 *	1. Initialize Peripherals
 *	2. Hold ESC's at 0% throttle for at least 2 seconds with init_motors(x) where x>2000
 *	3. Start sampling timer, calls TIM2's handler at 1kHz
 *	4. Set PID  values
 *	5. Initialize PID
 *	6. Set Throttle %
 *	7. [Infinite loop] update PWMs
 */
int main(void) {
	//1
	init_peripherals();

	//2. ESCs need 1-2 seconds of 0 throttle to initialize
	init_motors(6000);

	//3. start sampling
	tim2_data_sample_timer_init();

	//4. PID Constants
	//TODO: Set PID Constants; can alter on the fly by changing and running pid_init(0);
	xy_PID_kp = .46;
	xy_PID_ki = .004;
	xy_PID_kd = 0;

	//5. initialize PID with fresh state:
	pid_init(1);

	//6. Throttle level between 0 and 100
	throttle_value_perc = 15;

	/* 7. Infinite loop */
	while (1) {
		//Runs this for on-the-fly PID tuning from STMStudio
		pid_init(0);

		//set PWM channels to throttle and applies most recent
		//PID output to the motors
		set_throttle();

		//calculate PWM CCR value from Throttle Percentage
		throttle = CCRx_throttle_val(throttle_value_perc);
	}
}

