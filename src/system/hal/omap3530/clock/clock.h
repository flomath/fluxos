/*
 * clock.h
 *
 *  Created on: 13.05.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_CLOCK_CLOCK_H_
#define SRC_SYSTEM_HAL_OMAP3530_CLOCK_CLOCK_H_

#include "../../common/hal.h"
#include "../../common/clock.h"

#define MPU_CM      0x48004900
#define CORE_CM     0x48004A00
#define WKUP_CM     0x48004C00
#define PER_CM      0x48005000

// wakeup
#define CM_FCLKEN_WKUP    0x000
#define CM_ICLKEN_WKUP    0x010
#define CM_IDLEST_WKUP    0x020
#define CM_AUTOIDLE_WKUP  0x030
#define CM_CLKSEL_WKUP    0x040

// per
#define CM_FCLKEN_PER		0x000		///< Controls the modules functional clock activity
#define CM_ICLKEN_PER		0x010		///< Controls the modules interface clock activity
#define CM_CLKSEL_PER		0x040	 	///< PER domain modules source clock selection
#define CM_IDLEST_PER     	0x020
#define CM_AUTOIDLE_PER   	0x030
#define CM_SLEEPDEP_PER   	0x044
#define CM_CLKSTCTRL_PER  	0x048
#define CM_CLKSTST_PER    	0x04C

// core
#define CM_FCLKEN1_CORE		0x48004A00		// Controls the modules functional clock activity [4-129]
#define CM_ICLKEN1_CORE		0x48004A10		// Controls the modules interface clock activity [4-133]

#define CONTROL				0x48002274
#define CONTROL_DEVCONF0	0x004

// CORE
//TODO: after merging check this one important!!!
//#define CM_FCLKEN_CORE    0x000
//#define CM_ICLKEN1_CORE   0x010

#endif /* SRC_SYSTEM_HAL_OMAP3530_CLOCK_CLOCK_H_ */
