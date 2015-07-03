/*
 * mmcsd.c
 *
 *  Created on: 29.05.2015
 *      Author: florian
 */

#include "mmcsd.h"

CARD_INFO cardInfo;
EFI_BLOCK_MEDIA mmchsMedia = {
	1704,
	true,
    false,
	false,
	false,
	false,
	512,
	0
};

void mmcsd_configure_clocks() {

    // configure interface and functional clocks
    // p. 3161
	hal_bitmask_set(CORE_CM, CM_FCLKEN_CORE, BV(24));
	hal_bitmask_set(CORE_CM, CM_ICLKEN1_CORE, BV(24));

	memset(&cardInfo, 0, sizeof(cardInfo));
}

void mmcsd_card_detect()
{
    // p. 3161;

    // softreset mmc/sd controller
	mmcsd_softreset();

	// Softreset MMC controller
	hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(1));
	uint32_t softResetMMC;
	do {
		// wait until it returns 0x0
		softResetMMC = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_SYSCONFIG) & BV(1)), 1);
	} while ( softResetMMC != 0 );

	// Softreset for all
	hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(24)); // set to 1
	uint32_t softResetAllDone;
	do {
		// wait until it returns 0x0
		softResetAllDone = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(24)), 24);
	} while ( softResetAllDone != 0 );

    // set default capabilities [22.5.1.3]
    // all settings are for MMCHS1 [p. 3206]
    hal_bitmask_set(MMCHS1, MMCHS_CAPA, BV(26));    // Voltage Support 1.8V must be set to 1
    hal_bitmask_set(MMCHS1, MMCHS_CAPA, BV(25));    // Voltage Support 3.0V must be set to 1
    //hal_bitmask_clear(MMCHS1, MMCHS_CAPA, BV(24));  // Voltage Support 3.3V must be set to 0

    // Maximum current capabilities
    //hal_bitmask_clear(MMCHS1, MMCHS_CUR_CAPA, 0xFF);

    // set max currents (p. 3208)
	//hal_bitmask_set(MMCHS1, MMCHS_CUR_CAPA, BV(26));

    // wakeup configuration [22.5.1.4]
    hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(2));     // enwakup
    hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(24));    // (IWE) wakeup event on sd card interrupt
    //hal_bitmask_set(MMCHS1, MMCHS_IE, BV(8));       // enable card interrupt (sdio card only!)

    // enable interrupts
    // TODO: check if all important one are set
    //hal_bitmask_set(MMCHS1, MMCHS_IE, BV(20));       // data timeout error
    //hal_bitmask_set(MMCHS1, MMCHS_IE, BV(19));       // cmd index error
    //hal_bitmask_set(MMCHS1, MMCHS_IE, BV(16));       // cto enable error
    //hal_bitmask_set(MMCHS1, MMCHS_IE, BV(1));        // TC (transfer completed) interrupt
    //hal_bitmask_set(MMCHS1, MMCHS_IE, BV(0));        // CC (command completed) interrupt

    // mmc host/bus configuration [22.5.1.5]
    // MMCHS_CON to configure specific data/cmd transfer
    hal_bitmask_clear(MMCHS1, MMCHS_CON, BV(0));	// set OD to 0
    hal_bitmask_clear(MMCHS1, MMCHS_CON, BV(5));    // set DW8 (8 bit mode MMC) to 0 for SD/SDIO cards
    hal_bitmask_clear(MMCHS1, MMCHS_CON, BV(12));	// CEATA mode to standard

    // configure card voltage, pwr mode and data bus width
    //hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(11));		// 0x5: 1.8V (Typical)
    //hal_bitmask_clear(MMCHS1, MMCHS_HCTL, BV(10));		// 0x5: 1.8V (Typical)
    //hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(9));			// 0x5: 1.8V (Typical)
    hal_bitmask_set(MMCHS1, MMCHS_HCTL, (0x6 << 9));		// 0x6 3V
    hal_bitmask_clear(MMCHS1, MMCHS_HCTL, BV(1));			// DTW_1_BIT
    hal_bitmask_clear(MMCHS1, MMCHS_HCTL, BV(8));			// SDBP_OFF

    //hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(8));			// SD bus power on (SDBP)
    //hal_bitmask_set(MMCHS1, MMCHS_HCTL, (0x0 << 8));		// SD bus power on (SDBP)
    //hal_bitmask_clear(MMCHS1, MMCHS_HCTL, BV(1));		// data transfer width to 1 bit (DTW)


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

    hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(8));		// SD bus power on (SDBP)

    // read back the SDBP bit (wait until sd bus power is on)
    while(BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_HCTL) & BV(8)), 8) != 1) {}

    // idle mode
    /*hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(9)); 	// interface and fkt clocks are maintained
    hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(8)); 	// interface and fkt clocks are maintained
    hal_bitmask_clear(MMCHS1, MMCHS_SYSCONFIG, BV(4));	// pwr management
    hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(3));	// pwr management
    hal_bitmask_clear(MMCHS1, MMCHS_SYSCONFIG, BV(0));	// autoidle clock strgy: free running*/

	// precard identification procedure (p. 3162)
	uint32_t identificationStatus = mmcsd_precard_identification();
	if(identificationStatus != 0) {
		printf("Failed to identify card. try again\n");
		return;
	}

	//omap3530_mmchs_card_specific_data
	mmcsd_card_data();

	// configure card for transfer mode
	mmcsd_card_config();

	// check media data
	//mmchsMedia.LastBlock    = (cardInfo.NumBlocks - 1);
	mmchsMedia.BlockSize    = cardInfo.BlockSize;
	//gMMCHSMedia.ReadOnly     = (MmioRead32 (GPIO1_BASE + GPIO_DATAIN) & BIT23) == BIT23;
	mmchsMedia.MediaPresent = true;
	mmchsMedia.MediaId++;
}

