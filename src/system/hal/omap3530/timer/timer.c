/*
 * timer.c
 *
 *  Created on: 26.03.2015
 *      Author: florian
 */

#include "timer.h"

void gpt_timer_init(uint32_t timer)
{
    gpt_timer_stop(timer);

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

        // reset TOCR and TOWR
        hal_bitmask_clear(timer, GPT_TOCR, 0); // TODO FlorianM: check for correct values
        hal_bitmask_clear(timer, GPT_TOWR, 0); // TODO FlorianM: check for correct values

        // For 1-ms tick with a 32.768 Hz clock
        hal_bitmask_write(timer, GPT_TLDR, 0xFFFFFFE0);
    }
    else
    {

    }
}

void gpt_timer_start(uint32_t timer)
{
    // Start the timer
    hal_bitmask_set(timer, GPT_TCLR, BV(0));
}

void gpt_timer_stop(uint32_t timer)
{
    // Stop the timer
    hal_bitmask_clear(timer, GPT_TCLR, BV(0));
}