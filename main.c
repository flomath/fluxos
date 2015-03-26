/*
 * main.c
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */

#include <stdio.h>
#include "src/system/scheduler/scheduler.h"

void task1(void)
{
	while(1) {
		printf("task 1 is running\n");
	}
}

int main(void) {

	scheduler_addProcess(task1);

	scheduler_run();

	return 0;
}