void mmcsd_card_data()
{
	uint32_t CardSize;
    uint32_t *BlockSize;
    uint32_t *NumBlocks;
	uint32_t arg = cardInfo.RCA << 16;
	uint32_t status = mmcsd_sendcmd(CMD9, arg, CMD9_INT_EN);
	if(status != 0)
		return;

    ((uint32_t *) &(cardInfo.CSDData))[0] = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_RSP10) & BV(31)), 31);
    ((uint32_t *) &(cardInfo.CSDData))[1] = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_RSP32) & BV(31)), 31);
    ((uint32_t *) &(cardInfo.CSDData))[2] = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_RSP54) & BV(31)), 31);
    ((uint32_t *) &(cardInfo.CSDData))[3] = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_RSP76) & BV(31)), 31);


    // calculate number of blocks
    if (cardInfo.CardType == SD_CARD_2_HIGH) {

    } else {
    	*BlockSize = (0x1UL << cardInfo.CSDData.READ_BL_LEN); // populate blocksize

    	// calculate total number of blocks
    	CardSize = cardInfo.CSDData.C_SIZELow2 | (cardInfo.CSDData.C_SIZEHigh10 << 2);
    	*NumBlocks = (CardSize + 1) * (1 << (cardInfo.CSDData.C_SIZE_MULT + 2));
    }

    cardInfo.BlockSize = (BlockSize > 512) ? 512 : BlockSize; // max 2G
    cardInfo.NumOfBlocks = NumBlocks;

    // calculate max data transfer rate
}

void mmcsd_card_config()
{
	uint32_t arg = cardInfo.RCA << 16;
	uint32_t status = mmcsd_sendcmd(CMD7, arg, CMD7_INT_EN);
	if(status != 0)
		return;

	if(cardInfo.CardType != UNKNOWN_CARD && cardInfo.CardType != MMC_CARD) {
		status = mmcsd_sendcmd(CMD55, arg, CMD55_INT_EN);
		if(status == 0) {
			status = mmcsd_sendcmd(ACMD6, 0x2, ACMD6_INT_EN);
			if(status == 0) {
				// set controller to 4-bit
				hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(1));
			}
		}
	}

	// send cmd16 (block length)
	arg = cardInfo.BlockSize;
	mmcsd_sendcmd(CMD16, arg, CMD16_INT_EN);

	mmcsd_change_clockfrequency(cardInfo.ClockFrequencySelect);
}

