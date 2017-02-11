
#ifndef SYNTH_H_
#define SYNTH_H_

#include <stdlib.h>
#include "stm32f0xx.h"

// Defines waveform output types
typedef enum {
    SINE = 0,
    TRIANGLE,
    SQUARE,
    SAWTOOTH,
} waveType_t;

// Initializes the waveform synthesizer
void synth_init(void);

// Converts ADC value and updates frequency output
void synth_updateFrequency(void);

/* Manually sets frequency of output (500-2000Hz)
 * Returns -1 if input out of bounds, returns 0 if success
 */
int8_t synth_setFrequency(uint16_t freq);

// Changes the synthesized output type
void synth_setWave(waveType_t wave);


// Internally used functions
static void ADC_init(void);
static void DAC_init(void);
static void TIM6_init(void);


#endif /* SYNTH_H_ */