/*
 * audio.c
 *
 *  Created on: 02.07.2015
 *      Author: Philip
 */

#include "audio.h"
#include "../../system/hal/omap3530/mcbsp/mcbsp.h"

static char allmyducks[] = "CDEF2G2GxAxAxAxA4GxAxAxAxA4GFFFF2E2EGGGG4C";

static int audio_frequencies[] = {
		220.00,		///< A3
		246.94,		///< B3
		261.63,		///< C4
		293.66,		///< D4
		329.63,		///< E4
		349.23,		///< F4
		392.00,		///< G4
		440.000,	///< A4
		493.88,		///< B4
		523.25,		///< C5
		587.33,		///< D5
		659.25,		///< E5
		698.46,		///< F5
		783.99		///< G5
};

static int audio_get_frequency(char note, int level) {
	return AUDIO_SAMPLE_RATE / audio_frequencies[note - 'A' + 7 * level];
}

void audio_play_duck() {
	audio_play_notes(allmyducks);
}

void audio_play_notes(char* notes) {

	int length = 128;
	int level = 0;
	while (*notes != '\0') {

		if (*notes >= '0' && *notes <= '9') {
			length = (*notes - '0') * 128;
		} else if (*notes == 'x') {
			++level;
		} else {
			audio_play_wave(audio_get_frequency(*notes, level), 3, length);
			length = 128;
			level = 0;
		}

		notes++;
	}
}

static env_t adsr[] = {
	{ MAX, ((unsigned int) (AUDIO_SAMPLE_RATE * 0.05)) },
	{ SUS, ((unsigned int) (AUDIO_SAMPLE_RATE * 0.05)) },
	{ SUS, ((unsigned int) (AUDIO_SAMPLE_RATE * 0.2)) },
	{ 0, ((unsigned int) (AUDIO_SAMPLE_RATE * 0.02)) }
};

// Original author unknown, found this method a lot of times while googling
// All thanks to you!
void audio_play_wave(int frequency, int channels, int length) {
	int amplitude_start = 0;
	int st = 0;

	int i;
	for (i = 0; i < 4; i++) {
		int at = adsr[i].target;
		int duration = adsr[i].time;

		// Scale 'sustain' for note length
		if (i == 2) {
			duration = duration * length >> 8;
		}

		int j;
		for (j = 0; j < duration; j++) {
			int amplitude = (at - amplitude_start) * j / duration + amplitude_start;
			int v = (st * 2 - frequency) * amplitude / frequency;

			// Send signal on both channels
			while ((hal_get_address_value(MCBSP2, MCBSPLP_SPCR2_REG) & MCBSP_XRDY) == 0);
			hal_bitmask_write(MCBSP2, MCBSPLP_DXR_REG, channels & 1 ? v : 0, 32);

			while ((hal_get_address_value(MCBSP2, MCBSPLP_SPCR2_REG) & MCBSP_XRDY) == 0);
			hal_bitmask_write(MCBSP2, MCBSPLP_DXR_REG, channels & 2 ? v : 0, 32);

			st += 1;
			if (st >= frequency) {
				st = 0;
			}
		}
		amplitude_start = at;
	}
}
