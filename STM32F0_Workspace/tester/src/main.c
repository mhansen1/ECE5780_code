#include <stdio.h>
#include <stdlib.h>
#include "stm32f0xx.h"
#include "diag/Trace.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

volatile uint32_t period;
uint32_t T_timer_clock;
volatile uint32_t wave_pos;
const uint8_t wave_table[32] = 
	{127,151,175,197,216,232,244,251,254,251,244,232,216,197,175,151,127,102,78,56,37,21,9,2,0,2,9,21,37,56,78,102};

void init_ADC();
void init_DAC();
void init_gpio();
void init_pwm();
void init_time();

void TIM2_IRQHandler(void) {
	GPIOC->ODR ^= (1 << 9);
	DAC->DHR8R1 = wave_table[wave_pos];
	if(wave_pos == 31)
		wave_pos = 0;
	else
		wave_pos++;
	TIM2->SR &= 0xFFFE;
}

int main(int argc, char* argv[])
{
	T_timer_clock = 4000000;
	wave_pos = 0;
	
	init_gpio();
	init_pwm();
	init_ADC();
	init_DAC();
	while (1)  {
		TIM3->CCR3 = (uint32_t)(ADC1->DR);
		period = 20*(ADC1->DR);
		TIM2->ARR = T_timer_clock*period/32;
		//DAC->DHR8R1 = (uint32_t)(ADC1->DR);
	}
}


void init_gpio() {
	RCC->AHBENR |= 0x000A0000; // Enable clock on port A and C.
	// Set the green LED (PC9) to GPIO output.
	GPIOC->MODER &= ~(1 << 19); 
	GPIOC->MODER |= (1 << 18);
	GPIOC->ODR &= ~(1 << 9); // Turn off the green LED.
	// Set the blue LED (PC8) to GPIO alternate function.
	GPIOC->MODER &= ~(1 << 16);
	GPIOC->MODER |= (1 << 17);
	GPIOC->AFR[1] &= ~(15 << 0);
	// Set up ADC0 (PA0)
	GPIOA->MODER |= (3 << 2); // Set PA0 to analog mode.
	// Set up DAC1 (PA4)
	GPIOA->MODER |= (3 << 8); // Set to analog mode.
	
	
	return;
}

void init_DAC() {
	RCC->APB1ENR |= (1 << 29);
	DAC->CR &= ~(15 << 8);
	DAC->CR |= (7 << 8);
	DAC->CR &= ~(2 << 1);
	DAC->CR |= (1 << 0);
	return;
}

void init_ADC() {
	RCC ->APB2ENR |= (1 << 9); // Start the clock for the ADC.
	ADC1->CFGR1 &= ~(1 << 3); // Set to 8-bit resolution.
	ADC1->CFGR1 |= (2 << 3);
	ADC1->CFGR1 |= (1 << 13); // Set to continuous mode.
	ADC1->CFGR2 &= ~(3 << 30); // Make sure that the clock is asynchronous.
	ADC1->CHSELR |= (1 << 1); // Select ADC input 0.
	ADC1->CR &= ~(1 << 0); // Make sure that the ADC is not enabled.
	ADC1->CR |= (1 << 31); // Start the calibration.
	while(ADC1->CR & (1 << 31)) {;} // Wait until the ADC is done calibrating.
	ADC1->CR |= (1 << 0); // Enable the ADC.
	while(!(ADC1->ISR&(1<<0)))  {;} // Wait until the ADC is ready.
	ADC1->CR |= (1 << 2); // Start the ADC conversion.
	return;
}

void init_pwm() {
	RCC->APB1ENR |= (1 << 1);
	
	TIM3->PSC = 47;
	TIM3->ARR = 256;
	TIM3->CCR3 = 0;
	TIM3->CCMR2 &= ~(3 << 0);
	TIM3->CCMR2 &= ~(1 << 4);
	TIM3->CCMR2 |= (3 << 5);
	TIM3->CCMR2 |= (1 << 3);
	TIM3->CCER |= (1 << 8);
	//TIM3->BDTR |= TIM_BDTR_MOE;
	TIM3->CR1 |= (1 << 0);
	TIM3->EGR |= (1 << 0);
	return;
}

void init_time() {
	RCC->APB1ENR |= (1 << 0);
	TIM2->CNT = 1;
	TIM2->PSC = 11; // Timer clock. 48MHz/12=4MHz (250ns)
	TIM2->ARR = 0;
	TIM2->DIER |= 0x0001;
	TIM2->CR1 |= 0x0015;
	return;
}



#pragma GCC diagnostic pop











