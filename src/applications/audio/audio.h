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

#define AUDIO_SAMPLE_RATE 	48000	///< Sample Rate 48 KHz
#define AUDIO_LENGTH		128		///< Note length for 1/8 note

/**
 * Capsule structure for audio
 */
typedef struct {
	unsigned int target;
	unsigned int time;
} env_t;

static int audio_get_frequency(char note, int level);

/**
 * Play one note
 * @param wave_length
 * @param channels Left/Right (bits [1:0])
 * @param length Sustain Length
 */
void audio_play_wave(int wave_length, int channels, int length);

/**
 * Play a string of characters
 *
 * All notes are 1/8 notes. However if you add a cipher before a note the length will be multiplied with it.
 * For 1/4 notes write '2A', for 1/2 notes write 4A and for 1/1 notes write '8A'.
 * If you need a higher octave you can write 'x' before a note. For example write xA for A4, and just A for A3.
 * Supported notes are A3 to G5.
 * @param notes The notes to play; Supports only uppercase characters, numbers and 'x'; Must end with '\0'
 */
void audio_play_notes(char* notes);

/**
 * Plays the "Alle meine Entchen" Song
 */
void audio_play_duck(void);

#endif /* SRC_APPLICATIONS_AUDIO_AUDIO_H_ */
