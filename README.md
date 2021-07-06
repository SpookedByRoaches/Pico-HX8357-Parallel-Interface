# HX8357 8-bit Parallel Pi Pico Interface

- There is not much support for customizable parallel communication with the 
TFT driver chip: HX8357. Hopefully this helps others on their projects as
much as it did for me

- This is based on the Adafruit-GFX library and uses the same algorithms for
drawing with some modifications to optimize for speed.

- All of the geometric primitives are there and can be tested using the available
test functions

- Keep in mind there are many features that the GFX library has that this one 
does not




## Features not included in this library are
    1- Custom fonts
    2- PROGMEM bitmap drawing
    3- Grayscale bitmap drawing
    4- XBitMap drawing
    5- AdafruitGFXButton
    6- Reading GRAM data
    7- Display inversion