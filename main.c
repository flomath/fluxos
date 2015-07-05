/*
 * main.c
 */

#include "src/system/kernel.h"

#pragma TASK(main)
void main(void) {

	kernel_boot();

	// MagicExecute
	while(1) {
		printf("..idle\n");
	}
}

