#!/usr/bin/env python2

cfilename = "fx.c"
hfilename = "fx.h"

with open(hfilename, "w") as hfile:
    with open(cfilename, "w") as cfile:
        cfile.write("#include \"music.h\"\n\n")
        hfile.write("#ifndef FX_H\n#define FX_H\n\n#include <inttypes.h>\n#include <avr/pgmspace.h>\n\n")
        hfile.write("#define STOP 0xFFFF\n\n")

        arrayname = "shoot"
        delay = 10
        array = "const uint16_t " + arrayname + "[] PROGMEM = {\n"
        rng = range(4000, 1000, -20)
        for increment in rng:
            array += "    " + str(delay) + ", " + str(increment) + ",\n"
        array += "    STOP\n};"
        hfile.write("extern const uint16_t " + arrayname + "[" + str(2 * len(rng) + 1) + "] PROGMEM;\n\n")
        cfile.write(array)
        cfile.write("\n\n")

        arrayname = "explosion"
        delay = 100
        array = "const uint16_t " + arrayname + "[] PROGMEM = {\n"
        rng = range(0, 1000, 100) + [(1.0 / 4 * i) ** 2 for i in range(126, 0, -1)]
        for increment in rng:
            array += "    " + str(delay) + ", " + str(increment) + ",\n"
        array += "    STOP\n};"
        hfile.write("extern const uint16_t " + arrayname + "[" + str(2 * len(rng) + 1) + "] PROGMEM;\n\n")
        cfile.write(array)
        cfile.write("\n\n")


        hfile.write("#endif")
