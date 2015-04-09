/*
 * timer.h
 *
 *  Created on: 19.03.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_TIMER_TIMER_H_
#define SRC_SYSTEM_HAL_OMAP3530_TIMER_TIMER_H_

#include<stdint.h>
#include "../../common/hal.h"
#include "../../common/timer.h"
#include "../prcm/percm.h"

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

#define GPT_TIER        0x01C    ///< enable or disable interrupt events [16-64]
#define GPT_TCLR        0x024    ///< optional features specific to timer functionality [16-28]
#define GPT_TCRR        0x028    ///< value of internal counter [16-30]
#define GPT_TLDR        0x02C    ///< timer load values [16-32]
#define GPT_TISR        0x02C    ///< shows which interrupt events are pending inside module [16-22]
#define GPT_TTGR        0x030    ///< triggers a counter reload of timer by writing any value in it [16-34]
#define GPT_TPIR        0x048    ///< value of positive increment for 1ms tick generation [16-46]
#define GPT_TNIR        0x04C    ///< value of negative increment for 1ms tick generation [16-48]
#define GPT_TOCR        0x054    ///< mask the tick interrupt for a selected number of ticks [16-52]
#define GPT_TOWR        0x058    ///< number of masked overflow interrupts [16-54]
#define GPT_TWER        0x020    ///< controls the wake-up feature on spec. interrupt events [16-26]
#define GPT_TMAR		0x038	 ///< holds the value to be compared with the counter value [16-38]

#define GPT_FCLK        32.768   ///< timer input clock is 32.768 Hz [16.2.4.2.1]
#define GPT_TICK        1        ///< Interupt period 1ms [16.2.4.2.1]
#define GPT_LRGNR       1000000  ///< Large Number (1e6)
#define GPT_TIOCP_CFG	0x010	 ///< Register controls various parameters of GP timer L4 interface

#endif /* SRC_SYSTEM_HAL_OMAP3530_TIMER_TIMER_H_ */
