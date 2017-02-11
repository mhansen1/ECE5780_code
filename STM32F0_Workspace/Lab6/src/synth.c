
/* -------------------------------------------------------------------------------------------------------------
 *  Analog-to-Digital and Digital-to-Analog Functions
 *  -------------------------------------------------------------------------------------------------------------
 */

#include "synth.h"

/* -------------------------------------------------------------------------------------------------------------
 *  Internal-Use Global Variable Declarations
 *
 *  Not included in header file, they aren't visible outside of this compilation unit
 *  -------------------------------------------------------------------------------------------------------------
 */

/* Multidimensional array of sampled waveforms
 * Each array contains a single wave cycle
 */
const uint8_t wave_tables[4][32] = { \
    {127,151,175,197,216,232,244,251,254,251,244,232,216,197,175,151,127,102,78,56,37,21,9,2,0,2,9,21,37,56,78,102}, \
    {0,15,31,47,63,79,95,111,127,142,158,174,190,206,222,238,254,238,222,206,190,174,158,142,127,111,95,79,63,47,31,15}, \
    {254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, \
    {0,7,15,23,31,39,47,55,63,71,79,87,95,103,111,119,127,134,142,150,158,166,174,182,190,198,206,214,222,230,238,246}};

// Selected waveform and current position in cycle
uint8_t cycle_pos;
waveType_t sel_wave;


/* -------------------------------------------------------------------------------------------------------------
 *  Waveform Syntheziser API Funtions
 *  -------------------------------------------------------------------------------------------------------------
 */

// Initializes the waveform synthesizer
void synth_init() {
    // Initialize global variables
    cycle_pos = 0;
    sel_wave = SQUARE;

    // Initialize components of the waveform generator
    ADC_init();
    DAC_init();
    TIM6_init();
}

// Converts ADC value and updates frequency output
void synth_updateFrequency(void) {
	
	/// TODO: Read the count from the ADC and write to data variable
	
	/* Hint: The ADC and DAC have multiple data registers, each with 
	 *       different characteristics. You'll need to choose the 
	 *       appropriate one for the mode you've configured.
	 */
	
	uint8_t data = (uint8_t)(ADC1->DR);

    // Apply a linear transformation to convert between ADC counts and timer reload value
    TIM6->ARR = (((data*2250)>>8)+750);
}

/* Manually sets frequency of output (500-2000Hz)
 * Returns -1 if input out of bounds, returns 0 if success
 */
int8_t synth_setFrequency(uint16_t freq) {
    // Check to see if input is within bounds, return -1 otherwise
    if(freq > 2000 || freq < 50) {
        return -1;
    }
    // Apply a linear transformation to convert between Hz and timer reload value
    TIM6->ARR = (((freq*2250)/1500)+750);
    return 0;
}

// Changes the synthesized output type
void synth_setWave(waveType_t wave) {
    sel_wave = wave;    // Change index used to select portions of the wave table
}


/* -------------------------------------------------------------------------------------------------------------
 *  Internal-Use Functions
 *  -------------------------------------------------------------------------------------------------------------
 */

// Initialize the ADC peripheral
void ADC_init() {
	
	/// TODO: Configure an ADC input pin
	
	/* Hint: Note that PA0 is already used by the button, don't try to use that channel
	 *       You will need to remember what ADC channel your pin uses so you can 
	 *       configure the ADC to use it.
	 *
	 *       Remember that the ADC and DAC are analog peripherals
	 */
	 // Use PA1 for the ADC.
	 RCC->AHBENR |= (1 << 17); // Make sure that the clock to port A is enabled.
	 GPIOA->MODER |= (3 << 2); // Set PA1 to analog function mode.
	 
    /// TODO: Configure the ADC to 8-bit, free-run mode (no trigger), asynchronous clock

    /* Hint: You will notice that the ADC talks about trigger modes/selection
	 *       Don't enable any trigger modes, we just want the ADC to continuously run 
	 *       Don't enable the ADC yet, you'll need to run a calibration first!
	 */
	 RCC->APB2ENR |= (1 << 9); // Start the clock for the ADC
	 ADC1->CFGR1 &= ~(1 << 3); // Set to 8-bit resolution.
	 ADC1->CFGR1 |= (2 << 3);
	 ADC1->CFGR1 |= (1 << 13); // Set to continuous mode.
	 ADC1->CFGR2 &= ~(3 << 30); // Make sure that the clock is asynchronous.
	 ADC1->CHSELR |= (1 << 1); // Select ADC input 1.
	 
	/// TODO: Start an ADC self-calibration cycle, wait until it's complete  
	 	
	/* Hint: Pay close attention to the polarity of the flag signals used! 
	 *       Read the bit descriptions carefully! The ADC has a few conditions
	 *       that need to be fulfilled for calibration and startup.
	 *
	 *       Not all flags are in status registers, the ADC sometimes will modify
	 *       control bits to let you know what's happening.
	 */
	 ADC1->CR &= ~(1 << 0); // Make sure that the ADC is not enabled.
	 ADC1->CR |= (1 << 31); // Start the calibration.
	 while(ADC1->CR & (1 << 31)) {;} // Wait until the ADC is done calibrating.

	/// TODO: Enable the ADC and wait until it's ready to run
	ADC1->CR |= (1 << 0); // Enable the ADC.
	while(!(ADC1->ISR&(1<<0)))  {;} // Wait until the ADC is ready.
	
	/// TODO: Signal/Trigger the start of the continuous conversion
	ADC1->CR |= (1 << 2); // Start the ADC conversion.
	
}


// Initialize the DAC peripheral
void DAC_init() {
	
	/// TODO: Configure an DAC output pin
	
	/* Hint: Remember that the ADC and DAC are analog peripherals
	 *       Once a pin is connected to the analog bus, the other
	 *       GPIO settings don't have any effect.
	 */
	 // Use DAC1 (PA4)
	 RCC->AHBENR |= (1 << 17); // Make sure that the clock to Port A is enabled.
	 GPIOA->MODER |= (3 << 8); // Set PA4 to analog mode.

	/// TODO: Configure the DAC to 8-bit, no trigger mode
	
    /* Hint: You will notice that the DAC talks about trigger modes/selection
	 *       Don't enable any trigger modes, the DAC will automatically update
	 *       whenever we write to its data register.
	 *
	 *       In the basic configuration that we are using the DAC is probably
	 *       the simplest peripheral to configure, don't over-think the config register.
	 */	
	 RCC->APB1ENR |= (1 << 29); // Enable the clock to the DAC.
	 DAC->CR &= ~(15 << 8); // Set MAMP1 to 255 amplitude.
	 DAC->CR |= (7 << 8);
	 DAC->CR &= ~(2 << 1); // Make sure that there is no trigger for DAC1.
	 DAC->CR |= (1 << 0); // Enable DAC
	
}

// Initialize the timer feeding the DAC with data
void TIM6_init() {
    // Configure the timer to fire an ISR on update event
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->ARR = 3000; // Equals 2000 Hz
    TIM6->DIER |= TIM_DIER_UIE;
    TIM6->CR1 |= (TIM_CR1_ARPE | TIM_CR1_CEN);

    NVIC_EnableIRQ(TIM6_DAC_IRQn);
    NVIC_SetPriority(TIM6_DAC_IRQn,2);
}

// ISR moves new data into DAC register
void TIM6_DAC_IRQHandler() {
    // Write value from wave table into DAC

    if(++cycle_pos > 31) {
        cycle_pos = 0;
    }
	uint8_t data = wave_tables[sel_wave][cycle_pos];
    
	/// TODO: Write data into the appropriate DAC data register
	
    /* Hint: The ADC and DAC have multiple data registers, each with 
	 *       different characteristics. You'll need to choose the 
	 *       appropriate one for the mode you've configured.
	 */
	 DAC->DHR8R1 = data;

    GPIOC->ODR ^= GPIO_ODR_8;
    TIM6->SR &= ~TIM_SR_UIF;
}
