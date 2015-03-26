/*
 * main.c
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */

#include <stdio.h>
#include "src/system/scheduler/scheduler.h"
#include "src/system/hal/omap3530/timer/timer.h"

void task1(void)
{
	while(1) {
		printf("task 1 is running\n");
	}
}

int main(void) {

	/*scheduler_addProcess(task1);
	scheduler_run();*/

	/**
	* Timer Tests
	*/
	printf("Test timer\n");
	gpt_timer_init(GPT_TIMER10);
	gpt_timer_start(GPT_TIMER10);

	int i=0;
	while (1)
	{
		for(i = 0; i < 500; i++);
		printf("Coutner: %x\n", hal_get_address_value(GPT_TIMER10, GPT_TCRR));
		printf("OCR: %i\n", hal_get_address_value(GPT_TIMER10, GPT_TOCR));
	}

	return 0;
}
