//
//  io.h
//  square-wave-gen
//
//  Created by Michael Obed on 19/10/2023.

#ifndef io_h
#define io_h

/* Includes. */
#include "api.h"

/* Macros. */
#define PIN_UARTRX              PD0
#define PIN_UARTTX              PD1
#define PIN_SQ                  PB1

#define IO_FREQTOPERIOD(x)      (F_CPU / (2 * (1 + x)))
#define IO_PERIODTOFREQ(x)      ((F_CPU / (2 * x)) - 1)

/* I/O state type. */
typedef enum
{
    IOSTATE_IDLE = 0,
    IOSTATE_PERIODUPDATED
} ioState_e;

/* Global functions. */
void IoInit(void);
void IoPrintFreq(void);
void IoSetFreq(uint32_t freq);
void IoSetPeriod(uint16_t period);
void IoUpdate(void);

#endif