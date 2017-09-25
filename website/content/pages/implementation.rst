Implementation
##############
:order: 3

Drawing
=======
The display of the game console can recognise four grey scales.
Each pixel on the display is defined by two bits
(00 ≙ white pixel, 01 ≙ light grey pixel, 10 ≙ dark grey pixel, 11 ≙ black pixel).

To convert png images into code we wrote a python script
that takes a png image as input and converts it into an C array.
The entries of the array are 8-bit entries that represent the different pages
(page ≙ four vertical pixels) of the image.

Instead of using the :code:`page()` function
we wrote the following function,
in which we use the window functionality of the display.
The details are explained in the `data sheet`_.

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

.. _data sheet: http://www.lcd-module.com/eng/pdf/grafik/dogxl160-7e.pdf

Level Setup
===========
Every level consists of a number of rooms, whereby the number of rooms is a random number between one and five.
Furthermore the monster(s), the positions of the platforms and water are chosen randomly for every new room and the floor and ceiling sprite is chosen randomly for every new level.

To make sure that a level you have already been to looks the same as before, we choose the seed of the random number generator depending on the level and on the room the protagonist is in.


.. code-block:: c

    srandom(level_seed + level_pos);
    platforms_13 = random();
    platforms_19 = random();
    platforms_24 = random();
    nofloor = random();

Depth-first Search
------------------
Since the position of the platforms and the water is chosen randomly,
it is possible that the protagonist is not able to simply walk from left to right,
Nevertheless, we have to ensure that he can reach the door.
Therefore, we implemented an algorithm based on `depth-first search`_
that tries to find a path from left to right and also considers jumping on platforms.
Below, we will explain the idea behind this algorithm using an exemplary level.

.. figure:: {filename}/images/dfs1.png
   :alt: We start at the bottom left corner, from where we can only walk to the right.
   :width: 50%

   We start at the bottom left corner, from where we can only walk to the right.

.. figure:: {filename}/images/dfs2.png
   :alt: From there, we can either jump onto the platform at the left or over the water.
   :width: 50%

   From there, we can either jump onto the platform at the left or over the water.

.. figure:: {filename}/images/dfs3.png
   :alt: We first consider the latter alternative. The only option is to continue walking.
   :width: 50%

   We first consider the latter alternative. The only option is to continue walking.

.. figure:: {filename}/images/dfs4.png
   :alt: But now, we discover that there is no further path that we can take since the platform is too far away.
   :width: 50%

   But now, we discover that there is no further path that we can take since the platform is too far away.

.. figure:: {filename}/images/dfs5.png
   :alt: Therefore, we consider the other alternative. We see that we can jump ono another platform.
   :width: 50%

   Therefore, we consider the other alternative. We see that we can jump ono another platform.

.. figure:: {filename}/images/dfs6.png
   :alt: Now, we jump over the gap between two platforms.
   :width: 50%

   Now, we jump over the gap between two platforms.

.. figure:: {filename}/images/dfs7.png
   :alt: This goes on until we have found a path to the door.
   :width: 50%

   This goes on until we have found a path to the door.

If there is no path, we set the :code:`nofloor` variable to a new random value
and try again to find a path to the door.

.. _depth-first search: https://en.wikipedia.org/wiki/Depth-first_search
