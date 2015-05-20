/*
 * process.h
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_SCHEDULER_PROCESS_H_
#define SRC_SYSTEM_SCHEDULER_PROCESS_H_

#include <stdint.h>

typedef enum {
	PROCESS_CREATED = 0,
	PROCESS_READY,
	PROCESS_RUNNING,
	PROCESS_BLOCKED,
	PROCESS_TERMINATED
} ProcessState_t;

typedef struct {
	uint8_t ProcessID;
	ProcessState_t state;
	void (*func)(void);

	uint32_t* pageTable;
} Process_t;

#endif /* SRC_SYSTEM_SCHEDULER_PROCESS_H_ */
