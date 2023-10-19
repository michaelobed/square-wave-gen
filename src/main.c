//
//  main.c
//  square-wave-gen
//
//  Created by Michael Obed on 18/10/2023.

/* Includes. */
#include "api.h"
#include "io.h"
#include "uart.h"

int main(void)
{
    /* Initialise with default freq of 1kHz. */
    IoInit();
    UartInit();
    IoPrintFreq();

    while(TRUE)
    {

    }
}