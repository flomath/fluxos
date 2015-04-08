/*
 * timer.c
 *
 *  Created on: 26.03.2015
 *      Author: florian
 */

#include "timer.h"
#include <stdio.h>

void gpt_timer_init(uint32_t timer)
{
    gpt_timer_stop(timer);
    gpt_disable_interrupts(timer);

    // 1-ms Tick Generation (only suitable for timer 1, 2 and 10 [16.2.4.2.1]
    if(timer == GPT_TIMER1 || timer == GPT_TIMER2 || timer == GPT_TIMER10)
    {
        // formulae for pos/neg increment value
        // Positive increment value = ( (INTEGER[ Fclk * Ttick] + 1) * 1e6) - (Fclk * Ttick * 1e6)
        // Negative increment value = (INTEGER[ Fclk * Ttick] * 1e6) - (Fclk * Ttick * 1e6)
        int positiveIncValue = ( ( ((int)(GPT_FCLK * GPT_TICK)) + 1 ) * GPT_LRGNR ) - (GPT_FCLK * GPT_TICK * GPT_LRGNR);
        int negativeIncValue = ( ((int)(GPT_FCLK * GPT_TICK)) * GPT_LRGNR ) - (GPT_FCLK * GPT_TICK * GPT_LRGNR);
        hal_bitmask_write(timer, GPT_TPIR, positiveIncValue);
        hal_bitmask_write(timer, GPT_TNIR, negativeIncValue);

        // Reset pending interrupts
        //hal_bitmask_set(timer, GPT_TISR, BV(0) + BV(1) + BV(2));
        hal_bitmask_clear(timer, GPT_TISR, BV(0) + BV(1) + BV(2));

        // reset TOCR and TOWR
        //hal_bitmask_write(timer, GPT_TOCR, 0x00); // TODO FlorianM: check for correct values
        //hal_bitmask_write(timer, GPT_TOWR, ticks); // TODO FlorianM: check for correct values

        // For 1-ms tick with a 32.768 Hz clock (GPT_FCLK)
        // Timer Counting Rate [16.2.4.7] with prescaler=1
        // (0xFFFFFFFF - TLDR + 1) * clockPeriod * prescaler
        //hal_bitmask_write(timer, GPT_TLDR, (0xFFFFFFFF - 5000 + 1)); ///< set to reload value
        //hal_bitmask_write(timer, GPT_TLDR, (0xFFFFFFFF - 5000 + 1) * (1/GPT_FCLK));
        hal_bitmask_write(timer, GPT_TLDR, 0xFFFF0000);
        hal_bitmask_write(timer, GPT_TCRR, 0xFFFF0000);
        //see page 2614

        // Enable optional features
        //hal_bitmask_clear(timer, GPT_TCLR, 0xFF); ///< clear all settings
        hal_bitmask_set(timer, GPT_TCLR, BV(0) + BV(10) + BV(1)); ///< enable overflow trigger, autoreload mode and compare

        hal_bitmask_set(timer, GPT_TTGR, BV(1));

        // Enable Interrupt
        hal_bitmask_set(timer, GPT_TIER, BV(1)); ///< OVF_IT_ENA, enable overflow interrupt
        //hal_bitmask_set(timer, GPT_TWER, BV(0)); ///< OVF_WUP_ENA, enable overflow wake-up
    }
    else
    {
    	hal_bitmask_clear(timer, GPT_TISR, BV(0) + BV(1) + BV(2));

    	hal_bitmask_write(timer, GPT_TLDR, (0xFFFFFFFF - 5000 + 1) * (1/GPT_FCLK));
		//hal_bitmask_write(timer, GPT_TCRR, (0xFFFFFFFF - 5000 + 1) * (1/GPT_FCLK));

		hal_bitmask_set(timer, GPT_TTGR, BV(1));

		hal_bitmask_clear(PER_CM, CM_CLKSEL_PER, BV(2)); // set gpt4 to 32k

		// Enable optional features
		//hal_bitmask_clear(timer, GPT_TCLR, 0xFF); ///< clear all settings
		hal_bitmask_set(timer, GPT_TCLR, BV(0) + BV(10) + BV(1)); ///< enable overflow trigger, autoreload mode and compare

		hal_bitmask_set(timer, GPT_TIER, BV(1)); ///< OVF_IT_ENA, enable overflow interrupt
    }
}

void gpt_disable_interrupts(uint32_t timer)
{
    hal_bitmask_write(timer, GPT_TIER, 0x00);
}

void gpt_timer_start(uint32_t timer)
{
    hal_bitmask_set(timer, GPT_TCLR, BV(0));
}

void gpt_timer_reset(uint32_t timer)
{
	hal_bitmask_clear(GPT_TIMER10, GPT_TISR, BV(0) + BV(1) + BV(2));
}

void gpt_timer_stop(uint32_t timer)
{
    hal_bitmask_clear(timer, GPT_TCLR, BV(0));
}
