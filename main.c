/*
 * main.c
 */

#include <stdio.h>
#include "src/system/hal/omap3530/uart/uart.h"
#include "src/system/hal/omap3530/interrupt/interrupt.h"
#include "src/system/hal/omap3530/timer/timer.h"

#pragma TASK(main)
void main(void) {

	// Set up interrupts
	interrupt_init();

	// Add IRQ handler
	// Notice: Handler won't be called right now -> Todo
	// Notice: Still needed for enabling the IRQ
	interrupt_add_listener(46, NULL);

	interrupt_enable();

	gpt_timer_init(GPT_TIMER10);
	gpt_timer_start(GPT_TIMER10);

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
		//uint8_t b;
		uart_write(UART3, &a);
		//uart_read(UART1, &b);
	}
}
