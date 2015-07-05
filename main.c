/*
 * main.c
 */

#include <stdio.h>
#include "src/system/driver/uart/UartDriver.h"
#include "src/system/driver/button/ButtonDriver.h"
#include "src/system/driver/gpio/GPIODriver.h"
#include "src/system/hal/common/interrupt_sw.h"
#include "src/system/hal/omap3530/interrupt/interrupt.h"
#include "src/system/hal/omap3530/timer/timer.h"
#include "src/system/hal/omap3530/prcm/percm.h"
#include "src/system/scheduler/scheduler.h"
#include "src/system/hal/omap3530/mmu/mmu.h"
#include "src/system/scheduler/loader.h"
#include "src/console/console.h"

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

	// initialise MMU
	mmu_init();

	// Add IRQ handler
	interrupt_add_listener(40, &timer_irq);

	gpt_timer_init(GPT_TIMER4, 500);
	gpt_timer_start(GPT_TIMER4);

	uart_driver_init(9600);
	scheduler_addProcess(console_init);
	//scheduler_addProcess(test);
	//scheduler_addProcess(test2);
	//scheduler_addProcess(uart_process);

	// Enable interrupts globally
	interrupt_enable();

	//uart_driver_write("hallo nino", 10);
	//syscall(SYS_PRINT, (uint32_t*)"hallo nino", 10);

	// call software interrupt
	//syscall(SYS_DEBUG, 0);

	// Execute
	while(1) {
		//uart_driver_write("hallo", 5);
		printf("..idle\n");
	}
}

void test(void) {
	int a = 1;
	a++;

	printf("%i\n", a);
	char* test = "test";
	syscall(SYS_PRINT, (uint32_t*)test, 5);

	a++;
	printf("%i\n", a);

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
		printf("[2] further test\n");
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
			//printf("No Data to process\n");
		}
	}
}

void timer_irq(Registers_t* context) {
	// This method will never return
	//button_driver_interrupt(context);
	scheduler_run(context);

	gpt_timer_reset(GPT_TIMER4);
	gpt_timer_start(GPT_TIMER4);
}
