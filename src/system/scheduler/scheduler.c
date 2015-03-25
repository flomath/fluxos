/*
 * scheduler.c
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */


#include "scheduler.h"

Scheduler_t* scheduler_init(SchedulerAlgorithm_t algorithm)
{
	Scheduler_t* sched;
	sched = (Scheduler_t*)malloc(sizeof(Scheduler_t));

	sched->algorithm = algorithm;
	sched->processList = list_create();

	return 0;
}

void scheduler_run(Scheduler_t* scheduler)
{
	Process_t* proc = list_first(scheduler->processList); // nextAvailableProcess
	proc->task();
}

void scheduler_addprocess(Scheduler_t* scheduler, Process_t* process)
{
	if(scheduler == NULL)
		return;

	list_push(scheduler->processList, process);
}
