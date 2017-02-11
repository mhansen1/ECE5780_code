#ifndef LED_DISP_H_
#define LED_DISP_H_

#include "stm32f4xx.h"
#include <stdlib.h>
#include "delay.h"

/*
 * Initialize the LCD display
 *
 * Parameters:
 * 	Inputs:
 * 		char port - Character of the port that corresponds to the pins (A, B, C, D, etc...)
 * 		uint16_t pins - The pins that are used for data transfer and commands
 * 	Outputs:
 * 		N/A
 */
void disp_init(char port, uint16_t _pins);


/*
 * Write command to the LCD display
 *
 * Parameters:
 * 	Inputs:
 * 		uint8_t data - The command to perform.
 * 	Outputs:
 * 		N/A
 */
void disp_wrt_com(uint32_t data);

/*
 * Write data to be written on the LCD display
 *
 * Parameters:
 * 	Inputs:
 * 		uint8_t data - The character to be written to the display.
 * 	Outputs:
 * 		N/A
 */
void disp_wrt_dat(uint8_t data);










#endif
