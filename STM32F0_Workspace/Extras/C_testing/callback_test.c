#include <stdio.h>
#include <stdlib.h>

int test_var;

typedef void (*callback)(void);

void the_callback();
void cb_test(callback callback_func);

int main() {
	
	test_var = 10;
	printf("\nValue before callback function: %d\n", test_var);
	
	callback test_cb = the_callback;
	
	cb_test(test_cb);
	
	
	printf("\nValue after the callback function: %d\n\n", test_var);
	
	test_var = 20;
	test_cb = NULL;
	printf("\nValue before callback function again: %d\n", test_var);
	cb_test((callback)the_callback);
	printf("\nValue after the callback function again: %d\n\n",test_var);
	
	
	return 0;
}

void the_callback() {
	test_var = 50;
	return;
}

void cb_test(callback callback_func) {
	(*callback_func)();
}