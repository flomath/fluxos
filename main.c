/*
 * main.c
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */

#include <stdio.h>
#include "src/system/scheduler/process.h"
#include "src/system/scheduler/scheduler.h"
#include "src/tools/list.h"

void task1(void)
{
	printf("task 1 is running\n");
}

int main(void) {
	SchedulerAlgorithm_t roundrobin;
	Scheduler_t* sched = scheduler_init(roundrobin);

	Process_t* prc1 = process_create();
	prc1->task = &task1;
	scheduler_addprocess(sched, prc1);

	prc1->task();

	scheduler_run(sched);

	return 0;
}
