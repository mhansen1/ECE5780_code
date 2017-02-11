#include <stdio.h>
#include <stdlib.h>
#include <stm32f0xx.h>
#include "diag/Trace.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void init_interrupts();
void init_gpio();
void init_timer();

// External interrupt 0 interrupt handler.
void EXTI0_1_IRQHandler(void) {
	GPIOC->ODR ^= (3 << 8);
	EXTI->PR |= 0x00000001; // Clear the pending interrupt bit.
}

void SysTick_Handler(void) {
	//GPIOC->ODR ^= (3 << 8);
	while(1) {;}
}

void TIM2_IRQHandler(void) {
	GPIOC->ODR ^= (3 << 8);
	TIM2->SR &= 0xFFFE;
}

int
main(int argc, char* argv[])
{

	init_gpio();
	init_interrupts();

	while (1) {
		;
	}
}

void init_gpio() {
	RCC->AHBENR |= 0x000A0000; // Enable the GPIO clock for port A and C.
	GPIOC->MODER &= 0xFFF0FFFF; // Reset GPIO mode for PC8 and PC9.
	GPIOC->MODER |= 0x00050000; // Set the mode values for PC8 and PC9 to 01 (output).
	GPIOC->ODR |= 0x00000100; // Put the signals 90 degrees out of phase.
	GPIOC->ODR &= 0x11111D11;
}

void init_interrupts() {

	// Set up external interrupt 0.
	SYSCFG->EXTICR[1] &= 0xFFF0; // Sets the source for external interrupt 0.
	EXTI->RTSR |= 0x00000001; // Set the interrupt to the rising edge.
	EXTI->FTSR &= 0xFFFFFFFE; // Make sure that it's not trigger on the falling edge
	EXTI->IMR |= 0x00000001; // Unmask EXTI0 interrupt.
	NVIC->ISER[0] |= 0x00000020; // Enable the EXTI0 interrupt in the vector.
	NVIC->IP[1] |= (1 << 8); // Set the priority of EXTI0 to 1.

	// Enable timer 2 interrupt
	NVIC->ISER[0] |= 0x00008000; // Enable timer 2 interrupt in the NVIC.
	NVIC->IP[3] |= 0x40000000;
	//NVIC_SetPriority(15,4);
}

void init_timer() {

	// Setup timer 2
	SysTick->CTRL |= 0x00000003;
http://www.atollic.com/resources/video-tutorials/	SysTick->LOAD = 0x005B8D80;
	SysTick->VAL = 1;

	// Start the SysTick timer
	RCC->APB1ENR |= 0x00000001; // Enable TIM2 clock.
	TIM2->SR &= 0xFFFE; // Make sure that the interrupt flag is cleared.
	TIM2->PSC = 8000; // Set the timer clock frequency.
	TIM2->ARR = 750; // Timer duration.
	TIM2->DIER = 0x0001; // Enable updated interrupt.
	
	TIM2->CR1 |= 0x0015; // Set to count down mode, and enable timer.
}

#pragma GCC diagnostic pop