uint32_t mmcsd_precard_identification()
{
	// p. 3147
	uint32_t cmdStatus;
	uint8_t cmd8Supported = 0;
	uint32_t response;

	// activate all interrupts
	hal_bitmask_write(MMCHS1, MMCHS_IE, MMCHS_IDENTIFY, 32);

	hal_bitmask_set(MMCHS1, MMCHS_CON, BV(1)); // send init sequence
	hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x00000000, 32);	// send dummy command
	uint32_t dummyCmdSent;
	do {
		// wait until it returns 0x0
		dummyCmdSent = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(0)), 0);
	} while ( dummyCmdSent != 0 );

	// wait for 1ms
	util_ksleep(1);

	hal_bitmask_set(MMCHS1, MMCHS_STAT, BV(0));	// set 1 to clear flag
	hal_bitmask_clear(MMCHS1, MMCHS_CON, BV(1)); // stop sending init sequence

	hal_bitmask_set(MMCHS1, MMCHS_HCTL, (0x6 << 9));	// sdvs_3v
	hal_bitmask_clear(MMCHS1, MMCHS_HCTL, BV(1));		// dtw1 bit
	hal_bitmask_set(MMCHS1, MMCHS_HCTL, BV(8));			// sdbp power on

	hal_bitmask_write(MMCHS1, MMCHS_STAT, 0xFFFFFFFF, 32);	// clear stat register

	// change clock frequency to fit protocol
	mmcsd_change_clockfrequency(MMCHS_CLK_FRQCY400);
	hal_bitmask_set(MMCHS1, MMCHS_CON, BV(0)); // set OD

	// send cmd0
	cmdStatus = mmcsd_sendcmd(CMD0, 0x0, CMD0_INT_EN);
	if(cmdStatus != 0)
		return cmdStatus;

	// send cmd5 (fails if no sdio card found)
	cmdStatus = mmcsd_sendcmd(CMD5, 0x0, CMD5_INT_EN);
	if(cmdStatus == MMCHS_SUCCESS) {
		printf("found sdio card. not yet implemented!\n");
		return cmdStatus;
	}

	// Found SDIO-Card
	hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(25)); // set to 1
	uint32_t softResetDone;
	do {
		// wait until it returns 0x0
		softResetDone = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(25)), 25);
	} while ( softResetDone == 1 );

	// send cmd8
	uint32_t cmd8Status = mmcsd_sendcmd(CMD8, CMD8_ARG, CMD8_INT_EN);
	if(cmd8Status == MMCHS_SUCCESS) {
		//uint32_t value_rsp10 = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_RSP10) & BV(31)), 31);
		uint32_t value_rsp10 = hal_get_register(MMCHS1, MMCHS_RSP10);
		if(value_rsp10 != CMD8_ARG) {
			printf("MMCSD: EFI Device error! Possible no card available?\n");
		}

		// supports sector mode, high capacity flag
		cmd8Supported = 1;
	} else {
		printf("CMD8 fails. Not an sd2.0 card.\n");
	}

	// soft reset
	hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(25)); // set to 1
	softResetDone = 0;
	do {
		// wait until it returns 0x0
		softResetDone = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(25)), 25);
	} while ( softResetDone != 0 );

	// do until card is not busy anymore
	int loopCount = 0;
	do {
		// send cmd55
		uint32_t cmd55Status = mmcsd_sendcmd(CMD55, 0x0, CMD55_INT_EN);
		if(cmd55Status == MMCHS_SUCCESS) {
			response = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_RSP10) & BV(30)), 30);
			printf("succesfully detected sdcard. cmd5 response: %x\n", response);
			cardInfo.CardType = SD_CARD;

			//cmd8Supported = 1;
			// send ACMD41 command
			uint32_t cmdArg = ((uint32_t *) &(cardInfo.OCRData))[0];
			if(cmd8Supported)
				cmdArg |= HCS;

			uint32_t cmd41Status = mmcsd_sendcmd(ACMD41, cmdArg, ACMD41_INT_EN);
			if(cmd41Status != 0)
				return cmd41Status;

			((uint32_t *) &(cardInfo.OCRData))[0] = hal_get_register(MMCHS1, MMCHS_RSP10);
			printf("SD card detected: OCR: %x\n", cardInfo.OCRData);

		} else {
			cardInfo.CardType = MMC_CARD;
			// todo: not supported yet
			printf("detected mmc card. not supported yet\n");
			return cmdStatus;
		}

		// verify that card is not busy
		if (cardInfo.OCRData.Busy == 1) {

		  if (cmd8Supported)
			  cardInfo.CardType = SD_CARD_2;

		  // card is ready, check capacity modes
		  if (cardInfo.OCRData.AccessMode & BV(1)) {
			  cardInfo.CardType = SD_CARD_2_HIGH;
			  printf("High capacity card\n");
		  } else {
			  printf("Standard capacity card\n");
		  }

		  // card is ready to use
		  break;
		}

		loopCount++;
		util_ksleep(1000);

	} while(loopCount < MAX_RETRY);

	// send cmd2 to gather information about how to read sd card
	mmcsd_sendcmd(CMD2, 0x0, CMD2_INT_EN);
	// todo: parse card information

	// send cmd3
	mmcsd_sendcmd(CMD3, 0x0, CMD3_INT_EN);
	// read card relative address (rca)

	// TODO: handle mmc card
	// if mmc card -> more than one card connected? if only 1 or all are identified, go on then

	// send cmd7
	mmcsd_sendcmd(CMD7, 0x0, CMD7_INT_EN);


	return MMCHS_SUCCESS;
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

