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
	gpt_timer_init(GPT_TIMER1);
	gpt_timer_start(GPT_TIMER1);


	return 0;
}
