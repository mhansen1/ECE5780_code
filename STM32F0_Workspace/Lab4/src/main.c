#include <stdio.h>
#include <stdlib.h>
#include <stm32f0xx.h>
#include "diag/Trace.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

/////////////////////////////////////////////
//
// FUNCTION DECLARATIONS
//
/////////////////////////////////////////////
void init_interrupts();
void init_gpio();
void init_timer();
void init_usart();

//////////////////////////////////////////////
//
// VARIABLE DECLARATIONS
//
/////////////////////////////////////////////
volatile uint8_t btn_count; // Stores the number of times the button was pressed.
volatile uint8_t data_in; // Data that is read into USART1.
volatile uint8_t blink_count; // Number of times that the LED needs to blink;


//////////////////////////////////////////////
//
// INTERUPT ROUTINES
//
/////////////////////////////////////////////
// External interrupt 0 interrupt handler.
void EXTI0_1_IRQHandler(void) {
	if(GPIOA->IDR&0x00000001)  {// If the button is pressed...
		GPIOC->ODR |= (1 << 9); // Turn on the green LED.
		btn_count++; // Increase the counter for the button pressed.
	}
	else // If the button is not pressed...
		GPIOC->ODR &= ~(1 << 9); // Turn off the  LED.
		
	EXTI->PR |= 0x00000001; // Clear the pending interrupt bit.
}

void USART1_IRQHandler(void) {
	GPIOC->ODR |= (1 << 8);  // Turn on the blue LED.
	data_in = USART1->RDR; // Read the USART1 RX data.
	USART1->RQR |= (1 << 3); // Clear the interrupt flag.
}

void SysTick_Handler(void) {
	// If all blinks aren't done, and the LED is not on....
	if((data_in > 0) && !(GPIOC->IDR&(1<<8))) {
		GPIOC->ODR |= (1 << 9); // Turn on the LED.
		data_in--; // Decrement the number of times to blink.
	}
	else {
		GPIOC->ODR &= ~(1 << 9); // Else, turn off the LED.
	}
}

void TIM2_IRQHandler(void) {
	GPIOC->ODR &= ~(1 << 8); // Turn off the blue LED.
	USART1->TDR = btn_count;// Start transferring the USART1 TX data.
	// Set the button press count to zero
	btn_count = 1;
	TIM2->SR &= 0xFFFE;
}

int
main(int argc, char* argv[])
{
	btn_count = 0;
	blink_count = 0;
	data_in = 0;

	init_gpio();
	init_interrupts();
	init_timer();
	
	while (1) {
		;
	}
}

//////////////////////////////////////////////
//
// BOARD INITIALIZATIONS
//
/////////////////////////////////////////////
void init_gpio() {
	// Set up the button
	RCC->AHBENR |= 0x000A0000; // Enable the GPIO clock for port A and C.
	
	// Set up the LEDs
	GPIOC->MODER &= 0xFFF0FFFF; // Reset GPIO mode for PC8 and PC9.
	GPIOC->MODER |= 0x00050000; // Set the mode values for PC8 and PC9 to 01 (output).
	GPIOC->ODR &= ~(3 << 8); // Turn off the green and blue LEDs.
	GPIOC->ODR |= (1 << 8); // Turn on the blue LED.
}

void init_interrupts() {

	// Set up external interrupt 0.
	SYSCFG->EXTICR[1] &= 0xFFF0; // Sets the source for external interrupt 0.
	EXTI->RTSR |= 0x00000001; // Set the interrupt to the rising edge.
	EXTI->FTSR |= 0x00000001; // Set the interrupt to the falling edge, as well.
	EXTI->IMR |= 0x00000001; // Unmask EXTI0 interrupt.
	NVIC->ISER[0] |= 0x00000020; // Enable the EXTI0 interrupt in the vector.
	//NVIC->IP[1] |= (1 << 8); // Set the priority of EXTI0 to 1.

	// Enable timer 2 interrupt
	NVIC->ISER[0] |= 0x00008000; 
	
	// Enable USART1 interrupt.
	NVIC->ISER[0] |= (1 << 27);
}

void init_timer() {

	// Setup SysTic
	SysTick->CTRL |= 0x00000003;
	SysTick->LOAD = 0x005B8D80;
	SysTick->VAL = 1;

	// Start timer 2
	RCC->APB1ENR |= 0x00000001; // Enable TIM2 clock.
	TIM2->SR &= 0xFFFE; // Make sure that the interrupt flag is cleared.
	TIM2->PSC = 8000; // Set the timer clock frequency.
	TIM2->ARR = 20000; // Timer duration.
	TIM2->DIER = 0x0001; // Enable updated interrupt.
	TIM2->CR1 |= 0x0015; // Set to count down mode, and enable timer.
}

void init_usart() {
	// USART1 is on GPIO pins PA8(CK), PA9(TX), PA10(RX), PA11(CTS), and PA12(RTS)
	// Enable the clock for the USART1.
	RCC->APB2ENR |= (1 << 14);
	// Set the USART1 pins to alternative function mode.
	GPIOA->MODER &= ~(0x01550000); // 0000 0001 0101 0101 0000 0000 0000 0000
	GPIOA->MODER |= 0x02AA0000; // 0000 0010 1010 1010 0000 0000 0000 0000
	// Set the alternate function of PA8 and PA9 to USART1_TX and USART1_RX
	GPIOA->AFR[1] &= ~(0x00000EE0); // 0000 0000 0000 0000 0000 1110 1110 0000
	GPIOA->AFR[1] |= 0x00000110; // 0000 0000 0000 0000 0000 0001 0001 0000
	// Set the USART1 pins to high speed, because why not?
	GPIOA->OSPEEDR |= 0x03FF0000; // 0000 0011 1111 1111 0000 0000 0000 0000
	// Set up USART control registers
	USART1->CR1 |= 0x0000002C; // Enable the RXNE interrupt, and enable RX, and TX, 0010 1100
	// Set the baud rate.
	USART1->BRR = 0x00001388;
	// Set the prescaler to 1.
	USART1->GTPR &= ~(0x000000FE);
	USART1->GTPR |= (1 << 0);
	// Enable the USART1.
	USART1->CR1 |= (1 << 0); 
}

#pragma GCC diagnostic pop
