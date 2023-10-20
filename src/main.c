//
//  main.c
//  square-wave-gen
//
//  Created by michaelobed on 18/10/2023.
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

int main(void)
{
    /* Initialise with default freq of 1kHz. */
    IoInit();
    UartInit();
    IoPrintFreq();

    while(TRUE)
    {
        IoUpdate();
    }
}