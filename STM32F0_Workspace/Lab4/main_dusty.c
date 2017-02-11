#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "stm32f0xx.h"
#include "delay.h"      // Provides delay functions using the SysTick timer


/* -------------------------------------------------------------------------------------------------------------
 * USART Functions and Interrupt
 *
 * Sends and receives TTL-RS232 data, sets the blinkCount variable used in the main loop
 * -------------------------------------------------------------------------------------------------------------
 */

volatile uint8_t blinkCount;

// Initializes GPIO and USART for use in TTL-RS232
void USART_init(uint32_t baud) {

    /// TODO: Configure some GPIO pins to act as the transmit (TX) and receive (RX) for USART1

    /* Hint: The GPIO Alternate function registers (AFRL & AFRH) are declared as an array.
             GPIO_AFRL is listed as AFR[0] in stm32f0xx.h
             If you read the lab, you'll get a major hint as to what pins to use...
     */
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;					//enable b port
	RCC->APB2ENR |= RCC_APB2Periph_USART1;
    GPIOB->MODER |= GPIO_MODER_MODER6_1;               // Set PB6 to alternate function
    GPIOB->MODER &= ~(GPIO_MODER_MODER6_0);
    GPIOB->AFR[6] &= GPIO_AF_0;							//reset AF Register to USART

    GPIOB->MODER |= GPIO_MODER_MODER7_1;               // Set PB7 to alternate function
    GPIOB->MODER &= ~(GPIO_MODER_MODER7_0);
    GPIOB->AFR[7] &= GPIO_AF_0;							//reset AF Register to USART




    /// TODO: Configure USART1 according to the serial specification below

    /* Specification:
            1 Start bit, 8 Data bits, 1 Stop bit
            Oversampling by 16
            No parity control
            Enable the RXNE interrupt (receive buffer not empty)
            Least significant bit transmitted first
            No hardware flow control
    */
    //USART_TypeDef* us;
    //us->CR1 =

    //USART_Init()
    USART1->CR1 |= USART_CR1_RXNEIE;		//Enable interrupt
    USART1->CR1 |= USART_CR1_TE; // Enable TXE
    USART1->CR1 |= USART_CR1_RE; // enable read


    /* Hint: Many of the specification parameters are default, we just want you thinking
             about them.
             You will need to separately enable the transmitter and receiver portions
             of the USART. (both are initially disabled for power saving)

             DON'T initially enable the entire USART (USART_CR1_UE) until after you're
             done configuring the entire thing!
             Use a separate statement at the end of the function
     */

    /// TODO: Set the BAUD rate to the rate specified by the init function argument

    USART1->BRR = SystemCoreClock/baud;
    USART1->CR1 |= USART_CR1_UE;


    /* Hint: The baud rate register takes a prescaler value that divides down
             the system clock. The equation is simply BRR = <System Clock>/<BAUD rate>;
             You may NOT use the auto baud rate feature of the USART!

             Remember that the system clock is defined as a variable in system_stm32f0xx.h
     */

    // Enable USART RX ISR
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_SetPriority(USART1_IRQn, 2); // Set to medium priority
}

	// Writes a char/byte to the USART transmit register, blocks until the USART is ready
	void USART_putByte(uint8_t byte) {
    /* Hint: Double check the set/reset conditions of the flags in the status register
             Some things are active-high and others are active-low
             You'll want to make a while loop that waits until the proper flag is in the
             proper state.
     */

    /// TODO: Wait until transmit buffer of USART is empty
    /// TODO: Write a byte into the Transmit data register
		while(!(USART1->ISR & (1<<7))){;}

		USART1->TDR = byte;
	}

// USART1 global interrupt, should be configured to fire on data recieve
void USART1_IRQHandler(void) {
    /* Hint: The USART is one of the few peripherals that knows when you've read data automatically.
             Since we only have the RX interrupt enabled the pending flag will automatically be handled
             when we read from the receive register. (Check the flag conditions in the status register!)
     */

    /// TODO: Read value from USART Receive data register
    /// TODO: Multiply value from USART by 2 and write into 'blinkCount' global variable
	uint8_t byte = USART1->RDR;
	byte = byte * 2;
	blinkCount = byte;

}


/* -------------------------------------------------------------------------------------------------------------
 *  Button Initialization and Interrupt
 *
 * Configures PA0 with an external IRQ, counts the number of button presses and sends on USART
 * -------------------------------------------------------------------------------------------------------------
 */

volatile uint8_t pressCount;

// Used for IRQ debouncing, the button interrupt disables itself for a while until the SysTick timer
// calls this function to re-enable it.
void reenable_IRQ(void) {
    NVIC_EnableIRQ(EXTI0_1_IRQn); // Re-enable button interrupt
}

