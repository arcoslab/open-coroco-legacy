/****************************************************************************
*
*   Copyright (c) 2006 Dave Hylands     <dhylands@gmail.com>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation.
*
*   Alternatively, this software may be distributed under the terms of BSD
*   license.
*
*   See README and COPYING for more details.
*
****************************************************************************/
/**
*
*   @file    Timer.c
*
*   @brief   Routines for dealing with Timer related functions.
*
*****************************************************************************/
/*
*
*   Configurable options
*
*   Option Name         Default Description
*   ------------------  ------- ----------------------------------------------
*   CFG_TIMER_MS_TICK       0   Add a 32 bit millisecond timer
*   CFG_TIMER_MICRO_TICK    1   Add a counter to make the tick occur once every 10 msec
*   CFG_TIMER_8_BIT_TICK    0   Should gTickCount be 8 bit or 16 bit?
*   CFG_TIMER0_INCLUDE          Name of a header file to #include (useful in conjunction with CFG_TIMER0_MS_TICK)
*   CFG_TIMER0_MS_TICK          Code to execute every tick
*
*****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Config.h"
#include "Timer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#if !defined( __AVR_LIBC_VERSION__ )
#include <avr/signal.h>
#endif

// By #defining CFG_TIMER0_INCLUDE to be the name of a header file containing
// a prototype, and CFG_TIMER0_MS_TICK to be some code to execute, you can
// get some code to execute every millisecond.

#if defined( CFG_TIMER0_INCLUDE )
#include    CFG_TIMER0_INCLUDE
#endif

// ---- Public Variables ----------------------------------------------------

volatile tick_t     gTickCount = 0;

#if CFG_TIMER_MS_TICK
volatile msTick_t   gMsTickCount = 0;
#endif

#if !defined( CFG_TIMER_MICRO_TICK )
#   define  CFG_TIMER_MICRO_TICK    1
#endif

// ---- Private Constants and Types -----------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/***************************************************************************/
/**
*  Timer 0 interrupt handler
*/

SIGNAL(SIG_OVERFLOW0)        /* signal handler for tcnt0 overflow interrupt */
{

    // With CFG_TIMER_MICRO_TICK set to 1, gTickCount will increment once
    // every 10 msec. Otherwise, it will increment every millisecond.

#if CFG_TIMER_MICRO_TICK
    static  uint8_t microTick = 0;

    if ( ++microTick >= 10 )
    {
        microTick = 0;

        gTickCount++;
    }
#else
    gTickCount++;
#endif

#if CFG_TIMER_MS_TICK
    gMsTickCount++;
#endif

    // We want our timer tick to interrupt once a millisecond.
    // If we use 16 MHz/64 then we get 250000 counts/second. So setting
    // things up to overflow after 250 will give us 1000 overflows/second
    //
    // For CFG_CPU_CLOCK = 16 MHz, 16,000,000 / 64 / 1000 = 250
    // For 
    // 256 - 250 = 6

#define OVERFLOW_COUNT  ( CFG_CPU_CLOCK / 1000 / 64 )

    TCNT0 = (uint8_t) -OVERFLOW_COUNT;

#if defined( CFG_TIMER0_MS_TICK )
    CFG_TIMER0_MS_TICK;
#endif

} // Timer 0 Overflow

/***************************************************************************/
/**
*  InitTimer
*/

void timer13Start(void)
{
  TCCR1B = TCCR1B | (1<<CS10); // clk/1 = 16Mhz
  TCCR3B = TCCR1B;
}



void InitTimer( void )
{
//Disabling timers interrupts
    TIMSK = 0;
    ETIMSK = 0;

//Initializing timer 0
#if defined( TCCR0B )
    TCCR0B = TIMER0_CLOCK_SEL_DIV_64; // Divide by 64
#else
    TCCR0  = TIMER0_CLOCK_SEL_DIV_64; // Divide by 64
#endif
    TCNT0 = 0;
    
//Initializing timer 1 
    TCCR1A = ((1 << COM1A1)|(1<<WGM11)); //Phase Correct PWM, non-inverting mode
    TCCR1A |= (1<<COM1B1) | (1<<COM1C1); //Clear  A B and C  output pins on compare
    // compare against 0x1ff = 511
    //for a timer overflow frequency of 15625Hz
    TCCR1B=0;
    TCCR1C = 0;
    TCNT1 = 0;
//Enabling PB pins as outputs for OC1 timer1 comparators PWM output
    DDRB |= (1<<DDB5) | (1<<DDB6) | (1<<DDB7);

//Initializing timer 3
    TCCR3A = ((1 << COM3A1)|(1<<WGM31)); //Phase Correct PWM, non-inverting mode
    TCCR3A |= (1<<COM3B1) | (1<<COM3C1); //Clear  A B and C  output pins on compare
    // compare against 0x1ff = 511
    //for a timer overflow frequency of 15625Hz
    TCCR3B =0;
    TCCR3C = 0;
    TCNT3 = 0;
//Enabling PE pins as outputs for OC3 timer3 comparators PWM output
    DDRE |= (1<<DDE5) | (1<<DDE4) | (1<<DDE3);

//Initializing timer 2 
    TCCR2 = (1 << CS21) | (1<<CS20) | (1<<WGM21); //CTC mode and clk/64 = 250KHz
    TCNT2 = 0;
    OCR2 = 0x1f; //250KHz/32 = 7812Hz
    TIMSK = 0; //Disable interrupt for now
    

    // Enable Timer 0 interrupt

#if defined( TIMSK )
    TIMSK = ( 1 << TOIE0 );
#endif
#if defined( TIMSK0 )
    TIMSK0 = ( 1 << TOIE0 );
#endif

    sei();

} // InitTimer

/***************************************************************************/
/**
*  Wait for Timer 0 to rollover
*/

#if !defined( CFG_TIMER_WAITFORTIMER0ROLLOVER )
#define CFG_TIMER_WAITFORTIMER0ROLLOVER 1
#endif

#if CFG_TIMER_WAITFORTIMER0ROLLOVER
void WaitForTimer0Rollover( void )
{
    tick_t prevCount = gTickCount;

    while ( gTickCount == prevCount )
    {
        ;
    }

} // WaitForTimer0Rollover
#endif
