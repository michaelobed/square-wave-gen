//
//  uart.c
//  square-wave-gen
//
//  Created by Michael Obed on 19/10/2023.

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