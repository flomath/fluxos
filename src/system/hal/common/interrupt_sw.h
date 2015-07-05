/*
 * syscall.h
 *
 *  Created on: 23.05.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_COMMON_INTERRUPT_SW_H_
#define SRC_SYSTEM_HAL_COMMON_INTERRUPT_SW_H_

#include "../../system.h"
#include "../../../api/syscall.h"

void handle_interrupt_sw(uint32_t swiID, uint32_t params[], uint32_t paramLength);

#endif /* SRC_SYSTEM_HAL_COMMON_INTERRUPT_SW_H_ */
