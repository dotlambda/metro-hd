Title: The Task
Order: 1

Our task was to design and implement a game for a console resembling the Nintendo Gameboy.
The console should also have a sound output for music and sound effects.

# Hardware
![The hardware]({filename}/images/console.jpg){width=50%}

The console features two microcontrollers:

* [Atmega328](https://en.wikipedia.org/wiki/ATmega328) with
    - 32 kB flash
    - 2 kB SRAM
    - 1 kB EEPROM
* Atmega168 with
    - 16 kB flash
    - 1 kB SRAM
    - 512 B EEPROM

where the former is responsible for the game logic and controlling the display
and the latter is used solely as a sound controller.

Furthermore, it features an
[EA DOGXL160L-7](http://www.lcd-module.com/eng/pdf/grafik/dogxl160-7e.pdf).
This LC display has a resolution of 160×104 pixels,
but four vertically adjacent pixels are grouped together in so-called pages
and can only be written as a whole.
Therefore, when writing to the display,
the x coordinate must be below 160 and the y coordinate below 25.

Last but not least, the 8 buttons known from the Gameboy are available:

* left, right, up, down
* A, B
* pause, select
