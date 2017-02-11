//#include <stdio.h>
//#include <stdlib.h>
//#include "diag/Trace.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "core_cm4.h"
#include "I2C.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int
main(int argc, char* argv[]) {
	
	init_I2C(400000, 'B', 0, 1);
	uint8_t data2;
	I2C1_read(0x68, 0x75, 1, &data2);
	uint8_t data[14];
	I2C1_read(0x68, 0x3B, 14, data);


	while(1) {
	}
}

#pragma GCC diagnostic pop
