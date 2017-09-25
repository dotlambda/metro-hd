Drawing
#######
:order: 3

The display of the game console can recognise four grey scales.
Each pixel on the display is defined by two bits
(00 ≙ white pixel, 01 ≙ light grey pixel, 10 ≙ dark grey pixel, 11 ≙ black pixel).

To convert png images into code we wrote a python script
that takes a png image as input and converts it into an C array.
The entries of the array are 8-bit entries that represent the different pages
(page ≙ four vertical pixels) of the image.

Drawing a sprite
================
Instead of using the :code:`page()` function
we wrote the following function,
in which we used the window functionality of the display.
The details are explained in the manual_.

.. code-block:: c

   void drawsprite(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* sprite)
   { 
       enable_window(x, y, width, height);
       for (uint16_t i = 0; i < width * height; ++i)
           sendbyte(pgm_read_byte_near(sprite + i), 1);
       disable_window();
   }

This is faster than using the :code:`page()` function, because fewer commands are send to the display.

In order to draw pixel-by-pixel and not only pagewise in the vertical direction,
we wrote the :code:`drawsprite_px()` function:

.. code-block:: c

   void drawsprite_px(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* sprite)
   {
       uint8_t offset = 2 * (y % 4);
       if (offset == 0)
       {
           drawsprite(x, y / 4, width, height / 4, sprite);
       }
       else
       {
           enable_window(x, y / 4, width, height / 4 + 1);
           uint16_t i = 0;
           for (; i < width; ++i)
               sendbyte(pgm_read_byte_near(sprite + i) << offset, 1);
           for (; i < height / 4 * width; ++i)
               sendbyte(pgm_read_byte_near(sprite + i) << offset |
                        pgm_read_byte_near(sprite + i - width) >> (8 - offset), 1);
           for (; i < (height / 4 + 1) * width; ++i)
               sendbyte(pgm_read_byte_near(sprite + i - width) >> (8 - offset), 1);
           disable_window();
       }
   }

.. _manual: http://www.lcd-module.com/eng/pdf/grafik/dogxl160-7e.pdf
