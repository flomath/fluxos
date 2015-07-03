/*
 * main.c
 */
#include "../../src/api/syscall.h"

int main(void) {
	char* data = "Hello from Process 1";
	syscall(SYS_PRINT, (uint32_t*) data, sizeof(data));
	
	return 0;
}
