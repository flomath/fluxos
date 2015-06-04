/*
 * interrupt_sw.c
 *
 *  Created on: 23.05.2015
 *      Author: florian
 */

#include "interrupt_sw.h"

void handle_interrupt_sw(uint32_t swiID, uint32_t params[])
{
	switch(swiID)
	{
		case SYS_DEBUG:
			printf("debug swi called\n");
			break;

		default:
			break;
	}
}
