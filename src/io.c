//
//  io.c
//  square-wave-gen
//
//  Created by Michael Obed on 19/10/2023.

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
    /* UART. */
    DDRD &= ~_BV(PIN_UARTRX);
    DDRD |= _BV(PIN_UARTTX);

    /* Square wave output. */
    DDRB |= _BV(PIN_SQ);

    /* Configure the square wave output to use Timer 1A in CTC mode and output on PIN_SQ. */
    OCR1A = gIoPeriod;
    TCNT1 = 0;
    TCCR1A = 0x40;
    TCCR1B = 0x09;
}

void IoPrintFreq(void)
{
    printf("Freq: %lu (period: %u)\n", gIoFreq, OCR1A);
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

void IoUpdate(void)
{
    switch(gIoState)
    {
        case IOSTATE_PERIODUPDATED:
            OCR1A = gIoPeriod;
            IoPrintFreq();
            gIoState = IOSTATE_IDLE;
            break;

        default:
            break;
    }
}