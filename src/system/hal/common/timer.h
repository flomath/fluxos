/*
 * timer common interface
 *
 *  Created on: 19.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_COMMON_TIMER_TIMER_H_
#define SRC_SYSTEM_HAL_COMMON_TIMER_TIMER_H_

#include<stdint.h>

/**
 * Initialise the timer
 */
void gpt_timer_init(uint32_t timer);

/**
 * Timer can be started at any time [16.2.4.2]
 */
void gpt_timer_start(uint32_t timer);

/**
 * Timer can be stopped at any time [16.2.4.2]
 */
void gpt_timer_stop(uint32_t timer);

/**
 * Disable Interrupts for timer
 */
void gpt_disable_interrupts(uint32_t timer);

#endif /* SRC_SYSTEM_HAL_COMMON_TIMER_TIMER_H_ */