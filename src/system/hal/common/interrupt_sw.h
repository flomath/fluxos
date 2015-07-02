/*
 * syscall.h
 *
 *  Created on: 23.05.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_COMMON_INTERRUPT_SW_H_
#define SRC_SYSTEM_HAL_COMMON_INTERRUPT_SW_H_

#include <stdio.h>
#include <stdint.h>
#define SYS_DEBUG		10
#define SYS_EXIT	    20

#pragma SWI_ALIAS(syscall, 0)
void syscall(uint32_t swiID, uint32_t params[]);

void handle_interrupt_sw(uint32_t swiID, uint32_t params[]);

extern void _switch_mode(uint32_t mode);

#endif /* SRC_SYSTEM_HAL_COMMON_INTERRUPT_SW_H_ */
