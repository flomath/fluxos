/*
 * timer.c
 *
 *  Created on: 26.03.2015
 *      Author: florian
 */

#include "timer.h"

void gpt_timer_init(uint32_t timer)
{
    int ticks = 50;

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
        hal_bitmask_write(timer, GPT_TPIR, positiveIncValue, 32);
        hal_bitmask_write(timer, GPT_TNIR, negativeIncValue, 32);

        // reset TOCR and TOWR
        hal_bitmask_write(timer, GPT_TOCR, 0x00, 32); // TODO FlorianM: check for correct values
        hal_bitmask_write(timer, GPT_TOWR, ticks, 32); // TODO FlorianM: check for correct values

        // Enable optional features
        hal_bitmask_clear(timer, GPT_TCLR, 0xFF); ///< clear all settings
        hal_bitmask_set(timer, GPT_TCLR, BV(11) + BV(1) + BV(6)); ///< enable overflow trigger, autoreload mode and compare

        // For 1-ms tick with a 32.768 Hz clock
        hal_bitmask_write(timer, GPT_TLDR, 0xFFFFFFE0, 32); ///< set to reload value

        hal_bitmask_set(timer, GPT_TMAR, 5000);

        // Enable Interrupt
        hal_bitmask_set(timer, GPT_TIER, BV(0)); ///< OVF_IT_ENA, enable overflow interrupt
        hal_bitmask_set(timer, GPT_TWER, BV(0)); ///< OVF_WUP_ENA, enable overflow wake-up
    }
}

void gpt_disable_interrupts(uint32_t timer)
{
    // disable interrupts
    hal_bitmask_write(timer, GPT_TIER, 0x00, 32);
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
