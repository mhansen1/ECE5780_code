#include <stdio.h>
#include <stdlib.h>
#include <stm32f0xx.h>
#include "diag/Trace.h"
#inclide "delay.h"
#include "led_init.h"

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
	led_init(0x0F);
	delay_init();
	
	GPIOD->MODER |
	
	while(1) {
		
	}
}


#pragma GCC diagnostic pop