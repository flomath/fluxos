/*
 * scheduler.c
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */


#include "scheduler.h"

void scheduler_addProcess(ProcFunc fct)
{
	mutex_lock();

	int newProcessID = scheduler_getFreeProcessID();
	if (newProcessID == SCHEDULER_INVALID_ID)
	{
		mutex_release();
	}

	SchedulerProcesses[newProcessID].ProcessID = newProcessID;
	SchedulerProcesses[newProcessID].state = PROCESS_READY;
	SchedulerProcesses[newProcessID].func = fct;
}

void scheduler_run() 
{
	mutex_lock();

	int nextProcess = scheduler_getNextProcess();
	if(nextProcess == SCHEDULER_INVALID_ID) return;

	switch(SchedulerProcesses[nextProcess].state) {
		case PROCESS_RUNNING: break;

		case PROCESS_READY: 
		{
			// speicher Context für Thread
			/*if(setjmp(SchedulerProcesses[runningThread].context) == 0) {*/
			if(SchedulerProcesses[SchedulerCurrentRunningProcess].state == PROCESS_RUNNING)
				SchedulerProcesses[SchedulerCurrentRunningProcess].state = PROCESS_READY;

			SchedulerCurrentRunningProcess = nextProcess;
			SchedulerProcesses[SchedulerCurrentRunningProcess].state = PROCESS_RUNNING;
			SchedulerProcesses[SchedulerCurrentRunningProcess].func();
			//longjmp(SchedulerProcesses[runningThread].context, 1);
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
		if (SchedulerProcesses[nextProcess].state == PROCESS_READY) 
		{
			return nextProcess;
		}
		// found a terminated process, remove it
		else if (SchedulerProcesses[nextProcess].state == PROCESS_TERMINATED)
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
	SchedulerProcesses[processID].state = PROCESS_TERMINATED;
	SchedulerProcesses[processID].func = NULL;
}

int scheduler_getFreeProcessID() 
{
	int i;
	for (i=0; i < SCHEDULER_MAX_PROCESSES; i++) 
	{
		if (SchedulerProcesses[i].state == PROCESS_CREATED) 
		{
			return i;
		}
	}
	return SCHEDULER_INVALID_ID;
}
