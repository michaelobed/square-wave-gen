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

/* Local functions. */
void loadSettings(void);
void saveSettings(void);

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
    TCCR1A |= _BV(COM1A0);
    TCCR1B |= (_BV(WGM12));

    /* Finally, load our last period settings. */
    loadSettings();
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
            saveSettings();
            IoPrintFreq();
            gIoState = IOSTATE_IDLE;
            break;
        
        case IOSTATE_CLOCKUPDATED:
            TCCR1B ^= _BV(CS10);
            saveSettings();
            IoPrintOutputStatus();
            gIoState = IOSTATE_IDLE;
            break;

        default:
            break;
    }
}

void loadSettings(void)
{
    uint16_t period = 0xffff;

    /* Read the EEPROM-is-in-use byte. If zero, skip loading. */
    if(eeprom_read_byte((const uint8_t*)0x0000) == IO_EEPROMINUSEBYTE)
    {
        /* Read the 2-byte period from EEPROM. */
        period = eeprom_read_word((const uint16_t*)0x0001);
        IoSetPeriod(period);
    }
}

void saveSettings(void)
{
    /* Write 2-byte period to 0x0000 of EEPROM, then write a byte saying there is now data in the EEPROM. */
    eeprom_write_word((const uint16_t*)0x0001, gIoPeriod);
    eeprom_write_byte((const uint8_t*)0x0000, IO_EEPROMINUSEBYTE);
}