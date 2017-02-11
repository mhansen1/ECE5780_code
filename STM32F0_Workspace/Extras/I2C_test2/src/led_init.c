#include "led_init.h"

void led_init(uint8_t leds) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	// Enable LED on pin 12
	if(leds & 0x01) {
		GPIOD->MODER |= (1 << 24);
		GPIOD->ODR &= ~(1 << 12);
	}
	// Enable LED on pin 13
	if(leds & 0x02) {
		GPIOD->MODER |= (1 << 26);
		GPIOD->ODR &= ~(1 << 13);
	}
	/// Enable LED on pin 14
	if(leds & 0x04) {
		GPIOD->MODER |= (1 << 28);
		GPIOD->ODR &= ~(1 << 14);
	}
	// Enable LED on pin 15
	if(leds & 0x08) {
		GPIOD->MODER |= (1 << 30);
		GPIOD->ODR &= ~(1 << 15);
	}
}
