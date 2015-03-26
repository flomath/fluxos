/*
 * main.c
 */

#include <stdio.h>
#include "src/system/hal/omap3530/uart/uart.h"
#include "src/system/hal/omap3530/interrupt/interrupt.h"

int main(void) {

	// Set up interrupts
	interrupt_init();
	interrupt_enable();

	// Set up UART
	UARTConfiguration_t conf = {
			0x0,
			9600,
			UART_PARITY_NONE,
			UART_LENGTH_8
	};
	uart_init(UART3, &conf);
	
	// Execute
	printf("Start writing data...");
	while(1)
	{
		uint8_t a = 0x55;
		uint8_t b;
		uart_write(UART3, &a);
		//uart_read(UART1, &b);
	}

	return 0;
}
