#include <stdio.h>
#include <stdlib.h>

typedef struct  {
	int slave_addr;
	int dest_reg;
	int num_bytes;
	int read_data;
	int write_data;
} I2C_data;

I2C_data *I2C1_data;

void test_func(void);
void test_func2(int *data);

int main() {

	//I2C_data temp_struct;
	
	//test_func();
	
	I2C1_data = malloc(sizeof(I2C_data));
	
	I2C1_data->slave_addr = 0x68;
	I2C1_data->dest_reg = 0x75;
	I2C1_data->num_bytes = 1;
	I2C1_data->read_data = 0;
	I2C1_data->write_data = 10;
	
	printf("\nSlave address: 0x%x\nDestination register: 0x%x\nNumber of bytes: %x\n\n\n",
	          I2C1_data->slave_addr, I2C1_data->dest_reg, I2C1_data->num_bytes);
	
	int data;
	data = 9;
	test_func2(&data);
	
	int *data1;
	
	data1 = malloc(sizeof(int)*2);
	
	data1[0] = 10;
	data1[1] = 5;
	
	printf("data1[0]: %d\n", data1[0]);
	printf("data1[1]: %d\n\n", data1[1]);
	
	printf("Is this an address? %d", &data1[0]);
	
	return 0;
}

void test_func(void) {
	I2C_data I2C_struct;
	I2C1_data = &I2C_struct;
		
	printf("It's reaching here\n");
	
}

void test_func2(int *data) {
	printf("Printing 'data': %d\n", data);
	printf("Prindint '*data': %d\n\n", *data);
}


