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
		hal_bitmask_set(CORE_CM, CM_FCLKEN_CORE, BV(24));
		hal_bitmask_set(CORE_CM, CM_ICLKEN_CORE, BV(24));

    // softreset mmc/sd controller
		mmcsd_softreset();

    // set default capabilities [22.5.1.3]
    // all settings are for MMCHS1 [p. 3206]
    hal_bitmask_set(MMCHS1, MMCHS_CAPA, BV(26));    // Voltage Support 1.8V must be set to 1
    hal_bitmask_set(MMCHS1, MMCHS_CAPA, BV(25));    // Voltage Support 3.0V must be set to 1
    hal_bitmask_clear(MMCHS1, MMCHS_CAPA, BV(24));  // Voltage Support 3.3V must be set to 0

    // Maximum current capabilities
    hal_bitmask_clear(MMCHS1, MMCHS_CUR_CAPA, 0xFF);

    // set max currents (p. 3208)
		//hal_bitmask_set(MMCHS1, MMCHS_CUR_CAPA, BV(26));

    // wakeup configuration [22.5.1.4]
    hal_bitmask_set(MMCHS1, MMCHS_CAPA, BV(2));     // enwakup
    hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(24));    // wakeup event on sd card interrupt
    hal_bitmask_set(MMCHS1, MMCHS_IE, BV(8));       // enable card interrupt (sdio card only!)

    // mmc host/bus configuration [22.5.1.5]
    // MMCHS_CON to configure specific data/cmd transfer
    hal_bitmask_clear(MMCHS1, MMCHS_CON, BV(5));    // set DW8 (8 bit mode MMC) to 0 for SD/SDIO cards
    hal_bitmask_clear(MMCHS1, MMCHS_CON, BV(12));	// CEATA mode to standard

    // configure card voltage, pwr mode and data bus width
    hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(11));		// 0x5: 1.8V (Typical)
    hal_bitmask_clear(MMCHS1, MMCHS_HCTL, BV(10));		// 0x5: 1.8V (Typical)
    hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(9));			// 0x5: 1.8V (Typical)

    hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(8));		// SD bus power on
    hal_bitmask_clear(MMCHS1, MMCHS_HCTL, BV(1));		// data transfer width to 1 bit

    // read back the SDBP bit
    while(hal_get_address_value(MMCHS1, MMCHS_HCTL) & BV(8) != 1) {}

    hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(0));		// enable internal clock by setting bit 0

    // Enable the input buffer of the clock output.
    // As a result, there is a loopback through the
    // output and input buffers. The goal is to
    // support the synchronization of the mmci_clk.
    // see page 777

    // update clock
    hal_bitmask_clear(MMCHS1, MMCHS_SYSCTL, BV(2)); 	// set unstable
    hal_bitmask_write(MMCHS1, MMCHS_SYSCTL, (0xD << 6), 32);
    while(hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(1) != 1) {}	// wait until clk is set
    hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(2));		// set stable

    // idle mode
    hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(9)); 	// interface and fkt clocks are maintained
    hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(8)); 	// interface and fkt clocks are maintained
    hal_bitmask_clear(MMCHS1, MMCHS_SYSCONFIG, BV(4));	// pwr management
    hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(3));	// pwr management
    hal_bitmask_clear(MMCHS1, MMCHS_SYSCONFIG, BV(0));	// autoidle clock strgy: free running

		// precard identification procedure (p. 3162)
		mmcsd_precard_identification();
}

void mmcsd_precard_identification()
{
	// p. 3147
	hal_bitmask_set(MMCHS1, MMCHS_CON, BV(1)); // send init sequence
	hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x00000000, 32);	// send dummy command

	// wait for 1ms

	hal_bitmask_clear(MMCHS1, MMCHS_STAT, BV(0));	// clear flag
	hal_bitmask_clear(MMCHS1, MMCHS_CON, BV(1)); // stop sending init sequence
	hal_bitmask_write(MMCHS1, MMCHS_STAT, 0xFFFFFFFF, 32);	// clear stat register

	// change clock frequency to fit protocol

	// send cmd0
	mmcsd_sendcmd(MMCHS_CMD0);

	// send cmd5
	mmcsd_sendcmd(MMCHS_CMD5);

	// read MMCHS_STAT and check CC and CTO
	// cto = cmd timeout error
	// cc = command complete
	while((hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(16) != 1) && (hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(16) != 1)) { }
}

void mmcsd_sendcmd(uint32_t cmd)
{
	switch(cmd)
	{
		case 1:
		{
				// what to do?
		}
		break;
	}
}

void mmcsd_softreset()
{
    // set softreset bit to 1
    hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(1));
    // wait for reset done
    while(hal_get_address_value(MMCHS1, MMCHS_SYSSTATUS) & BV(0) != 1) {}
}
