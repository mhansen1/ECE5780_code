//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stm32f0xx.h>
#include "diag/Trace.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F0 empty sample (trace via $(trace)).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the $(trace) output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


void TIM2_IRQHandler(void) {
  GPIOC->ODR ^= 0x00000300; // Change the state of the two LEDs.
  TIM2->SR &= 0xFFFE;
}

uint8_t button_state;
int
main(int argc, char* argv[])
{

  char btn_state = 0; // Use this the for debouncing the button. Use the LSB to tell if the
                      // button has been pressed (after doing the debounce). The second LSB
	                  // will be used as the "debounce state".
				      // Third bit used to say whether the time delay has been changed since being pushed.
  uint32_t count = 0; // Use to keep track of debounce time.
  uint32_t bounce_time; // Debounce time.
  
  if(RCC->CFGR & 0x00000003) { // Using 48MHz clock. Use for debounce time length.
	  bounce_time = 100000; // Pipeline is 3 stages, so 3/48e6=62.5ns. Assuming about 8
  }                        // instructions in the loop (randomly chosen), gets about 500ns
                           // per increment of count, so this is about a 50ms time.
  else {
	  bounce_time = 1000000; // Make long wait time to signal that 48MHz clock isn't being
  }                          // used. This was more my way of testing out the clock source
	                         // than anything else.
							 
  /////////////////////
  //
  // SETUP TIMER
  //
  //////////////////////
  uint32_t delay_time = 400;
  RCC->APB1ENR |= 0x00000001; // Enable TIM2 clock.
  TIM2->SR &= 0xFFFE; // Make sure that the interrupt flag is cleared.
  TIM2->PSC = 8000; // Set the timer clock frequency.
  TIM2->DIER = 0x0001; // Enable updated interrupt.
  TIM2->ARR = delay_time; // Timer duration.
  TIM2->CR1 |= 0x0015; // Set to count down mode, and enable timer.
  NVIC->ISER[0] |= 0x00008000; // Enable timer 2 interrupt in the NVIC.


  RCC->AHBENR |= 0x000A0000; // Enable the GPIO clock for port A and C.
  GPIOC->MODER &= 0xFFF0FFFF; // Reset GPIO mode for PC8 and PC9.
  GPIOC->MODER |= 0x00050000; // Set the values for PC8 and PC9 to 01.
  GPIOC->ODR |= 0x00000100; // Put the signals 90 degrees out of phase.
  GPIOC->ODR &= 0x11111D11;
 
  while (1) {
	///////////////////
	//
	// Change LED with button
	//
	////////////////////
	/**	
	  if(btn_state & 0x01) { // If the button is in a pressed state, light up the blue LED and turn the green one off.
		  GPIOC->ODR |= 0x00000100;
		  GPIOC->ODR &= 0x11111D11;
	  }
	  else { // Else, turn the blue LED off and the green one on.
		  GPIOC->ODR |= 0x00000200;
		  GPIOC->ODR &= 0x11111E00;
	  }
	  
	  count++; // Increment the counter.
	  if(count == bounce_time) { // If it's time to check the bounce...
		  if(GPIOA->IDR&0x00000001) { // If the button is pressed
			if(btn_state&0x02) // If the button "state" (second LSB), then make sure that the button is one.
				btn_state |= 0x01; // The button is indeed pushed down, set the button state appropriately.
			else
				btn_state |= 0x02; // The button state is not "pressed", but the actual button is, so set the state to "pressed"
		  }                                  // but don't update the state of the LEDs (need to wait for debounce period first).
		  else { // If the button is not pressed.
			  if(btn_state&0x02) // If the button state says that the button is pressed...
				btn_state &= 0xFD; // Set the button state to say that the button is not pressed  (set to 0).
			  else
				 btn_state &= 0xFE; // The button has not been pressed for the debounce period, so set the button state to 0.
		  }
		  count = 0; // Reset the counter.
	  }
	  **/
	  
	  ///////////////////////
	  //
	  // Change LED speed with button.
	  //
	  //////////////////////

	  if(!(btn_state & 0x04) && (btn_state & 0x02)) { // If the button hasn't been pressed, and the state after debounce is done..
	    if(delay_time > 10000) // If max delay time is reached, reset back to original value.
			delay_time = 400;
		else // Else, increase the delay time.
			delay_time += 2000;
	    TIM2->ARR = delay_time; // Set the delay time to the timer register.
		btn_state |= 0x04; // Set "flag" saying that the button has been pushed. This is to make sure that the delay time
	  }                    // doesn't change during the same button push.
	  else if(!(btn_state&0x02)) { // If the button is confirmed to not be pressed, make sure that the button push
		btn_state &= 0xFB;         // can be pushed again and register as a time delay increase.
	  }

	  count++;
	  if(count == bounce_time) { // If time to check bounce...
	    char current = (char)GPIOA->IDR&0x00000001; // Get the current value of the button.
	    if(current == (btn_state&0x01)) { // If the current state is equal to the initial state...
	    	if(current) // If button is pressed...
	    		btn_state |= 0x02; // Set the final state to 1.
	    	else // If button is not pressed.
	    		btn_state &= 0xFD; // Set the final state to 0.
	    }
		else { // If the current state is not equal to the initial state.
			if(current) // If button is pressed...
				btn_state |= 0x01; // Set the initial state to 1.
			else // Else...
				btn_state &= 0xFE; // Set the initial state to 0.
		}
	    count = 0;
	  }
	  button_state = (uint8_t)GPIOA->IDR&0x00000001;
	  
  }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
