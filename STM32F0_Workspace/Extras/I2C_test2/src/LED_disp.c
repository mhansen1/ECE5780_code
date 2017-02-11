#include "LED_disp.h"

#define addr(x) ((volatile uint32_t *) x)

uint16_t pins;
uint32_t base_addr;
uint32_t odr_addr;
uint8_t bit_pos[11];

// On the pins, valid bit map:
// 1. RS
// 2. R/W
// 3. E
// 4-7. DB0-DB3
// 8-11. DB4-DB7
void disp_init(char port, uint16_t _pins) {
	pins = _pins; // Store the pins that are being used.

	// Set the port addresses
	switch(port) {
		case 'A' :
			RCC->AHB1ENR |= (1 << 0);
			base_addr = GPIOA_BASE;
			break;

		case 'B' :
			RCC->AHB1ENR |= (1 << 1);
			base_addr = GPIOB_BASE;
			break;

		case 'C' :
			RCC->AHB1ENR |= (1 << 2);
			base_addr = GPIOC_BASE;
			break;

		case 'D' :
			RCC->AHB1ENR |= (1 << 3);
			base_addr = GPIOD_BASE;
			break;

		case 'E' :
			RCC->AHB1ENR |= (1 << 4);
			base_addr = GPIOE_BASE;
			break;

		case 'F' :
			RCC->AHB1ENR |= (1 << 5);
			base_addr = GPIOF_BASE;
			break;
	}
	uint32_t spd_addr = base_addr + 0x08;
	odr_addr = base_addr + 0x14;
	
	// Set MODE register.
	uint16_t i;
	for(i = 0; i < 16; i++) {
		*addr(base_addr) |= ((pins >> i) & 1) << (i*2);
	}
	
	// Set the speed. 
	for(i = 0; i < 16; i++) {
		*addr(spd_addr) |= ((pins >> i) & 1) << (i*2);
		*addr(spd_addr) |= ((pins >> i) & 1) << ((i*2)+1);
	}

	// Set output to low.
	*addr(odr_addr) &= ~(pins);

	// Find the bit positions.
	uint16_t shift = 0;
	for(i = 0; i < 16; i++) {
		if((1 << i) & pins) {
			bit_pos[shift] = i;
			shift++;
		}
	}

	// Initialize the delay functionality.
	delay_init();
	delay_us(1);

}
/*
 * Message structure
 * 1. Set RS (0 for command, 1 for data)
 * 2. Set read/write bit.
 * 3. Set enable high.
 * 4. Write data.
 * 5. Delay 1us
 * 6. Set enable low.
 * 7. Delay 1us.
 * 8. Reset data lines back to zero.
 * 9. Return.
 */


void disp_wrt_com(uint32_t data) {
	*addr(odr_addr) &= ~pins;
	// (1)
	*addr(odr_addr) &= ~(1 << bit_pos[0]);
	// (2)
	*addr(odr_addr) &= ~(1 << bit_pos[1]);
	// (3)
	*addr(odr_addr) |= (1 << bit_pos[2]);
	// (4)
	uint8_t i;
	uint16_t send_data;
	for(i = 3; i < 11; i++) {
		uint32_t temp = (data >> (i-3)) & 1;
		send_data |= (temp << bit_pos[i]);
	}
	*addr(odr_addr) |= send_data; //1100 1100 1100 1000
	*addr(odr_addr) &= ~(1 << bit_pos[0]);
	*addr(odr_addr) &= ~(1 << bit_pos[1]);
	//GPIOB->ODR |= (0xFF << 3);
	// (5)
	delay_us(1);
	// (6)
	*addr(odr_addr) &= ~(1 << bit_pos[2]);
	// (7)
	delay_us(1);
	//(8)
	*addr(odr_addr) &= ~pins;
	data = 0;
	return;
}




void disp_wrt_dat(uint8_t data) {
	*addr(odr_addr) &= ~pins;
	// (1)
	*addr(odr_addr) |= (1 << bit_pos[0]);
	// (2)
	*addr(odr_addr) &= ~(1 << bit_pos[1]);
	// (3)
	*addr(odr_addr) |= (1 << bit_pos[2]);
	// (4)
	uint8_t i;
	uint16_t send_data;
	for(i = 3; i < 11; i++) {
		uint32_t temp = (data >> (i-3)) & 1;
		send_data |= (temp << bit_pos[i]);
	}
	*addr(odr_addr) |= send_data; //1100 1100 1100 1000
	//GPIOB->ODR |= (0xFF << 3);
	// (5)
	delay_us(1);
	// (6)
	*addr(odr_addr) &= ~(1 << bit_pos[2]);
	// (7)
	delay_us(1);
	//(8)
	for(i = 3; i < 11; i++) {
		*addr(odr_addr) &= ~(1 << bit_pos[i]);
	}
	return;
}








