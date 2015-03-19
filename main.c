/*
 * main.c
 */

#include "src/system/hal/arch/omap3530/uart/uart.h"

int main(void) {

	UARTConfiguration_t conf = {
			0x0,
			115200,
			UART_PARITY_0,
			UART_LENGTH_8
	};

	uart_init(UART1, &conf);
	
	while(1)
	{}

	return 0;
}
