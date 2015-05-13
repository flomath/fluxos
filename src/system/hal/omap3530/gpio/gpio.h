/*
 * timer.h
 *
 *  Created on: 13.05.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_GPIO_GPIO_H_
#define SRC_SYSTEM_HAL_OMAP3530_GPIO_GPIO_H_

#include "../../common/hal.h"
#include "../../common/gpio.h"
#include "../clock/clock.h"

// General Purpose Input/Ouput, each 4K bytes [24.6]
// all GPIOs are mmaped to the L4 memory space
#define GPIO1     0x48310000
#define GPIO2     0x49050000
#define GPIO3     0x49052000
#define GPIO4     0x49054000
#define GPIO5     0x49056000
#define GPIO6     0x49058000

#define GPIO_REVISION       0x000    ///< contains IP revision code [24-9]
#define GPIO_SYSCONFIG      0x010    ///< controls various parameters of L4 [24-11]
#define GPIO_SYSSTATUS      0x014    ///< status info about module (exl. irq status) [24-13]
#define GPIO_IRQSTATUS1     0x018    ///< IRQ1 status info [24-15]
#define GPIO_IRQENABLE1     0x01C    ///< IRQ1 enable info [24-17]
#define GPIO_WAKEUPENABLE   0x020    ///< wake-up enable info [24-19]
#define GPIO_IRQSTATUS2     0x028    ///< IRQ2 status info [24-21]
#define GPIO_IRQENABLE2     0x02C    ///< IRQ2 enable info [24-23]
#define GPIO_CTRL           0x030    ///< controls clock gating [24-25]
#define GPIO_OE             0x034    ///< enable pins output capabilities [24-27]
#define GPIO_DATAIN         0x038    ///< read data from GPIO pins [24-29]
#define GPIO_DATAOUT        0x03C    ///< set value of GPIO pin [24-31]
#define GPIO_LEVELDETECT0   0x040    ///< enable/disable for each input lines the low-level (0) [24-33]
#define GPIO_LEVELDETECT1   0x044    ///< -- high-level (1) [24-34]
#define GPIO_RISINGDETECT   0x048    ///< -- rising-edge (0 => 1) [24-37]
#define GPIO_FALLINGDETECT  0x04C    ///< -- falling-edge (1 => 0) [24-39]
#define GPIO_DEBOUNCEENABLE 0x050    ///< enable/disable debouncing feature for each input line [24-41]
#define GPIO_DEBOUNCINGTIME 0x054    ///< controls debouncing time [24-43]
#define GPIO_CLEARIRQENABLE1  0x060    ///< clear to 0 [24-45]
#define GPIO_SETIRQENABLE1  0x060    ///< set to 1 [24-47]
#define GPIO_CLEARIRQENABLE2  0x070    ///< clear to 0 [24-49]
#define GPIO_SETIRQENABLE2  0x074    ///< set to 1 [24-51]
#define GPIO_CLEARWKUENA    0x080    ///< clear to 0 [24-53]
#define GPIO_SETWKUENA      0x084    ///< set to 1 [24-55]
#define GPIO_CLEARDATAOUT   0x090    ///< clear to 0 [24-57]
#define GPIO_SETDATAOUT     0x094    ///< set to 1 [24-59]

#endif /* SRC_SYSTEM_HAL_OMAP3530_GPIO_GPIO_H_ */
