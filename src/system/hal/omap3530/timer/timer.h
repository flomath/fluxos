/*
 * timer.h
 *
 *  Created on: 19.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_TIMER_TIMER_H_
#define SRC_SYSTEM_HAL_OMAP3530_TIMER_TIMER_H_

// General Purpose Timers, each 4K bytes [16.3.1.1]
// all timers are mmaped to the L4 memory space
#define GPT_TIMER1		0x48318000
#define GPT_TIMER2		0x49032000
#define GPT_TIMER3		0x49034000
#define GPT_TIMER4		0x49036000
#define GPT_TIMER5		0x49038000
#define GPT_TIMER6		0x4903A000
#define GPT_TIMER7		0x4903C000
#define	GPT_TIMER8		0x4903E000
#define GPT_TIMER9		0x49040000
#define GPT_TIMER10		0x48086000
#define GPT_TIMER11		0x48088000

#endif /* SRC_SYSTEM_HAL_OMAP3530_TIMER_TIMER_H_ */
