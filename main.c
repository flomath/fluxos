/*
 * main.c
 */

#include <stdio.h>
#include "src/system/hal/omap3530/uart/uart.h"
#include "src/system/hal/omap3530/interrupt/interrupt.h"
#include "src/system/hal/omap3530/timer/timer.h"
#include "src/system/hal/omap3530/prcm/percm.h"

interrupt_callback timer_irq;
interrupt_callback uart_irq;

#pragma TASK(main)
void main(void) {

	// Set up interrupts
	interrupt_init();

	// Add IRQ handler
	// Notice: Handler won't be called right now -> Todo
	// Notice: Still needed for enabling the IRQ
	//interrupt_add_listener(46, &timer_irq);
	interrupt_add_listener(74, &uart_irq);

	gpt_timer_init(GPT_TIMER10);
	gpt_timer_start(GPT_TIMER10);

	// Set up UART
	UARTConfiguration_t conf = {
			0x1,
			9600,
			UART_PARITY_NONE,
			UART_LENGTH_8
	};
	uart_init(UART3, &conf);

	// Enable interrupts globally
	interrupt_enable();

	// Execute
	printf("Start writing data...\n");
	while(1)
	{
		//uint8_t a = 0x55;
		//uint8_t b;
		//uart_write(UART3, &a);
		//uart_read(UART1, &b);
	}
}

void timer_irq(void)
{
	*((mmio_t)(GPT_TIMER10 + GPT_TISR)) |= BV(0) + BV(1) + BV(2);
}

void uart_irq(void)
{
	uint8_t type = BIT_RANGE(hal_get_address_value(UART3, UART_IIR_REG), 5, 1);
	uint8_t received;
	switch (type) {
	case 0x2:
		// RHR
		received = hal_get_address_value(UART3, UART_RHR_REG);
		// Clear RS-FIFO Trigger and reset counter logic
		hal_bitmask_set(UART3, UART_FCR_REG, BV(1));

		uart_write(UART3, &received);
		break;
	case 0x6:
		// Timeout
		received = hal_get_address_value(UART3, UART_RHR_REG);
		//hal_bitmask_set(UART3, UART_FCR_REG, BV(1));
		printf("UART Timeout (%c)\n", received);

		uart_write(UART3, &received);
		break;
	default:
		printf("Unsupported UART IRQ Type: %d\n", type);
		break;
	}
}
