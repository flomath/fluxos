/*
 * mmcsd.h
 *
 *  Created on: 29.05.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_MMCSD_H_
#define SRC_SYSTEM_HAL_OMAP3530_MMCSD_H_

#include "../../common/hal.h"
#include "../clock/clock.h"

#define MMCHS1      0x4809C000
#define MMCHS2      0x480B4000
#define MMCHS3      0x480AD000

#define MMCHS_SYSCONFIG     0x010
#define MMCHS_SYSSTATUS     0x014
#define MMCHS_CSRE          0x024
#define MMCHS_SYSTEST       0x028
#define MMCHS_CON           0x02C
#define MMCHS_PWCNT         0x030
#define MMCHS_SDMASA        0x100
#define MMCHS_BLK           0x104
#define MMCHS_ARG           0x108
#define MMCHS_CMD           0x10C
#define MMCHS_DATA          0x120
#define MMCHS_PSTATE        0x124
#define MMCHS_HCTL          0x128
#define MMCHS_SYSCTL        0x12C
#define MMCHS_STAT          0x130
#define MMCHS_IE            0x134
#define MMCHS_ISE           0x138
#define MMCHS_AC12          0x13C
#define MMCHS_CAPA          0x140
#define MMCHS_CUR_CAPA      0x148
#define MMCHS_FE            0x150
#define MMCHS_ADMAES        0x154
#define MMCHS_ADMASAL       0x158
#define MMCHS_REV           0x1FC

#define MMCHS_RSP(n)        (0x210 + (n * 4))
#define MMCHS_RSP10         0x110

#define CONTROL_PADCONF_MMC1_CLK	0x48002144	// mmc1_clk and mmc1_cmd
#define CONTROL_PADCONF_MMC1_DAT0	0x48002148
#define CONTROL_PADCONF_MMC1_DAT1	0x48002148
#define CONTROL_PADCONF_MMC1_DAT2	0x4800214C
#define CONTROL_PADCONF_MMC1_DAT4	0x48002150
#define CONTROL_PADCONF_MMC1_DAT6	0x48002154

#define MMCHS_CMD0      0
#define MMCHS_CMD2      2
#define MMCHS_CMD3      3
#define MMCHS_CMD5      5
#define MMCHS_CMD7      7
#define MMCHS_CMD8      8
#define MMCHS_CMD55      55

/**
 * initialize mmc/sd controller
 */
void mmcsd_init();

/**
 * soft-reset the mmc/sd controller
 */
void mmcsd_softreset();

/**
 * pre card identifaction procedure
 */
void mmcsd_precard_identification();

/**
 * send a command
 * a command starts an operation (serially transfered from sd host ctrl to the card on the mmci_cmd line)
 */
void mmcsd_sendcmd(uint32_t cmd);

#endif /* SRC_SYSTEM_HAL_OMAP3530_MMCSD_H_ */
