/*
 * main.c
 */
#include "../../src/api/syscall.h"
#include <string.h>

int main(void) {
	char* data = "Hello from Process 1";
	syscall(SYS_PRINT, (uint32_t*) data, strlen(data));
	
	return 0;
}
