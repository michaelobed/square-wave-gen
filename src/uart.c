//
//  uart.c
//  square-wave-gen
//
//  Created by Michael Obed on 19/10/2023.

/* Includes. */
#include "api.h"
#include "uart.h"

/* Local functions. */
int printChar(char c, FILE* pStream);

/* Global variables. */
FILE uartOut = FDEV_SETUP_STREAM(printChar, NULL, _FDEV_SETUP_RW);

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
    stdout = &uartOut;

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
    
    
}