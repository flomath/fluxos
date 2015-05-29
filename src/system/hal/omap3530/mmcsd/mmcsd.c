/*
 * mmcsd.c
 *
 *  Created on: 29.05.2015
 *      Author: florian
 */

#include "mmcsd.h"

void mmcsd_init()
{
    // configure interface and functional clocks
    // p. 3161

    // softreset mmc/sd controller
    mmscd_softreset();

    // set default capabilities [22.5.1.3]
    // all settings are for MMCHS1 [p. 3206]
    hal_bitmask_set(MMCHS1, MMCHS_CAPA, BV(26));    // Voltage Support 1.8V must be set to 1
    hal_bitmask_set(MMCHS1, MMCHS_CAPA, BV(25));    // Voltage Support 3.0V must be set to 1
    hal_bitmask_clear(MMCHS1, MMCHS_CAPA, BV(24));  // Voltage Support 3.3V must be set to 0

    // Maximum current capabilities
    hal_bitmask_clear(MMCHS1, MMCHS_CUR_CAPA, 0xFF);

    // todo: set currents

    // wakeup configuration [22.5.1.4]
    hal_bitmask_set(MMCHS1, MMCHS_CAPA, BV(2));     // enwakup
    hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(24));    // wakeup event on sd card interrupt
    hal_bitmask_set(MMCHS1, MMCHS_IE, BV(8));       // enable card interrupt (sdio card only!)

    // mmc host/bus configuration [22.5.1.5]



}

void mmcsd_softreset()
{
    // set softreset bit to 1
    hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(1));

    // wait for reset done
    while(hal_get_address_value(MMCHS1, MMCHS_SYSSTATUS) & BV(0) != 1) {}
}