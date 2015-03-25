/*
 * scheduler.h
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_COMMON_SCHEDULER_SCHEDULER_H_
#define SRC_SYSTEM_HAL_COMMON_SCHEDULER_SCHEDULER_H_

#include "process.h"
#include "../../tools/list.h"

typedef void (*SchedulerAlgorithm_t)();

typedef struct {
	SchedulerAlgorithm_t algorithm;
	List_t* processList;
} Scheduler_t;

/**
 * Initialize the scheduler
 */
Scheduler_t* scheduler_init(SchedulerAlgorithm_t algorithm);

/**
 * Run scheduler
 */
void scheduler_run(Scheduler_t* scheduler);

/**
 * Add a new process to the scheduler
 */
void scheduler_addprocess(Scheduler_t* scheduler, Process_t* process);

#endif /* SRC_SYSTEM_HAL_COMMON_SCHEDULER_SCHEDULER_H_ */
