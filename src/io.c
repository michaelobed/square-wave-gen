//
//  io.c
//  square-wave-gen
//
//  Created by Michael Obed on 19/10/2023.

/* Includes. */
#include "api.h"
#include "io.h"

/* Global variables. */
volatile uint16_t ioSqFreq = 1000;

void IoInit(void)
{
    /* UART. */
    DDRD &= ~_BV(PIN_UARTRX);
    DDRD |= _BV(PIN_UARTTX);

    /* Square wave output. */
    DDRB |= _BV(PIN_SQ);

    /* Configure the square wave output to use Timer 1A in CTC mode and output on PIN_SQ. */
    OCR1A = IO_FREQTOPERIOD(ioSqFreq);
    TCNT1 = 0;
    // TCCR1A |= _BV(COM1A0);
    // TCCR1B |= (_BV(WGM12) | _BV(CS10));
    TCCR1A = 0x40;
    TCCR1B = 0x09;
}

void IoPrintFreq(void)
{
    printf("Freq: %u (period: %u)\n", ioSqFreq, OCR1A);
}