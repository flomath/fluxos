/*
 * main.c
 */

#include <stdio.h>
#include "src/system/driver/uart/UartDriver.h"
#include "src/system/driver/button/ButtonDriver.h"
#include "src/system/driver/gpio/GPIODriver.h"
#include "src/system/hal/omap3530/interrupt/interrupt.h"
#include "src/system/hal/omap3530/timer/timer.h"
#include "src/system/hal/omap3530/prcm/percm.h"
#include "src/system/scheduler/scheduler.h"

interrupt_callback timer_irq;

void test(void);
void test2(void);
void uart_process(void);

#pragma TASK(main)
void main(void) {

	// Set up interrupts
	interrupt_init();

	// initialise onboard button
	button_driver_init();

	// initialise LED
	gpio_driver_init();

	// Add IRQ handler
	interrupt_add_listener(40, &timer_irq);

	gpt_timer_init(GPT_TIMER4, 3000);
	gpt_timer_start(GPT_TIMER4);

	scheduler_addProcess(test);
	scheduler_addProcess(test2);
	scheduler_addProcess(uart_process);
	uart_driver_init(9600);

	// Enable interrupts globally
	interrupt_enable();

	// Execute
	while(1) {
		printf("..idle\n");
	}
}

void test(void) {
	while(1) {
		printf("[1] task test\n");
		int x = 0;
		x++;
	}
}
void test2(void) {
	while(1) {
		printf("[2] task test\n");
		int y = 1;
		y--;
	}
}

void uart_process(void) {
	while (1) {
		int count = uart_driver_count();
		if ( count > 0 ) {
			char buffer[8];
			uart_driver_read(buffer, 8);

			int i;
			for ( i = 0; i < 8 && i < count; i++ ) {
				printf("%c", buffer[i]);
			}
			uart_driver_write(buffer, count < 8 ? count : 8);
		} else {
			printf("No Data to process\n");
		}
	}
}

void timer_irq(Registers_t* context) {
	gpt_timer_reset(GPT_TIMER4);
	gpt_timer_start(GPT_TIMER4);

	// This method will never return
	scheduler_run(context);
}