void  button_init(void) {
    // Initialize PA0 for button input
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;                                          // Enable peripheral clock to GPIOA
    GPIOA->MODER &= ~(GPIO_MODER_MODER0_0 | GPIO_MODER_MODER0_1);               // Set PA0 to input
    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR0_0 | GPIO_OSPEEDR_OSPEEDR0_1);     // Set to low speed
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_1;                                        // Set to pull-down

    // Initialize EXTI for rising-edge interrupt on PA0
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;   // Enable peripheral clock for SYSCFG
    SYSCFG->EXTICR[0]  &= ~(0x000F);            // Clear the lowest 4 bits in the EXTI input MUX (Select PA0)
    EXTI->RTSR = EXTI_RTSR_TR0;                 // Set interrupt trigger to rising edge
    EXTI->IMR = EXTI_IMR_MR0;                   // Un-mask EXTI0 line
    NVIC_SetPriority(EXTI0_1_IRQn, 3);          // Set priority to low
    NVIC_EnableIRQ(EXTI0_1_IRQn);               // Enable EXTI0_1 interrupt in NVIC

    SysTick_setCallback(&reenable_IRQ);         // Register function pointer with SysTick for callback
    pressCount = 0;
}

void EXTI0_1_IRQHandler(void) {
    EXTI->PR |= EXTI_PR_PR0;        // Acknowledge interrupt in EXTI
    NVIC_DisableIRQ(EXTI0_1_IRQn);  // Disable IRQ as form of debounce, it will be re-enabled later by SysTick callback
    pressCount++;
    USART_putByte(pressCount);      // Send pressCount out USART as hexadecimal string
}


/* -------------------------------------------------------------------------------------------------------------
 *  Miscellaneous Initialization Functions
 *  -------------------------------------------------------------------------------------------------------------
 */

void LED_init(void) {
    // Initialize PC8 and PC9 for LED's
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;                                          // Enable peripheral clock to GPIOC
    GPIOC->MODER |= GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;                  // Set PC8 & PC9 to outputs
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);                    // Set to push-pull output type
    GPIOC->OSPEEDR &= ~((GPIO_OSPEEDR_OSPEEDR8_0 | GPIO_OSPEEDR_OSPEEDR8_1) |
                        (GPIO_OSPEEDR_OSPEEDR9_0 | GPIO_OSPEEDR_OSPEEDR9_1));   // Set to low speed
    GPIOC->PUPDR &= ~((GPIO_PUPDR_PUPDR8_0 | GPIO_PUPDR_PUPDR8_1) |
                      (GPIO_PUPDR_PUPDR9_0 | GPIO_PUPDR_PUPDR9_1));             // Set to no pull-up/down
    GPIOC->ODR &= ~(GPIO_ODR_8 | GPIO_ODR_9);                                   // Shut off LED's
}

/* C-style strings are actually character arrays terminated with a null char '\0'
   To define a string constant: (null char at end is added automatically for double-quoted stuff)
       char stringName[] = "Initial Text";
   If you want an editable string you'll need to set an array length and keep in mind
   not to overstep the array bounds. You'll also need to add a null character to the end
   unless you are using double-quote syntax "".

   This function deals with strings as a pointer, which gets incremented along
   the string data until the null character is encountered.
 */

// Sends a string on the USART
void USART_putString(char* string) {
    while(*string) { // Dereferences pointer, exits if finds the null character
        USART_putByte(*string);
        string++; // Move pointer to next character in string
    }
}


/* -------------------------------------------------------------------------------------------------------------
 * Main Program Code
 *
 * Starts initialization of peripherals
 * Blinks green LED (PC9) in loop, also blinks blue LED (PC8) if blinkCount variable is set
 * -------------------------------------------------------------------------------------------------------------
 */

int main(int argc, char* argv[]) {

    delay_init();       // Initialize delay and SysTick timer
    LED_init();         // Initialize PC8 and PC9 to output
    button_init();      // Initialize PA0 to input, and enable EXTI interupt
    USART_init(9600);   // Initialize USART TX/RX pins and USART to 9600 BAUD TTL-RS232
                        // (The BAUD is the bit-rate used to determine the speed of the communication)

    while (1) {
    	USART_putString("ay yo what up");
        GPIOC->ODR ^= GPIO_ODR_9;       // Blink green LED
        if(blinkCount) {                // If blinkCount is nonzero also blink the blue LED
            GPIOC->ODR ^= GPIO_ODR_8;
            blinkCount--;
        }
        delay_ms(125);                  // Delay 1/8 second
    }
}
