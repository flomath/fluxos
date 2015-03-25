/*
 * process.c
 *
 *  Created on: 25.03.2015
 *      Author: florian
 */

#include "process.h"

static uint8_t unique_process_id = 0;


Process_t* process_create()
{
	Process_t* process;
	process = malloc(sizeof(Process_t));
	process->id = process_next_id();
	process->state = PROCESS_CREATED;

	return process;
}

uint8_t process_next_id()
{
	return ++unique_process_id;
}
