//
//  api.h
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

#ifndef api_h
#define api_h

/* Includes. */
#include <avr/boot.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/wdt.h>

/* Macros. */
#define bool                _Bool
#define TRUE                ((bool)1)
#define FALSE               ((bool)0)

#endif