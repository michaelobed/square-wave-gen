# square-wave-gen

## What is it?
A slightly boring tool to accurately* generate square waves for test purposes.

*: Accuracy unverified. 😜

## How does it work?
It's based on the ~~Atmel~~ Microchip ATMEGA328P, mostly because that's what's in my Arduino Uno.

### Pinout
Pin 0: UART Rx
Pin 1: UART Tx
Pin 9: Square wave output

### UART
The UART can, rather conveniently, also be accessed over the Arduino Uno's USB connection.
Currently, it's only used for controlling either the frequency or the period of the square wave. It supports the following commands:

#### Set frequency:
```
[f][New frequency][Newline]
```

#### Set period:
```
[p][New period][Newline]
```

omitting the '[]', of course, and assuming that `[Newline]` is either "\n" or "\r\n" (or, if you have no idea what those mean, the Enter/Return key on your keyboard).

### Caveats
Given that I'm using a 16-bit timer to generate the square wave, all conversions between period and frequency are integer approximations, so if it's inaccurate, sowwy.

### Ideas
None. I don't plan to do anything more with this. It was just a test tool I built for work because I needed a quick-and-dirty signal generator ~~and don't have the money for the lab bench kind lol~~.

## How build?
If you have Visual Studio Code, rejoice! 🙌🏾 ✨
This supports the AVR Helper extension directly, so just hit "Build" on that and you should have a nice, shiny .hex file in the `bin/` folder that you can flash with avrdude or similar.

Alternatively, just invoke avr-gcc the way you would in your favourite toolchain.