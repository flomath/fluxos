/*
 * tps65950.c
 *
 *  Created on: 02.07.2015
 *      Author: Philip
 */

#include "tps65950.h"
#include "../i2c/i2c.h"
#include "../../common/hal.h"

void tps_i2c_set(uint32_t address, uint32_t value) {
	i2c_write(I2C1, SCD_AUDIO_VOICE, address, value);
}

void tps_audio_volume_down()
{
	tps_i2c_set(TPS_SOFTVOL_CTL, SOFTVOL_CTL_7);
}
void tps_audio_volume_up()
{
	tps_i2c_set(TPS_SOFTVOL_CTL, SOFTVOL_CTL_0);
}

void tps_audio_setup(void)
{
	// see page 731 in TPS65950 OMAP Power Management and System Companion Device Silicon Revision 1.2 (version G)

	// disable before setup
	tps_i2c_set(TPS_CODEC_MODE, 0);

	// select 8kHz Sampling frequency
	// and stereo output (2 rx/tx)
	tps_i2c_set(TPS_CODEC_MODE, (APPL_RATE_8 | OPT_MODE) );

	// enable audio TX right 1 and left 1
	tps_i2c_set(TPS_OPTION, (ATXR1_EN | ATXL1_EN) );
	// enable audio RX right 2 and left 2
	tps_i2c_set(TPS_OPTION, (ARXR2_EN | ARXL2_EN) );

	// select input RXL1 and RXR1
	tps_i2c_set(TPS_RX_PATH_SEL,  (RXL1_SEL | RXR1_SEL) );

	// audio PLL power control enable with 26MHz input frequency
	tps_i2c_set(TPS_APLL_CTL, (APLL_EN | APLL_INFREQ_26MHZ) );

	// enable the right and left voice DAC
	tps_i2c_set(TPS_AVDAC_CTL, (ADACR2_EN | ADACL2_EN) );

	// enable application mode output and set gain to 0dB
	tps_i2c_set(TPS_ARXL2_APGA_CTL, (ARXL2_PDZ | ARXL2_DA_EN | ARXL2_GAIN_0DB) );
	tps_i2c_set(TPS_ARXR2_APGA_CTL, (ARXL2_PDZ | ARXL2_DA_EN | ARXL2_GAIN_0DB) );

	// enable headset amplifier output (left/right)
	tps_i2c_set(TPS_HS_SEL, (HSOL_AL2_EN | HSOR_AR2_EN) );

	// define gain control (set 0dB)
	tps_i2c_set(TPS_ARXL2PGA, TPS_ARXL2PGA_GAIN0DB);
	tps_i2c_set(TPS_ARXR2PGA, TPS_ARXL2PGA_GAIN0DB);

	// headset amp gain to 0dB
	tps_i2c_set(TPS_HS_GAIN_SET, 0x0a);

	// enable audio serial interface with 16bit audio
	tps_i2c_set(TPS_AUDIO_IF, (AIF_FORMAT_CODEC | DATA_WIDTH_16 | AIF_EN) );

	// set gain for transmit audio to 0dB
	tps_i2c_set(TPS_ATXL1PGA, 0x0);
	tps_i2c_set(TPS_ATXR1PGA, 0x0);

	// enable ADCL and ADCR
	tps_i2c_set(TPS_AVADC_CTL, (ADCL_EN | ADCR_EN) );

	// enable input mode: ADCL routed to TXL1, ADCR routed to TXR1
	tps_i2c_set(TPS_ADCMICSEL, 0);
	// enable softvolume
	//tps_i2c_set(TPS_SOFTVOL_CTL, (0x1 << 0));
	//tps_i2c_set(TPS_SOFTVOL_CTL, (0x0 << 5));	//< 1*0.8/FS
	// enable 48kHz with codec
	tps_i2c_set(TPS_CODEC_MODE, (APPL_RATE_48 | CODEECPDZ_ON | OPT_MODE) );
}
