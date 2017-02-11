#define stub (uint16_t)0x0000; // my stub. You should not end up with any of these!

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f0xx.h"


// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


volatile uint32_t D; // Duty cycle of TIM3 PWM.

/*=====================================================================================
	Private Functions
=======================================================================================*/

/*
 * Initializes PC8 in Output Mode
 */
void LED_BLUE_init(void) {

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 				// Enable peripheral clock to GPIOC
	GPIOC->MODER |= GPIO_MODER_MODER8_0; 			// Set PC8 to output
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8);			// Set to push-pull output type
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR8_0		// Set to low speed
		| GPIO_OSPEEDR_OSPEEDR8_1);
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8_0 			// Set to no pull-up/down
		| GPIO_PUPDR_PUPDR8_1);
	GPIOC->ODR &= ~(GPIO_ODR_8);					// Shut off LED
}


/*
 * Initializes PC9 in Output Mode
 */
void LED_GREEN_init(){

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 				// Enable peripheral clock to GPIOC
	GPIOC->MODER |= GPIO_MODER_MODER9_0; 			// Set PC9 to output
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_9);			// Set to push-pull output type
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR9_0		// Set to low speed
		| GPIO_OSPEEDR_OSPEEDR9_1);
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR9_0 			// Set to no pull-up/down
		| GPIO_PUPDR_PUPDR9_1);
	GPIOC->ODR &= ~(GPIO_ODR_9);					// Shut off LED
}


/*
 * Initializes PC8 in Alternate Function Mode
 *
 * Table 13 in the STM32F0-Discovery Datasheet
 * shows PC8 having one alternative function
 * attached to TIM3 Channel 3
 */
void LED_BLUE_AF_init(){

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 				// Enable peripheral clock to GPIOC
	GPIOC->MODER |= (1 << 17); 			// Set PC8 to AF mode
	//GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8);			// Set to push-pull output type
	//GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR8_0 	// Set to low speed
	//	| GPIO_OSPEEDR_OSPEEDR8_1);
	//GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8_0 			// Set to no pull-up/down
	//	| GPIO_PUPDR_PUPDR8_1);
	GPIOC->AFR[1] &= ~(15 << 0);				//alternate function 0 on pin 8
}


/*
 *  Initializes PC8 in Alternate Function Mode
 *
 * Table 13 in the STM32F0-Discovery Datasheet
 * shows PC9 having one alternative function
 * attached to TIM3 Channel 4
 */
void LED_GREEN_AF_init(){

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 				// Enable peripheral clock to GPIOC
	GPIOC->MODER |= GPIO_MODER_MODER9_1; 			// Set PC8 to AF mode
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_9);			// Set to push-pull output type
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR9_0 	// Set to low speed
		| GPIO_OSPEEDR_OSPEEDR9_1);
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR9_0 			// Set to no pull-up/down
		| GPIO_PUPDR_PUPDR9_1);
	GPIOC->AFR[1] &= ~(GPIO_AFRH_AFR9);				//alternate function 0 on pin 9
}


/*
 * A basic general purpose timer that is capable
 * of throwing an interrupt every millisecond.
 *
 * We'll be using TIM2 for this timer. Note that
 * TIM2 has a 32 bit counter, so would not normally
 * be used for something as simple as this, but it
 * suits our purposes.
 */
void millis_timer_init(){

/* TODO: Connect TIM2 to the appropriate bus */
	RCC->APB1ENR |= (1 << 0);

/* TODO: Set ARR and PSC registers */
	TIM2->CNT = 1; // Make sure that the counter isn't some unknown number.
	TIM2->ARR = 500000;
	TIM2->PSC = 47;

/* TODO: Enable UEV interrupt */
	TIM2->CR1 &= ~(1 << 1); // Make sure the UEV is enabled.
	TIM2->DIER |= 0x0001;
	
/* TODO: Enable timer */
	TIM2->CR1 |= (1<<0);

/* TODO: Finally, lets enable the TIM2 IRQ before we exit */
	NVIC->ISER[0] |=  0x00008000; 

}


/*
 * TODO: TIM2 ISR
 *
 * Be sure to clear the update interrupt flag.
 * It's located in the Status Register (SR)
 */
void TIM2_IRQHandler(void) {
	GPIOC->ODR ^= (1 << 9);
	//if(TIM3->CCR3 > 1000)
		//TIM3->CCR3 = 1;
	//else
		//TIM3->CCR3 = TIM3->CCR3 + 50;
	TIM2->SR &= 0xFFFE;
}

/*
 * Enable TIM3 in PWM mode. Timer 3 has 4 usable Channels.
 * If we look at the STM32F0-Discovery Datasheet
 * at Table 13 we'll see that Channel 3 is connected to PC8
 * and Channel 4 is connected to PC9. You're free to connect
 * whatever LED you'd like to this PWM signal, but note that
 * the bits you manipulate here will need to reflect the channel
 * you are using.
 *
 * PC8: TIM3 CH3
 * PC9: TIM3 CH4
 */
