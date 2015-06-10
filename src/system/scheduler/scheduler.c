/*
 * scheduler.c
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */

#include <stdio.h>
#include "scheduler.h"
#include "../hal/omap3530/interrupt/interrupt.h"
#include "process.h"

/**
 * Array with all PCBs
 */
PCB_t contexts[SCHEDULER_MAX_PROCESSES];
int SchedulerCurrentRunningProcess = SCHEDULER_INVALID_ID;

static void scheduler_killCurrentProcess(void);

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

	// stack size 0x20000 - do not start at 0x20000 because of rom exceptions
	contexts[newProcessID].registers.SP = (uint32_t) 0x10020000; //TODO: sp on different address?
	contexts[newProcessID].registers.CPSR = 0b10000; // USER MODE
	contexts[newProcessID].registers.LR = (uint32_t)&scheduler_killCurrentProcess;
	contexts[newProcessID].registers.PC = (uint32_t)(contexts[newProcessID].func) + 4;
	mmu_create_process(&contexts[newProcessID]);

	mutex_release();
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
//			printf("Process switch %i\n", contexts[SchedulerCurrentRunningProcess].processID);
			mmu_switch_process(&contexts[SchedulerCurrentRunningProcess]);
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

static void scheduler_killCurrentProcess()
{
	//TODO: extract to syscall

	// get current process for id
	// and kill it
	PCB_t* process = scheduler_getCurrentProcess();
	if (process != NULL) {
		scheduler_killProcess(process->processID);
	}
}

void scheduler_killProcess(int processID)
{
	//TODO: this function has to be fully executed!!!
	//contexts[processID].state = PROCESS_TERMINATED;
	contexts[processID].func = NULL;

	mmu_kill_process(&contexts[processID]);

	contexts[processID].state = PROCESS_CREATED;
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

PCB_t* scheduler_getCurrentProcess()
{
	return &contexts[SchedulerCurrentRunningProcess];
}
