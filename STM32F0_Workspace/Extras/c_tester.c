#include <stdio.h>

int main() {
	
	int i;
	int pin_addr = 0x000000000;
	int pins = 0b0000011111111111; // 0 1 2 5 6 7 8 9 11 13 14
	int bit_pos[11];
	int data = 0b01100101;
	
	for(i = 0; i < 16; i++) {
		pin_addr |= ((pins >> i) & 1) << (i*2);
		pin_addr |= ((pins >> i) & 1) << ((i*2)+1);
	}
	
	// 00 00 11 00 11 00 11 11 =  0x0CCF
	printf("\nThe resulting value:\n%X\n",pin_addr);
	printf("Now in decimal! %u\n", pin_addr);
	
	int shift = 0;
	for(i = 0; i < 16; i++) {
		if((1 << i) & pins) {
			bit_pos[shift] = i;
			shift++;
		}
	}
	
	printf("\nValues in the bit positions:\n");
	for(i = 0; i < 11; i++) {
		printf("%d\n",bit_pos[i]);
	}
	
	printf("\nTesting bit transfer...\n");
	for(i = 3; i < 11; i++) {
		int temp = (data >> (i-3)) & 1;
		printf("Bit sent: %d\n",temp);
	}
	
	// 0001 0000 0000 0000 0001 0000 0001
	
	return 0;
}