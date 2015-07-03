/*
 * syscall.h
 *
 * Created on: 02.07.15
 *     Author: Nino Schoch
 */

#ifndef SRC_API_SYSCALL_H
#define SRC_API_SYSCALL_H

#include <inttypes.h>

#define SYS_DEBUG		10
#define SYS_EXIT	    20
#define SYS_PRINT       30
#define SYS_LOAD_PROC   40

#pragma SWI_ALIAS(syscall, 0)
void syscall(uint32_t swiID, uint32_t params[], unsigned int paramLength);

#endif //SRC_API_SYSCALL_H
