/*
 * main.c
 */

#include "src/system/hal/omap3530/uart/uart.h"

int main(void) {

	UARTConfiguration_t conf = {
			0x0,
			9600,
			UART_PARITY_NONE,
			UART_LENGTH_8
	};

	uart_init(UART3, &conf);
	
	while(1)
	{
		uint8_t a = 0x55;
		uint8_t b;
		uart_write(UART3, &a);
		//uart_read(UART1, &b);
	}

	return 0;
}
