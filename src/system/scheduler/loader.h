/*
 * loader.h
 *
 *  Created on: 10.06.2015
 *      Author: Philip
 */

#ifndef SRC_SYSTEM_SCHEDULER_LOADER_H_
#define SRC_SYSTEM_SCHEDULER_LOADER_H_

#include "../system.h"

#define CODE_START 0x00150000
#define CODE_MAIN_OFFSET 0xc

/**
 * Starts a new process using the given binary data
 *
 * @param address The start address of the process data
 * @param size	The length of the data
 */
void loader_load_process(uint32_t* address, size_t size);



#endif /* SRC_SYSTEM_SCHEDULER_LOADER_H_ */
