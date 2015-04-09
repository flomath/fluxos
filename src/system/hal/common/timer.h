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
void gpt_timer_init(uint32_t timer, int timeInMs);

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

/**
 * Reset the timer on each irq handler call!
 */
void gpt_timer_reset(uint32_t timer);

/**
 * Select the clock for given timer (either system or 32k)
 */
void gpt_select_clock();

void gpt_enable_features(uint32_t timer);

#endif /* SRC_SYSTEM_HAL_COMMON_TIMER_TIMER_H_ */
