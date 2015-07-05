/*
 * sleep.c
 */

#include "sleep.h"

/**
 * https://github.com/cmwslw/puppybits/blob/master/lib/ksleep.c
 * sleep approximately v miliseconds
 * NB: this doesn't work if you have caching on ...
 */

void util_ksleep(uint32_t time)
{
	int i;

	// this is approximately 1ms, at least on beagleboard c3 w/ no cache on
	while (time--) {
		for (i=0;i<1000;i++) {
			asm ("\t mov r0, r0");
			asm ("\t mov r0, r0");
			asm ("\t mov r0, r0");
		}
	}
}
