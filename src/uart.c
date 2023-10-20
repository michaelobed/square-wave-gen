//
//  uart.c
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
#include "uart.h"

/* Local functions. */
int printChar(char c, FILE* pStream);

/* Global variables. */
char gUartBuf[UART_BUFSIZE];
uint8_t gUartBufIdx = 0;
FILE gUartOut = FDEV_SETUP_STREAM(printChar, NULL, _FDEV_SETUP_RW);

void UartInit(void)
{
    /* Disable interrupts. */
    cli();

    /* Configure for 8N1, 115.2kbaud. */
    UBRR0 = 16;
    UCSR0A |= _BV(U2X0);
    UCSR0B |= (_BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0));

    /* Enable interrupts. */
    sei();

    /* Redirect stdout to UART. */
    stdout = &gUartOut;

    printf("UART initialised\n");
}

int printChar(char c, FILE* pStream)
{
    UDR0 = c;
    while(!(UCSR0A & _BV(TXC0)));
    UCSR0A |= _BV(TXC0);
    return 0;
}

ISR(USART_RX_vect)
{
    uint16_t n = 0;

    /* Read the character. If it's a newline, parse what's in the buffer. We should always echo the character. */
    gUartBuf[gUartBufIdx] = UDR0;
    UDR0 = gUartBuf[gUartBufIdx];
    if(gUartBuf[gUartBufIdx] == '\n')
    {
        switch(gUartBuf[0])
        {
            /* Change the frequency. */
            case 'f':
                n = atoi(gUartBuf + 1);
                IoSetFreq(n);
                break;

            /* Change the period. */
            case 'p':
                n = atoi(gUartBuf + 1);
                IoSetPeriod(n);
                break;

            default:
                break;
        }

        /* Reset the byte counter for the next command. */
        gUartBufIdx = 0;
    }

    /* Increment the counter, as we're not done reading in bytes yet.
     * As a protective measure, don't bother with this if the buffer is full. */
    else if(gUartBufIdx < UART_BUFSIZE)
        gUartBufIdx++;
}