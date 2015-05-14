/*
 * scheduler.c
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */


#include "scheduler.h"
#include "../hal/omap3530/interrupt/interrupt.h"

/**
 * Array with all PCBs
 */
PCB_t __contexts[SCHEDULER_MAX_PROCESSES];
static int SchedulerCurrentRunningProcess = SCHEDULER_INVALID_ID;

void scheduler_addProcess(ProcFunc fct)
{
	mutex_lock();

	int newProcessID = scheduler_getFreeProcessID();
	if (newProcessID == SCHEDULER_INVALID_ID)
	{
		mutex_release();
	}

	__contexts[newProcessID].processID = newProcessID;
	__contexts[newProcessID].state = PROCESS_READY;
	__contexts[newProcessID].func = fct;
}

void scheduler_run() 
{
	mutex_lock();

	int nextProcess = scheduler_getNextProcess();
	if(nextProcess == SCHEDULER_INVALID_ID) return;

	switch(__contexts[nextProcess].state) {
		case PROCESS_RUNNING: break;

		case PROCESS_READY: 
		{
			// speicher Context für Thread
			/*if(setjmp(__contexts[runningThread].context) == 0) {*/
			if(__contexts[SchedulerCurrentRunningProcess].state == PROCESS_RUNNING)
				__contexts[SchedulerCurrentRunningProcess].state = PROCESS_READY;

			// Save context
			__contexts[SchedulerCurrentRunningProcess].registers = __context_current;

			SchedulerCurrentRunningProcess = nextProcess;
			__contexts[SchedulerCurrentRunningProcess].state = PROCESS_RUNNING;
			//__contexts[SchedulerCurrentRunningProcess].func();
			//longjmp(__contexts[runningThread].context, 1);

			// Load context
			__context_load(&__contexts[SchedulerCurrentRunningProcess]);

			/*}*/
		} break;

		default: break;
	}

	mutex_release();
}

int scheduler_getNextProcess()
{
	int nextProcess = SchedulerCurrentRunningProcess + 1;

	while (nextProcess < SCHEDULER_MAX_PROCESSES && nextProcess != SchedulerCurrentRunningProcess)
	{
		if (__contexts[nextProcess].state == PROCESS_READY)
		{
			return nextProcess;
		}
		// found a terminated process, remove it
		else if (__contexts[nextProcess].state == PROCESS_TERMINATED)
		{
			// TODO FlorianM: what do we need to do here?
		}

		nextProcess = nextProcess + 1;
		nextProcess = nextProcess % SCHEDULER_MAX_PROCESSES;
	}

	return SCHEDULER_INVALID_ID;
}

void scheduler_killProcess(int processID)
{
	__contexts[processID].state = PROCESS_TERMINATED;
	__contexts[processID].func = NULL;
}

int scheduler_getFreeProcessID() 
{
	int i;
	for (i=0; i < SCHEDULER_MAX_PROCESSES; i++) 
	{
		if (__contexts[i].state == PROCESS_CREATED)
		{
			return i;
		}
	}
	return SCHEDULER_INVALID_ID;
}
