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
	hal_bitmask_set(CORE_CM, CM_ICLKEN1_CORE, BV(24));

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

    // enable interrupts
    // TODO: check if all important one are set
    hal_bitmask_set(MMCHS1, MMCHS_IE, BV(20));       // data timeout error
    hal_bitmask_set(MMCHS1, MMCHS_IE, BV(19));       // cmd index error
    hal_bitmask_set(MMCHS1, MMCHS_IE, BV(16));       // cto enable error
    hal_bitmask_set(MMCHS1, MMCHS_IE, BV(1));        // TC (transfer completed) interrupt
    hal_bitmask_set(MMCHS1, MMCHS_IE, BV(0));        // CC (command completed) interrupt

    // mmc host/bus configuration [22.5.1.5]
    // MMCHS_CON to configure specific data/cmd transfer
    hal_bitmask_clear(MMCHS1, MMCHS_CON, BV(5));    // set DW8 (8 bit mode MMC) to 0 for SD/SDIO cards
    hal_bitmask_clear(MMCHS1, MMCHS_CON, BV(12));	// CEATA mode to standard

    // configure card voltage, pwr mode and data bus width
    //hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(11));		// 0x5: 1.8V (Typical)
    //hal_bitmask_clear(MMCHS1, MMCHS_HCTL, BV(10));		// 0x5: 1.8V (Typical)
    //hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(9));			// 0x5: 1.8V (Typical)
    hal_bitmask_set(MMCHS1, MMCHS_HCTL, (0x6 << 9));		// 0x6 3V

    //hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(8));			// SD bus power on (SDBP)
    hal_bitmask_set(MMCHS1, MMCHS_HCTL, (0x0 << 8));		// SD bus power on (SDBP)
    hal_bitmask_clear(MMCHS1, MMCHS_HCTL, BV(1));		// data transfer width to 1 bit (DTW)

    // Enable the input buffer of the clock output.
    // As a result, there is a loopback through the
    // output and input buffers. The goal is to
    // support the synchronization of the mmci_clk.
    // see page 777

    hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(0));		// enable internal clock by setting bit 0

    // update clock
    /*hal_bitmask_clear(MMCHS1, MMCHS_SYSCTL, BV(2)); 	// set unstable
    hal_bitmask_write(MMCHS1, MMCHS_SYSCTL, (0xD << 6), 32);
    while(BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(8)), 1) != 1) {}	// wait until clk is set
    hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(2));		// set stable*/
	mmcsd_change_clockfrequency(MMCHS_CLK_FRQCY80);

    hal_bitmask_set(MMCHS1, MMCHS_HCTL, (0x1 << 8));		// SD bus power on (SDBP)

    // read back the SDBP bit
    while(BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_HCTL) & BV(8)), 8) != 1) {}

    // idle mode
    /*hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(9)); 	// interface and fkt clocks are maintained
    hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(8)); 	// interface and fkt clocks are maintained
    hal_bitmask_clear(MMCHS1, MMCHS_SYSCONFIG, BV(4));	// pwr management
    hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(3));	// pwr management
    hal_bitmask_clear(MMCHS1, MMCHS_SYSCONFIG, BV(0));	// autoidle clock strgy: free running*/

	// precard identification procedure (p. 3162)
	mmcsd_precard_identification();

	//omap3530_mmchs_card_specific_data

	//  omap3530_mmchs_perform_card_configuration

	// check media data
}

void mmcsd_precard_identification()
{
	// p. 3147
	hal_bitmask_set(MMCHS1, MMCHS_CON, BV(1)); // send init sequence
	hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x00000000, 32);	// send dummy command

	// wait for 1ms
	util_ksleep(1);

	hal_bitmask_set(MMCHS1, MMCHS_STAT, BV(0));	// set 1 to clear flag
	hal_bitmask_clear(MMCHS1, MMCHS_CON, BV(1)); // stop sending init sequence
	hal_bitmask_write(MMCHS1, MMCHS_STAT, 0xFFFFFFFF, 32);	// clear stat register

	// change clock frequency to fit protocol
	mmcsd_change_clockfrequency(MMCHS_CLK_FRQCY400);

	// send cmd0
	mmcsd_sendcmd(MMCHS_CMD0);

	// send cmd5
	mmcsd_sendcmd(MMCHS_CMD5);

	// read MMCHS_STAT and check CC and CTO
	// cto = cmd timeout error
	// cc = command complete
	int cto;
	int cc;

	do {
		cc = hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(0);
		cto = hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(16);

		if(cc == 1) {
			// it is an sdio-card!
			break;
		}
	} while (cto != 1);

	// check again
	// it is an SDIO-card
	if (cc == 1)
	{

	}
	else
	{
		hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(25)); // set to 1
		while ( BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(25)), 25) != 1) {}	// wait until it returns 0x0

		// send cmd8
		mmcsd_sendcmd(MMCHS_CMD8);

		// read MMCHS_STAT register
		cc = 0;
		cto = 0;
		do {
			cc = hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(0);
			cto = hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(16);

			if(cc == 1) {
				// standard SD card compliant 2.0
				break;
			}
		} while (cto != 1);

		// it is SD card compliant with standard 2.0 or later
		if (cc == 1)
		{
			// standard specifiction
			// END
		}
		else
		{
			hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(25)); // set to 1
			while( BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(25)), 25) != 1) {}	// wait until it returns 0x0

			// do until card is not busy anymore
			int value_rsp10;
			int loopCount = 0;
			do {
				// send cmd55
				mmcsd_sendcmd(MMCHS_CMD55);

				// send ACMD41 command
				// TODO

				// read MMCHS_STAT register
				cc = 0;
				cto = 0;
				do {
					cc = hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(0);
					cto = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(16)), 16);

					if(cc == 1) {
						// standard SD card compliant 2.0
						break;
					}
				} while (cto != 1);

				// SD card compliant with standard 1.x
				if (cc == 1)
				{
					// verify if card is busy
					value_rsp10 = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_RSP10) & BV(31)), 31);
				}
				else
				{
					// it is an MMC card
					// will be implemented later
				}

				loopCount++;
			} while(value_rsp10 != 1 || loopCount < MAX_RETRY);

			// send cmd2 to gather information about how to read sd card
			mmcsd_sendcmd(MMCHS_CMD2);

			// send cmd3
			mmcsd_sendcmd(MMCHS_CMD3);

			// TODO: handle mmc card
			// if mmc card -> more than one card connected? if only 1 or all are identified, go on then

			// send cmd7
			mmcsd_sendcmd(MMCHS_CMD7);
		}
	}

}

