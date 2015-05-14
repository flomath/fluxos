/*
 * mcbsp.c
 *
 *  Created on: 14.05.2015
 *      Author: florian
 */


/*
 * notes:
 *
 * CS4231A audio codec
 * stereo-mode: configure for a one phase frame containing one 32 bit word
 * 				upper 16 bit (left ch.), lower 16 bit (right ch.)
 *
 * mono-mode: 	configure for a one phase frame containing one 16 bit word
 * 				input: codec only samples left ch.
 * 				output: codec sends sample to both channels
 */
