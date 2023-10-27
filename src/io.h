//
//  io.h
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
    IOSTATE_PERIODUPDATED,
    IOSTATE_CLOCKUPDATED
} ioState_e;

/* Global functions. */
void IoInit(void);
void IoPrintFreq(void);
void IoPrintOutputStatus(void);
void IoSetFreq(uint32_t freq);
void IoSetPeriod(uint16_t period);
void IoToggleClock(void);
void IoUpdate(void);

#endif