void TIM3_PWM_mode(){

/* 	TODO: Connect TIM3 to the appropriate bus */
	RCC->APB1ENR |= (1 << 1);

/*	TODO: Set the appropriate time base registers
		  like you did for TIM2
*/
	TIM3->PSC = 47; // Set the counter clock to 1 MHz
	TIM3->ARR = 1000; // Set the auto reload register to a rate of 20 kHz
	TIM3->CCR3 = 678;
	TIM3->CCMR2 &= ~(3 << 0); // Make sure channel 3 is output.
	TIM3->CCMR2 &= ~(1 << 4); // Set channel 3 to PWM mode 1.
	TIM3->CCMR2 |= (3 << 5);
	TIM3->CCMR2 |= (1 << 3); // Enable pre-load for channel 3.
	//TIM3->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE);
	TIM3->CCER |= (1 << 8); // Enable compare output.
	TIM3->BDTR |= TIM_BDTR_MOE;
	TIM3->CR1 |= (1 << 0); // Auto-reload pre-load enable.
	TIM3->EGR |= (1 << 0); // Update generation.
	
	
/* Create a new OCInitTypeDef object.

	The TIM_OCInitTypeDef struct is used
	as a parameter to the TIM_OCxInit()
	functions. Every member needs a value
	before it can be sent through the function. */
//	TIM_OCInitTypeDef  TIM_OCInitStructure;


/*	TODO: Fill in the values of the members in the
	 	  TIM_OCINitTypeDef struct. Looking at the
	   	  comments for this struct in the stm32f0xx_tim.h
		  file will be very helpful. I've filled in several
		  of the less useful members for you.

		  By defining these members we are able to instantiate
		  PWM on multiple channels with minimal effort. If you
		  look at the TIM_OCxInit() functions in the
		  stm32f0xx_tim.c file, you'll notice that the bits for
		  some of the registers are shifted in reference to
		  the channel you'd like to configure. This is a very
		  useful feature.
 */

/*	The type of output compare mode. This member is analagous
	to Bits [6:4] in the CCMR2. We'd like to use
	PWM Mode 1 here. */
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	/* TODO */

/*	Bits for CCER. The Output Compare signal is output on the
 	corresponding output pin, but is shifted for you in the
 	initiation function. Check out the function if you need
 	a better idea. */
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputNState_Enable; /* TODO */

/*	Maps to CCRx. This is the value that will control your
	duty cycle in relation to ARR and PSC */
//	TIM_OCInitStructure.TIM_Pulse = 50;	/* TODO */
	
/*	Bit 2 in CCER. Reserved, always read as zero. */
//	TIM_OCInitStructure.TIM_OutputNState = (uint16_t) 0;

/* 	Output polarity- We want an active low polarity.
	You don't need to do anything here. The value will be
	shifted left accordingly when you call the initiation function */
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

/* 	This bit is only used in TIM1 - just keep it zero */
//	TIM_OCInitStructure.TIM_OCNPolarity = (uint16_t) 0;

/*	This bit is only used in TIM1 - just keep it zero */
//	TIM_OCInitStructure.TIM_OCIdleState = (uint16_t) 0;

/* 	This bit is only used in TIM1 - just keep it zero */
//	TIM_OCInitStructure.TIM_OCNIdleState = (uint16_t) 0;

/* 	TODO: Call the corresponding TIM_OCxInit() function.
   	   	  Remember to include all parameters and to
   	   	  dereference your struct pointer parameter. */
//	TIM_OC3Init(TIM3,TIM_OCInitStructure);
	
/*	TODO: Finally let's enable the TIM3 counter */
	//TIM3->CR1 |= (1 << 0);

}


/*=====================================================================================
	Main Entry Point to Program
=======================================================================================*/
int main(void)
{
	/* TODO: For the first part of lab we'll blink an LED at 1Hz. You can choose either LED.
	 * Set up Timer 2 to be a millisecond timer. The outline to the function is
	 * described above. You might need another variable floating around your program
	 * to keep track of elapsed time.
	 */
	millis_timer_init();
	//LED_BLUE_init();
	LED_GREEN_init();
	GPIOC->ODR |= (1 << 8);
	

	/* TODO: The next part of the lab asks you to instantiate a PWM signal and use it to control
	 * the brightness of the other LED. You'll have to fill in some functions above. Because the LED
	 * is now attached to Timer 3, we'll have to configure the pin in Alternate Function mode
	 */
	D = 25;
	//LED_GREEN_AF_init();
	LED_BLUE_AF_init();
	TIM3_PWM_mode();


	/* TODO: For the final part of lab we'll demonstrate the PWM signal by pulsing the LED.
	 * You can do it anyway you want, just have it pulse on and off indefinitely.
	 */


	while(1){} // loop indefinitely
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
