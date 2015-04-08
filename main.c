/*
 * main.c
 */

#include <stdio.h>
#include "src/system/hal/omap3530/uart/uart.h"
#include "src/system/hal/omap3530/interrupt/interrupt.h"
#include "src/system/hal/omap3530/timer/timer.h"

interrupt_callback timer_irq;
static uint16_t timerCounter;

//#pragma TASK(main)
void main(void) {

	// Set up interrupts
	interrupt_init();

	timerCounter=0;
	// Add IRQ handler
	// Notice: Handler won't be called right now -> Todo
	// Notice: Still needed for enabling the IRQ
	interrupt_add_listener(40, &timer_irq);

	interrupt_enable();

	gpt_timer_init(GPT_TIMER4);
	gpt_timer_start(GPT_TIMER4);

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
	while(1) {
		uint8_t a = 0x55;
		//uint8_t b;
		uart_write(UART3, &a);
		//uart_read(UART1, &b);
	}
}

void timer_irq(void) {
	gpt_timer_reset(GPT_TIMER4);
	timerCounter++;
	printf("timer count: %u\n", timerCounter);
	//*((mmio_t)(GPT_TIMER10 + GPT_TISR)) |= BV(0) + BV(1) + BV(2);
}
