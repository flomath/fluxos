/*
 * mcbsp.h
 *
 *  Created on: 14.05.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_MCBSP_MCBSP_H_
#define SRC_SYSTEM_HAL_OMAP3530_MCBSP_MCBSP_H_

#define	MCBSP1		0x48074000
#define MCBSP2		0x48096000
#define MCBSP3		0x49022000
#define MCBSP4		0x49026000

///< supports 8/16/32 bit data
#define MCBSPLP_DRR_REG		0x000 ///< data receive register
#define MCBSPLP_DXR_REG		0x008 ///< data transmit register

///< only supports 32-bit data!
#define MCBSPLP_SPCR1_REG	0x014 ///< serial port control
#define MCBSPLP_RCR1_REG	0x01C ///< receive control register
#define MCBSPLP_XCR1_REG	0x024 ///< transmit control register
#define MCBSPLP_SRGR1_REG	0x02C ///< sample rate generator
#define MCBSPLP_MCR1_REG	0x034 ///< multichannel control


#endif /* SRC_SYSTEM_HAL_OMAP3530_MCBSP_MCBSP_H_ */
