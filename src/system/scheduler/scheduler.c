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

extern Registers_t __context_current;

char* stacks[SCHEDULER_MAX_PROCESSES];

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

	stacks[newProcessID] = (char*)malloc(4096);
	__contexts[newProcessID].registers.SP = (uint32_t)(stacks[newProcessID]) + 4095;
	__contexts[newProcessID].registers.CPSR = 0b10000; // USER MODE
	__contexts[newProcessID].registers.LR = (uint32_t)(__contexts[newProcessID].func);
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
			// Do not save an invalid process
			if (SchedulerCurrentRunningProcess == SCHEDULER_INVALID_ID) {
				SchedulerCurrentRunningProcess = nextProcess;
				__contexts[SchedulerCurrentRunningProcess].state = PROCESS_RUNNING;

				// Load context
				__context_load(&__contexts[SchedulerCurrentRunningProcess]);
			} else {
				// Save Context
				if (!__context_save(&(__contexts[SchedulerCurrentRunningProcess]))) {

					if(__contexts[SchedulerCurrentRunningProcess].state == PROCESS_RUNNING)
						__contexts[SchedulerCurrentRunningProcess].state = PROCESS_READY;

					// Save temporarly saved context
					__contexts[SchedulerCurrentRunningProcess].registers.R0 = __context_current.R0;
					__contexts[SchedulerCurrentRunningProcess].registers.R1 = __context_current.R1;
					__contexts[SchedulerCurrentRunningProcess].registers.R2 = __context_current.R2;
					__contexts[SchedulerCurrentRunningProcess].registers.R3 = __context_current.R3;
					__contexts[SchedulerCurrentRunningProcess].registers.R4 = __context_current.R4;
					__contexts[SchedulerCurrentRunningProcess].registers.R5 = __context_current.R5;
					__contexts[SchedulerCurrentRunningProcess].registers.R6 = __context_current.R6;
					__contexts[SchedulerCurrentRunningProcess].registers.R7 = __context_current.R7;
					__contexts[SchedulerCurrentRunningProcess].registers.R8 = __context_current.R8;
					__contexts[SchedulerCurrentRunningProcess].registers.R9 = __context_current.R9;
					__contexts[SchedulerCurrentRunningProcess].registers.R10 = __context_current.R10;
					__contexts[SchedulerCurrentRunningProcess].registers.R11 = __context_current.R11;
					__contexts[SchedulerCurrentRunningProcess].registers.R12 = __context_current.R12;

					SchedulerCurrentRunningProcess = nextProcess;
					__contexts[SchedulerCurrentRunningProcess].state = PROCESS_RUNNING;

					// Load context
					__context_load(&__contexts[SchedulerCurrentRunningProcess]);
				}
			}
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