void mmcsd_change_clockfrequency(uint32_t clockfrequency)
{
	// see p. 3157
	// set CEN bit to 0x0 to not provide the clock to card
	hal_bitmask_clear(MMCHS1, MMCHS_SYSCTL, BV(2));

	// set clock divider
	// MMCi.MMCHS_SYSCTL[15:6]CLKD
	hal_bitmask_clear(MMCHS1, MMCHS_SYSCTL, (0x3FFUL << 6)); // clear bit 6 to 15
	hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, (clockfrequency << 6));

	// wait until clk is set and stable
    while( BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(1)), 1) != 1) {}

    // provide the clock to the card
    hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(2));
}

void mmcsd_sendcmd(uint32_t cmd)
{
	// see p.3151 resp. 3155
	// read/write transfer flow with polling and without DMA

	// until cmd lines are not in use anymore
	while( (hal_get_address_value(MMCHS1, MMCHS_PSTATE) & BV(0)) != 0) { }

	// Set Bitfields in MMCHS_CON
	// set MIT, STR, but only for mmc cards (not gonna happen)

	// write MMCHS_CSRE if response type permits
	// TODO: set bit for error code card status
	//hal_bitmask_write(MMCHS1, MMCHS_CSRE, 0x00000000);

	// Set Blocksize and number of Blocks
	hal_bitmask_write(MMCHS1, MMCHS_BLK, BV(9), 8); // 512 bytes (block length)
	//hal_bitmask_write(MMCHS1, MMCHS_BLK, BV(16)); // possible TODO: 0 no block transferred, 1 block (block count)

	// MMCHS_ARG default
	hal_bitmask_write(MMCHS1, MMCHS_ARG, 0x00000000, 8);

	// if interrupts are used: MMCHS_ISE
	// set to default?

	// Write command
	// MMCHS_CMD and MMCHS_ISE
	switch(cmd)
	{
		case 1:
		{
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x01020000, 8);
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x00050001, 8);
		}
		break;

		case 2:
		{
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x02090000, 8);
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x00070001, 8);
		}
		break;

		case 3:
		{
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x031a0000, 8);
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x100f0001, 8);
			hal_bitmask_write(MMCHS1, MMCHS_ARG, 0x00010000, 8);
		}
		break;

		case 7:
		{
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x071a0000, 8);
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x100f0001, 8);
			hal_bitmask_write(MMCHS1, MMCHS_ARG, 0x00010000, 8);
		}
		break;

		case 9:
		{
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x09090000, 8);
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x00070001, 8);
			hal_bitmask_write(MMCHS1, MMCHS_ARG, 0x00010000, 8);
		}
		break;
	}

	// read MMCHS_STAT register
	uint32_t mmcStatus;
	do {
		mmcStatus = hal_get_address_value(MMCHS1, MMCHS_STAT);
	} while(mmcStatus == 0);

	// todo: fix status, not set!

	while( ( (hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(0)) != 1) && ( (hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(16)) != 1)) { }
	int cc = hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(0);
	int cto = hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(16);
	int ccrc = hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(17);

	// conflict occured on mmci_cmd line
	if (cto && ccrc)
	{
		// set MMCHS_SYSCTL[25] SRC bit to 0x1 and wait until it returns 0x0
		hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(25));
		while( BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(25)), 25) != 1 ) { }
	}
	else
	{
		if (cto && !ccrc)
		{
			// set MMCHS_SYSCTL[25] SRC bit to 0x1 and wait until it returns 0x0
			hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(25));
			while( BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(25)), 25) != 1 ) { }
		}
		else if (cc)
		{
			// read response type
			// RESP_TYPE = MMCHS_CMD[17:16]
			int RESP_TYPE = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_CMD) & (BV(17) | BV(16))), 16);

			// a response is waiting
			if(RESP_TYPE != 0x0)
			{
				// read MMCHS_RSP
				int mmchsResponse = hal_get_address_value(MMCHS1, MMCHS_RSP10);

				// read MMCHS_STAT CIE, CEB, CCRC and CERR
				int CIE = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(19)), 19);
				int CEB = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(18)), 18);
				int CCRC = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(17)), 16);
				int CERR = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(28)), 28);
			}
		}
	}
}


void mmcsd_softreset()
{
    // set softreset bit to 1
    hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(1));
    // wait for reset done
    while( (hal_get_address_value(MMCHS1, MMCHS_SYSSTATUS) & BV(0)) != 1) {}
}
