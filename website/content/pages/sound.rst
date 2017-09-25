Sound
##############

:order: 8

We have different background tunes playing during the levels.
Additionally, there is a characteristic music for each boss you can encounter.
The code supports playing up to three tones at the same time.
Additionally, sound effects are played ontop the background music
when a rocket is shot and when a bomb explodes.

Since synthesizing sound is a time-expensive task,
this is done on a dedicated microcontroller (Atmega168)
to which the speaker is connected through pin B1.

To generate a sound wave, one can use `pulse-width modulation`_.
If the frequency of the PWM signal is too high for for the human ear to perceive,
then the displacement of a sound wave can be determined by the pulse width:

.. figure:: {filename}/images/pwm.png
   :alt: A PWM signal generating a sine wave

   A PWM signal generating a sine wave

But instead of a sinusodial wave, we use `saw waves`_.
To generate this PWM signal, we use Timer1 of the Atmega168 in 8-bit Fast PWM mode.
The :code:`TOP` value of 255 is reached with a frequency of 62500 Hz.
By setting the register :code:`OCR1A`, we can determine the pulse width,
because B1 is set to high at 0 and set to low when reaching :code:`OCR1A`.

:code:`OCR1A` is set in the interrupt of Timer2,
which is called with a frequency of 15625 Hz.
To e.g. generate a tone with 440 Hz,
we simply need to reach the maximum pulse width (255) with a frequency of 440 Hz.
To do this, the pulse width can simply be incremented by

.. math::
   \frac{255}{15625\,\mathrm{Hz} / 440\,\mathrm{Hz}} \approx 7.18

each time the interrupt is called.
Because of integer overflow,
it will automatically be set to 0 again after reaching 255
if we use a :code:`uint8_t`.
But since we cannot represent non-integer values like 7.18,
we use :code:`uint16_t` and increment the pulse width by

.. math::
   \lfloor 7.18 \cdot 256 \rfloor = \lfloor 1838.08 \rfloor = 1838

which is obviously a lot more accurate.
Additionally, Timer2 is used to keep track of the time.

To make easier to write our own music for the console and use existing tunes,
we wrote a Python script that converts MIDI files
into C arrays that are stored in the :code:`PROGMEM`.
We use the mido_ library to read the files.
The entries of these arrays contain information about

- the track (as explained above, there are three channels for the music),
- the increment corresponding to the tone that should be played
  (an increment of 0 means that no tone should be playing on this track)
  and
- the delay until the next entry in the array should be read.

For example, an array could look like this:

.. code-block:: c

   const Event boss4[] PROGMEM = {
       { { .track = 0, .increment = 615, .delay = 0 } },
       { { .track = 1, .increment = 307, .delay = 0 } },
       { { .track = 2, .increment = 2463, .delay = 2812 } },
       { { .track = 2, .increment = 1231, .delay = 2812 } },
       { { .track = 0, .increment = 1231, .delay = 0 } },
       ...,
       STOP
   };

Here, you can hear some of the music as it is played by the console:

.. raw:: html

   <audio src="downloads/ingame1.ogg" controls></audio>
   <audio src="downloads/ingame2.ogg" controls></audio>
   <audio src="downloads/gameover.ogg" controls></audio>

.. _pulse-width modulation: https://en.wikipedia.org/wiki/Pulse-width_modulation
.. _saw waves: https://en.wikipedia.org/wiki/Sawtooth_wave
.. _mido: https://mido.readthedocs.io/en/latest/
