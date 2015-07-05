/*
 * tps65950.c
 *
 *  Created on: 02.07.2015
 *      Author: Philip
 *
 * All thanks to https://github.com/Wolfy42/itmOS
 */

#include "tps65950.h"
#include "../i2c/i2c.h"
#include "../../common/hal.h"

static void tps_register_write(uint32_t address, uint32_t value) {
	i2c_write(I2C1, SCD_AUDIO_VOICE, address, value);
}

void tps_init(void) {

	// turn it off to configure
	tps_register_write(AV_CODEC_MODE, 0);

	// page 725
	// Audio mode: Select sampling frequence (Fs) -> set to 8 kHz
	// Audio and voice option selection: Option 1: 2 RX and TX stereo audio paths
	tps_register_write(AV_CODEC_MODE, APPL_RATE_8 | OPT_MODE);

	// Audio/voice digital filter power control
	tps_register_write(AV_OPTION, ARXR2_EN | ARXL2_EN | ATXR1_EN | ATXL1_EN);

	// path selection
	tps_register_write(AV_RX_PATH_SEL,  B_RXL1_SEL | B_RXR1_SEL);

	// set system clock speed and enable the local clock
	tps_register_write(AV_APLL_CTL, APLL_EN | APLL_INFREQ_26);

	// left/right dacs enlabled
	tps_register_write(AV_AVDAC_CTL, ADACL2_EN | ADACR2_EN);

	// turn on left and right output
	tps_register_write(AV_ARXL2_APGA_CTL, ARX1_PDZ | ARX1_DA_EN | (0x06 << B_ARX1_GAIN_SET));
	tps_register_write(AV_ARXR2_APGA_CTL, ARX1_PDZ | ARX1_DA_EN | (0x06 << B_ARX1_GAIN_SET));

	// headset output
	tps_register_write(AV_HS_SEL, HSOL_AL2_EN | HSOR_AR2_EN);

	// set digital gain to 0db
	tps_register_write(AV_ARXL2PGA, 0x30 | (3<<6));              /* 0dB */
	tps_register_write(AV_ARXR2PGA, 0x30 | (3<<6));              /* 0dB */

	// set headset left/right gain (balance) to 0db each
	tps_register_write(AV_HS_GAIN_SET, 0x0a);

	// audio interface.  master mode, i2s format, 16 bit data
	tps_register_write(AV_AUDIO_IF, AIF_FORMAT_CODEC | DATA_WIDTH_16 | AIF_EN);

	// volume/effects
	// set base boost effect
	tps_register_write(AV_BOOST_CTL, 0);
	// anti-pop paramters
	tps_register_write(AV_HS_POPN_SET, VMID_EN | RAMP_DELAY_161ms | RAMP_EN);

	// tramsit settings (audio in)
	// left/right gain
	tps_register_write(AV_ATXL1PGA, 0x0);
	tps_register_write(AV_ATXR1PGA, 0x0);

	// left/right ADC, and choose as TXx1
	tps_register_write(AV_AVADC_CTL, ADCL_EN | ADCR_EN);
	tps_register_write(AV_ADCMICSEL, 0);

	// line in on beagle comes from AUX, also enable anti-pop and start
	tps_register_write(AV_ANAMICL, 0x34 | AUXL_EN | OFFSET_CNCL_SEL_RXALL | CNCL_OFFSET_START);
	tps_register_write(AV_ANAMICR, 0x14 | AUXR_EN);
	tps_register_write(AV_ANAMIC_GAIN, (0<<3)|0);

	// now power it up, with sample rate and option 1 (2x stereo audio paths in and out)
	tps_register_write(AV_CODEC_MODE, APPL_RATE_8 | 0 | OPT_MODE);
	tps_register_write(AV_CODEC_MODE, APPL_RATE_48 | CODECPDZ | OPT_MODE);
}

void tps_led_init(void) {
	// Disable the H-Bridge
	i2c_write(I2C1, SCD_LED, VIBRA_CTL, 0);
	// Enable the LED and PWM
	i2c_write(I2C1, SCD_LED, LED_EN, BV(0) | BV(5));
}
