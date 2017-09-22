---
css:
    - css/main.css

lang: en
title: Metro HD
---

# About Us
![](images/wir.jpg){width=100%}

* Daniela Kilian
    - BSc Applied Computer Science
    - 6th semester
* Stefan Müller
    - BSc Applied Computer Science
    - 6th semester
* Robert Schütz
    - BSc Mathematics
    - 6th semester

# The Task
Our task was it, to design and implement a game for a console resembling the Nintendo Gameboy. 
The console should also have a sound output for music and sound effects.


## Hardware
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

# The Game
![](images/splash.png){width=100%}

We based our game on the Metroid Series, especially on Metroid Fusion.
Thus we made this Logo, which is inspired by the actual Metroid Logo.


So what is there to do in our Game?



![Fight against various Monster.](images/world2.png){width=50%}


There are no less than 12 Monster types in our Game. At least one of them can be encountered in each Room and
just like in the real Metroid the transition between levels happens by
passing through doors, which can be opened by shooting them.

![Explore different Level layouts.](images/world3.png){width=50%}


The Monsters you kill have a possibility to drop so called X-Parasites which can refill your ammunition,
this is a reference to Metroid Fusion (X-Parasite right next to the Protagonist in the Picture).
There is an infinite amount of levels to explore, meaning the game is endless.

![Rest in the Recharge Room.](images/world4.png){width=50%}


Every fourth level there is a so called Recharge Room. There Energy and Ammunition can be refilled by standing in the Recharger
in the middle of the Room.

![Fight bosses.](images/world5.png){width=50%}


After the recharge room - so every fifth level - there will be a boss encounter in which one of the 4 bosses appear.
All the bosses have different moves, so none is like the other.

![Achieve Power-Ups.](images/world6.png){width=50%}


After successfully defeating a Boss Power-Ups are obtained. These will stay until death.



The amount of Monster which can appear will increase by one after the second slain Boss and increases by one for every Boss after that,
which will make the game increase in difficulty after slaying Bosses.





# Level Setup
Every level consists of a number of rooms, whereby the number of rooms is a random number between one and five. 
Furthermore the monster(s), the positions of the platforms and water are chosen randomly for every new room and the floor and ceiling sprite is chosen randomly for every new level. 

To make sure that a level you have already been to looks the same as before, we choose the seed of the random number generator depending on the level and on the room the protagonist is in. There are three variables for the three different heights on which the platforms can appear and one variable for the water:


```{.c}
    srandom(level_seed + level_pos);
    platforms_13 = random();
    platforms_19 = random();
    platforms_24 = random();
    nofloor = random();
``` 

## Depth-first Search
Since the position of the platforms and the water is chosen randomly,
it is possible that the protagonist is not able to simply walk from left to right,
Nevertheless, we have to ensure that he can reach the door.
Therefore, we implemented an algorithm based on depth-first search
that tries to find a path from left to right and also considers jumping on platforms.
Below, we will explain the idea behind this algorithm using an exemplary level.

![We start at the bottom left corner, from where we can only walk to the right.](images/dfs1.png){width=50%}

![From there, we can either jump onto the platform at the left or over the water,](images/dfs2.png){width=50%}

![We first consider the latter alternative. The only option is to continue walking.](images/dfs3.png){width=50%}

![But now, we discover that there is no further path that we can take since the platform is too far away.](images/dfs4.png){width=50%}

![Therefore, we consider the other alternative. We see that we can jump ono another platform.](images/dfs5.png){width=50%}

![Now, we jump over the gap between two platforms.](images/dfs6.png){width=50%}

![This goes on until we have found a path to the door.](images/dfs7.png){width=50%}

If there is no path, we set the `nofloor` variale to a new random value
and try again to find a path to the door.

# Movement
To make things simpler, we decided to move only whole pages in the vertical direction.
Additionally, different monsters etc. are never at the same position,
or if they are then this means that e.g. a projectile has hit a monster and both can be undrawn.
This allows us to only consider the object in motion when redrawing it at a new position.

## Implementation
We use a single C `struct` for monsters, projectiles and the protagonist.
This enables us to reuse the functions responsible for movement for all of them,
which saves us quite some flash space.

```{.c include=../master/character.h startline=11 endline=30}
```

First, there are the functions `moveleft`, `moveright`, `moveup` and `movedown`.
These take care of

- checking if there is an obstace, e.g. a platform, in the respective direction,
- redrawing the character at its new position and
- clearing the pixels that are left over from the old position.

Then, there is also a function `move` which automatically decides in which direction to move.
For example, a monster with `movement==FOLLOW_PROTAGONIST` will automatically move towards the protagonist
whenever this function is called with that very monster as argument.

# Sound
We have different background tunes playing during the levels.
Additionally, there is a characteristic music for each boss you can encounter.
The code supports playing up to three tones at the same time.
Additionally, sound effects are played ontop the background music
when a rocket is shot and when a bomb explodes.

Since synthesizing sound is a time-expensive task,
this is done on a dedicated microcontroller (Atmega168)
to which the speaker is connected through pin B1.

To generate a sound wave,
one can use [pulse-width modulation](https://en.wikipedia.org/wiki/Pulse-width_modulation).
If the frequency of the PWM signal is too high for for the human ear to perceive,
then the displacement of a sound wave can be determined by the pulse width:
![](images/pwm.png)
But instead of a sinusodial wave, we use a [saw wave](https://en.wikipedia.org/wiki/Sawtooth_wave).
To generate this PWM signal, we use Timer1 of the Atmega168 in 8-bit Fast PWM mode.
The `TOP` value of $255$ is reached with a frequency of $62500\,\mathrm{Hz}$.
By setting the register `OCR1A`, we can determine the pulse width,
because B1 is set to high at $0$ and set to low when reaching `OCR1A`.

`OCR1A` is set in the interrupt of Timer2,
which is called with a frequency of $15625\,\mathrm{Hz}$.
To e.g. generate a tone with $440\,\mathrm{Hz}$,
we simply need to reach the maximum pulse width ($255$) with a frequency of $440\,\mathrm{Hz}$.
To do this, the pulse width can simply be incremented by

$$ \frac{255}{15625\,\mathrm{Hz} / 440\,\mathrm{Hz}} \approx 7.18 $$

each time the interrupt is called.
Because of integer overflow,
it will automatically be set to $0$ again after reaching $255$
if we use a `uint8_t`{.c}.
But since we cannot represent non-integer values like $7.18$,
we use `uint16_t`{.c}s and increment the pulse width by

$$ \lfloor 7.18 \cdot 256 \rfloor = \lfloor 1838.08 \rfloor = 1838 $$

which is obviously a lot more accurate.
Additionally, Timer2 is used to keep track of the time.

To make easier to write our own music for the console and use existing tunes,
we wrote a Python script that converts MIDI files
into C arrays that are stored in the `PROGMEM`.
We use the [mido](https://mido.readthedocs.io/en/latest/) library to read the files.
The entries of these arrays contain information about

- the track (as explained above, there are three channels for the music),
- the increment corresponding to the tone that should be played
  (an increment of $0$ means that no tone should be playing on this track)
  and
- the delay until the next entry in the array should be read.

For example, an array could look like this:
```{.c}
const Event boss4[] PROGMEM = {
    { { .track = 0, .increment = 615, .delay = 0 } },
    { { .track = 1, .increment = 307, .delay = 0 } },
    { { .track = 2, .increment = 2463, .delay = 2812 } },
    { { .track = 2, .increment = 1231, .delay = 2812 } },
    { { .track = 0, .increment = 1231, .delay = 0 } },
    ...,
    STOP
};
```

# Download
