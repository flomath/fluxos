/*
 * kernel.h
 *
 *  Created on: 05.07.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_KERNEL_H_
#define SRC_SYSTEM_KERNEL_H_

#include "../utils/sleep.h"
#include "hal/omap3530/timer/timer.h"
#include "hal/omap3530/interrupt/interrupt.h"
#include "driver/uart/UartDriver.h"
#include "driver/button/ButtonDriver.h"
#include "driver/gpio/GPIODriver.h"
#include "scheduler/scheduler.h"

#include "driver/mcbsp/McbspDriver.h"
#include "../console/console.h"
#include "hal/omap3530/mmcsd/mmcsd.h"

void kernel_boot();
static void kernel_enable_audio();
static void timer_irq(Registers_t* context);

#endif /* SRC_SYSTEM_KERNEL_H_ */
