/*
 * audio.h
 *
 *  Created on: 02.07.2015
 *      Author: Philip
 */

#ifndef SRC_APPLICATIONS_AUDIO_AUDIO_H_
#define SRC_APPLICATIONS_AUDIO_AUDIO_H_

#include "../../system/hal/omap3530/tps65950/tps65950.h"

#define MAX 32767
#define SUS ((unsigned int )(MAX * 0.8))

#define SRATE 48000	///< Sample Rate 48 KHz

/**
 * Capsule structure for audio
 */
typedef struct {
	unsigned int target;
	unsigned int time;
} env_t;

/**
 * Play one note
 * @param wave_length
 * @param channels Left/Right (bits [1:0])
 * @param length Sustain Length
 */
void play_note(int wave_length, int channels, int length);

void play_sample(void);


#endif /* SRC_APPLICATIONS_AUDIO_AUDIO_H_ */
