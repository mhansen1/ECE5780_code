#include <stdio.h>
#include <stdlib.h>
#include "delay.h"
#include "led_init.h"
#include "LED_disp.h"

//#define addr1 ((volatile uint32_t *)0x40020C00)
//#define addr2 ((volatile uint32_t *)0x40020C14)
//#define addr(x) ((volatile uint32_t *)x)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

/*
	1. Write if data or command.
	2. Set read or write.
	3. Delay for 60ns.
	4. Set enable high.
	5. Delay 50ns.
	6. Write data.
	7. Delay 50ns.
	8. Write enable low.
	9. Delay 340ns
*/

/*
	PE7 - DB0
	PE8 - DB1
	PE9 - DB2
	PE10 - DB3
	PE11 - DB4
	PE12 - DB5
	PE13 - DB6
	PE14 - DB7
	PB10 - Rs
	PB11 - R/W
	PB12 - E
*/
int main() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	//GPIOD->MODER |= (1 << 10);
	// 0 1 2 5 7 8 11 12 13 14 15
	disp_init('B', 0b1111100110100111);
	delay_ms(100);
	//GPIOB->MODER = 0b00010101010000010100010100010101; // 0011 1001 1011 1111
	//GPIOB->ODR = 0;
	//GPIOB->ODR = 0b0111100110110111;

	//uint32_t data = (0x30); // 1110 0010
	disp_wrt_com(0x0F);
	while(1) {
		delay_us(5);
		//
	}
}


#pragma GCC diagnostic pop