uint32_t mmcsd_sendcmd(uint32_t cmd, uint32_t arg, uint32_t ie)
{
	// see p.3151 resp. 3155
	// read/write transfer flow with polling and without DMA

	// until cmd lines are not in use anymore
	while( (hal_get_address_value(MMCHS1, MMCHS_PSTATE) & BV(0)) != 0) { }

	// Set Blocksize and number of Blocks
	hal_bitmask_write(MMCHS1, MMCHS_BLK, BV(9), 32); // 512 bytes (block length)
	//hal_bitmask_write(MMCHS1, MMCHS_BLK, BV(16)); // possible TODO: 0 no block transferred, 1 block (block count)

	// reset timeout counter
	hal_bitmask_clear(MMCHS1, MMCHS_SYSCTL, (0xF << 16));
	hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, (0xE << 16));

	// Set Bitfields in MMCHS_CON
	// set MIT, STR, but only for mmc cards (not gonna happen)

	// Clear status register
	hal_bitmask_write(MMCHS1, MMCHS_STAT, 0xFFFFFFFF, 32);

	// write MMCHS_CSRE if response type permits
	// set bit for error code card status
	//hal_bitmask_write(MMCHS1, MMCHS_CSRE, 0x00000000);

	// MMCHS_ARG default
	hal_bitmask_write(MMCHS1, MMCHS_ARG, arg, 32);

	// if interrupts are used: MMCHS_ISE
	// set to default?
	hal_bitmask_write(MMCHS1, MMCHS_IE, ie, 32);

	//hal_bitmask_write(MMCHS1, MMCHS_CON, 0x00000001, 32);

	// Write command
	hal_bitmask_write(MMCHS1, MMCHS_CMD, cmd, 32);


	// MMCHS_CMD and MMCHS_ISE
	/*switch(cmd)
	{
		case 0:
		{
			//hal_bitmask_write(MMCHS1, MMCHS_CON, 0x00000001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x00040001, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_ISE, 0x00040001, 8);
			//hal_bitmask_write(MMCHS1, MMCHS_CON, 0x00000001, 8);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x00000000, 32);
		}
		break;

		case 1:
		{
			//hal_bitmask_write(MMCHS1, MMCHS_CON, 0x00000001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x00050001, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_ISE, 0x00050001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x01020000, 32);
		}
		break;

		case 2:
		{
			//hal_bitmask_write(MMCHS1, MMCHS_CON, 0x00000001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x00070001, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_ISE, 0x00070001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x02090000, 32);
		}
		break;

		case 3:
		{
			//hal_bitmask_write(MMCHS1, MMCHS_ARG, 0x00010000, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_CON, 0x00000001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x100f0001, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_ISE, 0x100f0001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x031a0000, 32);
		}
		break;

		case 41:
		{
			//hal_bitmask_write(MMCHS1, MMCHS_ARG, arg, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_CON, 0x00000001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_IE, ACMD41_INT_EN, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, ACMD41, 32);
		}
		break;

		case 5:
		{
			//hal_bitmask_write(MMCHS1, MMCHS_CON, 0x00000001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x00050001, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_ISE, 0x00050001, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_ARG, 0x00010000, 8);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x05020000, 32);
		}
		break;

		case 6:
		{
			hal_bitmask_write(MMCHS1, MMCHS_IE, ACMD6_INT_EN, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_ISE, 0x00050001, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_ARG, 0x00010000, 8);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, ACMD6, 32);
		}
		break;

		case 55:
		{
			//hal_bitmask_write(MMCHS1, MMCHS_CON, 0x00000001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x100f0001, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_ISE, 0x100f0001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x371a0000, 32);
		}
		break;

		case 7:
		{
			//hal_bitmask_write(MMCHS1, MMCHS_CON, 0x00000000, 32);
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x100f0001, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_ISE, 0x100f0001, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_ARG, 0x00010000, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x071a0000, 32);
		}
		break;

		case 8:
		{
			//hal_bitmask_write(MMCHS1, MMCHS_CON, 0x00000001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x100f0001, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_ISE, 0x100f0001, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x81a0000, 32);
		}
		break;

		case 9:
		{
			//hal_bitmask_write(MMCHS1, MMCHS_CON, 0x00000000, 32);
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x00070001, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_ARG, 0x00010000, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x09090000, 32);
		}
		break;

		case 16:
		{
			//hal_bitmask_write(MMCHS1, MMCHS_CON, 0x00000020, 32);
			hal_bitmask_write(MMCHS1, MMCHS_IE, 0x100f0001, 32);
			//hal_bitmask_write(MMCHS1, MMCHS_ARG, 0x00000200, 32);
			hal_bitmask_write(MMCHS1, MMCHS_CMD, 0x101a0000, 32);
		}
		break;

		default:
		{
			printf("WARNING: tried to send unknown cmd to MMCSD\n");
			return;
		}
	}*/

	int repeatSend = 0;
	while(repeatSend < MAX_RETRY)
	{
		// read MMCHS_STAT register
		uint32_t mmcStatus;
		do {
			mmcStatus = hal_get_address_value(MMCHS1, MMCHS_STAT);
		} while(mmcStatus == 0);

		// Ignore CRC errors on CMD2 and ACMD47, per relevant standards
		// https://code.google.com/p/beagleboard-freebsd/source/browse/trunk/sys/arm/cortexa8/omap3/omap3_mmc.c?r=15#774
		int val = BIT_TRIM_RIGHT( (mmcStatus & BV(17)), 17);
		if ( (cmd == MMCHS_CMD2) && (BIT_TRIM_RIGHT( (mmcStatus & BV(17)), 17) != 0) ){
			mmcStatus &= ~(BV(17));
			// clear ERRI if no other error is set
			if ((mmcStatus & (0xFFFF0000)) == 0)
			{
				mmcStatus &= ~(BV(15));
			}
		}

		// check error interrupt
		if( BIT_TRIM_RIGHT((mmcStatus & BV(15)), 15) != 0 && BIT_TRIM_RIGHT((mmcStatus & BV(16)), 16) != 0) {
			// Soft reset
			hal_bitmask_set(MMCHS1, MMCHS_SYSCTL, BV(25));
			uint32_t softResetAllDone;
			do {
				// wait until it returns 0x0
				softResetAllDone = BIT_TRIM_RIGHT( (hal_get_address_value(MMCHS1, MMCHS_SYSCTL) & BV(25)), 25);
			} while ( softResetAllDone != 0 );

			printf("error sending command: %x\n", mmcStatus);
			return MMCHS_ERROR;
		}

		// check if cmd is completed (CC)
		if((mmcStatus & BV(0)) == 1) {
			hal_bitmask_set(MMCHS1, MMCHS_STAT, BV(0));
			break;
		}

		/*while( ( (hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(0)) != 1) && ( BIT_TRIM_RIGHT((hal_get_address_value(MMCHS1, MMCHS_STAT) & BV(16)), 16) != 1)) { }
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
		}*/

		repeatSend++;
	}

	if(repeatSend == MAX_RETRY) {
		printf("MMCHS send command timeout\n");
		return MMCHS_ERROR;
	}

	return MMCHS_SUCCESS;
}


void mmcsd_softreset()
{
    // set softreset bit to 1
    hal_bitmask_set(MMCHS1, MMCHS_SYSCONFIG, BV(1));
    // wait for reset done
    while( (hal_get_address_value(MMCHS1, MMCHS_SYSSTATUS) & BV(0)) != 1) {}
}
