//
//  io.c
//  square-wave-gen
//
//  Created by michaelobed on 19/10/2023.
//
//  Copyright 2023 Michael Obed
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//  
//  http://www.apache.org/licenses/LICENSE-2.0
//  
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.


/* Includes. */
#include "api.h"
#include "io.h"

/* Global variables. */
volatile bool gIoFreqChanged = FALSE;
volatile uint32_t gIoFreq = 1000;
volatile uint16_t gIoPeriod = IO_FREQTOPERIOD(1000);
volatile ioState_e gIoState = IOSTATE_IDLE;

void IoInit(void)
{
    /* All I/O should be configured by board type (see the avr.targets.mk Makefile). */
#if defined(SWG_UNO)
    /* UART. */
    DDRD &= ~_BV(PIN_UARTRX);
    DDRD |= _BV(PIN_UARTTX);
    
    /* Square wave output. */
    DDRB |= _BV(PIN_SQ);

#elif defined(SWG_MEGA)
    /* UART. */
    DDRE &= ~_BV(PIN_UARTRX);
    DDRE |= _BV(PIN_UARTTX);

    /* Square wave output. */
    DDRB |= _BV(PIN_SQ);
#endif


    /* Configure the square wave output to use Timer 1A in CTC mode and output on PIN_SQ. */
    OCR1A = gIoPeriod;
    TCNT1 = 0;
    TCCR1A |= _BV(COM1A0);
    TCCR1B |= (_BV(WGM12));        
}

void IoPrintFreq(void)
{
    printf("Freq: %lu (period: %u)\r\n", gIoFreq, OCR1A);
}

void IoPrintOutputStatus(void)
{
    printf("Output: %s\r\n", (TCCR1B & _BV(CS10)) ? "on" : "off");
}

void IoSetFreq(uint32_t freq)
{
    gIoFreq = freq;
    gIoPeriod = IO_FREQTOPERIOD(freq);
    gIoState = IOSTATE_PERIODUPDATED;
}

void IoSetPeriod(uint16_t period)
{
    gIoPeriod = period;
    gIoFreq = IO_PERIODTOFREQ(period);
    gIoState = IOSTATE_PERIODUPDATED;
}

void IoToggleClock(void)
{
    gIoState = IOSTATE_CLOCKUPDATED;
}

void IoUpdate(void)
{
    switch(gIoState)
    {
        case IOSTATE_PERIODUPDATED:
            OCR1A = gIoPeriod;
            IoPrintFreq();
            gIoState = IOSTATE_IDLE;
            break;
        
        case IOSTATE_CLOCKUPDATED:
            TCCR1B ^= _BV(CS10);
            IoPrintOutputStatus();
            gIoState = IOSTATE_IDLE;
            break;
        default:
            break;
    }
}