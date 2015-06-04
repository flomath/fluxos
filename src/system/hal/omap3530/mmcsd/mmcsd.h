/*
 * mmcsd.h
 *
 *  Created on: 29.05.2015
 *      Author: florian
 */

#ifndef SRC_SYSTEM_HAL_OMAP3530_MMCSD_H_
#define SRC_SYSTEM_HAL_OMAP3530_MMCSD_H_

#define MMCHS1      0x4809 C000
#define MMCHS2      0x480B 4000
#define MMCHS3      0x480A D000

#define MMCHS_HL_REV        0x0
#define MMCHS_HL_HWINFO     0x4
#define MMCHS_HL_SYSCONFIG  0x10
#define MMCHS_SYSCONFIG     0x110
#define MMCHS_SYSSTATUS     0x114
#define MMCHS_CSRE          0x124
#define MMCHS_SYSTEST       0x128
#define MMCHS_CON           0x12C
#define MMCHS_PWCNT         0x130
#define MMCHS_SDMASA        0x200
#define MMCHS_BLK           0x204
#define MMCHS_ARG           0x208
#define MMCHS_CMD           0x20C
#define MMCHS_RSP(n)        (0x210 + (n * 4))
#define MMCHS_DATA          0x220
#define MMCHS_PSTATE        0x224
#define MMCHS_HCTL          0x228
#define MMCHS_SYSCTL        0x22C
#define MMCHS_STAT          0x230
#define MMCHS_IE            0x234
#define MMCHS_ISE           0x238
#define MMCHS_AC12          0x23C
#define MMCHS_CAPA          0x240
#define MMCHS_CUR_CAPA      0x248
#define MMCHS_FE            0x250
#define MMCHS_ADMAES        0x254
#define MMCHS_ADMASAL       0x258
#define MMCHS_REV           0x2FC

/**
 * initialize mmc/sd controller
 */
void mmcsd_init();

/**
 * soft-reset the mmc/sd controller
 */
void mmcsd_softreset();

#endif /* SRC_SYSTEM_HAL_OMAP3530_MMCSD_H_ */
