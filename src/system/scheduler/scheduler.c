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
PCB_t contexts[SCHEDULER_MAX_PROCESSES];
int SchedulerCurrentRunningProcess = SCHEDULER_INVALID_ID;

char stacks[SCHEDULER_MAX_PROCESSES][1024];

void scheduler_addProcess(ProcFunc fct)
{
	mutex_lock();

	int newProcessID = scheduler_getFreeProcessID();
	if (newProcessID == SCHEDULER_INVALID_ID)
	{
		mutex_release();
	}

	contexts[newProcessID].processID = newProcessID;
	contexts[newProcessID].state = PROCESS_READY;
	contexts[newProcessID].func = fct;

	contexts[newProcessID].registers.SP = (uint32_t)(stacks[newProcessID]) + 1020;
	contexts[newProcessID].registers.CPSR = 0b10000; // USER MODE
	contexts[newProcessID].registers.LR = NULL; // Todo Set Process Exit Handler
	contexts[newProcessID].registers.PC = (uint32_t)(contexts[newProcessID].func) + 4;
}

void scheduler_run(Registers_t* context)
{
	mutex_lock();

	int nextProcess = scheduler_getNextProcess();
	if(nextProcess == SCHEDULER_INVALID_ID) return;

	switch(contexts[nextProcess].state) {
		case PROCESS_RUNNING: break;

		case PROCESS_READY: 
		{
			// Do not save an invalid process
			if (SchedulerCurrentRunningProcess != SCHEDULER_INVALID_ID) {

				// Save the context
				if(contexts[SchedulerCurrentRunningProcess].state == PROCESS_RUNNING)
					contexts[SchedulerCurrentRunningProcess].state = PROCESS_READY;

				// Save temporarly saved context
				contexts[SchedulerCurrentRunningProcess].registers.R0 = context->R0;
				contexts[SchedulerCurrentRunningProcess].registers.R1 = context->R1;
				contexts[SchedulerCurrentRunningProcess].registers.R2 = context->R2;
				contexts[SchedulerCurrentRunningProcess].registers.R3 = context->R3;
				contexts[SchedulerCurrentRunningProcess].registers.R4 = context->R4;
				contexts[SchedulerCurrentRunningProcess].registers.R5 = context->R5;
				contexts[SchedulerCurrentRunningProcess].registers.R6 = context->R6;
				contexts[SchedulerCurrentRunningProcess].registers.R7 = context->R7;
				contexts[SchedulerCurrentRunningProcess].registers.R8 = context->R8;
				contexts[SchedulerCurrentRunningProcess].registers.R9 = context->R9;
				contexts[SchedulerCurrentRunningProcess].registers.R10 = context->R10;
				contexts[SchedulerCurrentRunningProcess].registers.R11 = context->R11;
				contexts[SchedulerCurrentRunningProcess].registers.R12 = context->R12;
				contexts[SchedulerCurrentRunningProcess].registers.SP = context->SP;
				contexts[SchedulerCurrentRunningProcess].registers.LR = context->LR;
				contexts[SchedulerCurrentRunningProcess].registers.PC = context->PC;
				contexts[SchedulerCurrentRunningProcess].registers.CPSR = context->CPSR;
			}

			// Switch context
			SchedulerCurrentRunningProcess = nextProcess;
			contexts[SchedulerCurrentRunningProcess].state = PROCESS_RUNNING;
			context->R0 = contexts[SchedulerCurrentRunningProcess].registers.R0;
			context->R1 = contexts[SchedulerCurrentRunningProcess].registers.R1;
			context->R2 = contexts[SchedulerCurrentRunningProcess].registers.R2;
			context->R3 = contexts[SchedulerCurrentRunningProcess].registers.R3;
			context->R4 = contexts[SchedulerCurrentRunningProcess].registers.R4;
			context->R5 = contexts[SchedulerCurrentRunningProcess].registers.R5;
			context->R6 = contexts[SchedulerCurrentRunningProcess].registers.R6;
			context->R7 = contexts[SchedulerCurrentRunningProcess].registers.R7;
			context->R8 = contexts[SchedulerCurrentRunningProcess].registers.R8;
			context->R9 = contexts[SchedulerCurrentRunningProcess].registers.R9;
			context->R10 = contexts[SchedulerCurrentRunningProcess].registers.R10;
			context->R11 = contexts[SchedulerCurrentRunningProcess].registers.R11;
			context->R12 = contexts[SchedulerCurrentRunningProcess].registers.R12;
			context->SP = contexts[SchedulerCurrentRunningProcess].registers.SP;
			context->LR = contexts[SchedulerCurrentRunningProcess].registers.LR;
			context->PC = contexts[SchedulerCurrentRunningProcess].registers.PC;
			context->CPSR = contexts[SchedulerCurrentRunningProcess].registers.CPSR;
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
		if (contexts[nextProcess].state == PROCESS_READY)
		{
			return nextProcess;
		}
		// found a terminated process, remove it
		else if (contexts[nextProcess].state == PROCESS_TERMINATED)
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
	contexts[processID].state = PROCESS_TERMINATED;
	contexts[processID].func = NULL;
}

int scheduler_getFreeProcessID() 
{
	int i;
	for (i=0; i < SCHEDULER_MAX_PROCESSES; i++) 
	{
		if (contexts[i].state == PROCESS_CREATED)
		{
			return i;
		}
	}
	return SCHEDULER_INVALID_ID;
}
