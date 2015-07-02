/*
 * audio.c
 *
 *  Created on: 02.07.2015
 *      Author: Philip
 */

#include "audio.h"
#include "../../system/hal/omap3530/mcbsp/mcbsp.h"

env_t adsr[] = {
	{ MAX, ((unsigned int)(SRATE * 0.05)) },
	{ SUS, ((unsigned int)(SRATE * 0.05)) },
	{ SUS, ((unsigned int)(SRATE * 0.2)) },
	{ 0, ((unsigned int)(SRATE * 0.02)) }
};

void play_note(int wave_length, int channels, int length) {
	int ampstart = 0;
	int st = 0;

	int i;
	for (i = 0; i < 4; i++) {
		int at = adsr[i].target;
		int dur = adsr[i].time;
		int j;

		// Scale 'sustain' for note length
		if (i == 2) {
			dur = dur * length >> 8;
		}

		for (j = 0; j < dur; j++) {
			int amp = (at-ampstart) * j / dur + ampstart;
			int v = (st*2 - wave_length) * amp / wave_length;

			// send out 2 samples, left/right
			while ((hal_get_address_value(MCBSP2, MCBSPLP_SPCR2_REG) & MCBSP_XRDY) == 0);
			hal_bitmask_write(MCBSP2, MCBSPLP_DXR_REG, channels & 1 ? v : 0, 32);

			while ((hal_get_address_value(MCBSP2, MCBSPLP_SPCR2_REG) & MCBSP_XRDY) == 0);
			hal_bitmask_write(MCBSP2, MCBSPLP_DXR_REG, channels & 2 ? v : 0, 32);

			st += 1;
			if (st >= wave_length) {
				st = 0;
			}
		}
		ampstart = at;
	}
}

char smoke[] = "GH2CGHI4CGH2CH4G";

// wavelengths of various basic notes
int ftable[] = {
        (int)(SRATE / 220.000),  /* a3 */
        (int)(SRATE / 246.942),
        (int)(SRATE / 261.626),  /* c3 */
        (int)(SRATE / 293.665),
        (int)(SRATE / 329.628),
        (int)(SRATE / 349.228),
        (int)(SRATE / 195.998),  /* g2 */
        (int)(SRATE / 233.080),  /* h3 */
        (int)(SRATE / 277.180) 	 /* cis */
};

void play_sample(void) {
	int i = 0;
	while (i++ < 3) {
		char *np = smoke;
		char n;
		int len = 256;

		while ( (n = *np++) ) {
			if (n >= '0' && n <= '9') {
					len = (n - '0') * 128;
			} else {
					play_note(ftable[n - 'A'], 3, len);
					len = 256;
			}
		}
	}
}
