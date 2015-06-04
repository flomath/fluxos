/*
 * mcbsp.h
 *
 *  Created on: 14.05.2015
 *      Author: florian
 *      Modified: philip (03.06.2015)
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_MCBSP_MCBSP_H_
#define SRC_SYSTEM_HAL_OMAP3530_MCBSP_MCBSP_H_


#include "../../common/hal.h"

#define	MCBSP1		0x48074000
#define MCBSP5		0x48096000
#define MCBSP2		0x49022000
#define MCBSP3		0x49024000
#define MCBSP4		0x49026000

///< supports 8/16/32 bit data
#define MCBSPLP_DRR_REG		0x000 ///< data receive register
#define MCBSPLP_DXR_REG		0x008 ///< data transmit register

///< only supports 32-bit data!
#define MCBSPLP_SPCR1_REG	0x014 ///< Serial Port Control Register [21-50]
#define MCBSPLP_SPCR2_REG 	0x010 ///< Serial Port Control Register 2 [21-48]
#define MCBSPLP_PCR_REG		0x048 ///< Pin Control Register [21-76]
#define MCBSPLP_RCR1_REG	0x01C ///< Receive Control Register [21-54]
#define MCBSPLP_RCR2_REG	0x018 ///< Receive Control Register 2 [21-52]
#define MCBSPLP_XCR1_REG	0x024 ///< Transmit Control Register [21-58]
#define MCBSPLP_XCR2_REG	0x020 ///< Transmit Control Register 2 [21-56]
#define MCBSPLP_SRGR1_REG	0x02C ///< Sample Rate Generator [21-62]
#define MCBSPLP_SRGR2_REG	0x028 ///< Sample Rate Generator [21-60]

#define CONTROL_PADCONF_MCBSP2_FSX	0x4800213C
#define CONTROL_PADCONF_MCBSP2_DR	0x48002140

#define MCBSPLP_MCR1_REG	0x034 ///< multichannel control

/**
 *
 */
void mcbsp2_enable(void);

/**
 * Initialize the device as master
 * @param mcbsp The MCBSP address to initialize
 */
void mcbsp_init_master(uint32_t mcbsp);

#endif /* SRC_SYSTEM_HAL_OMAP3530_MCBSP_MCBSP_H_ */
