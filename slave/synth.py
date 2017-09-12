#!/usr/bin/env python2

from mido import MidiFile

F_CPU = 16000000

hfilename = "music.h"
cfilename = "music.c"

class Synth():
    def __init__(self, hfile, cfile):
        self.hfile = hfile
        self.cfile = cfile
    def writeCArray(self,filename,arrayname,includeTracks=[]):
        mid = MidiFile(filename)
        events=[]
        for i, track in enumerate(mid.tracks):
            t = 0
            if not (includeTracks==[] or i in includeTracks):
                continue
            for message in track:
                t+=message.time
                if (message.type=="note_on" or message.type=="note_off") :
                    e={}
                    if message.velocity==0:
                        e["type"]="note_off"
                    else:
                        e["type"]=message.type
                    e["note"]=message.note
                    events.append((t,e))
                elif message.type=="set_tempo":
                    e={}
                    e["type"]=message.type
                    e["tempo"]=message.tempo
                    events.append((t,e))
        tempo = 600000
        events.sort(key = lambda tup: tup[0])
        notes = set()
        note = None
        last_time = 0
        t = 0
        changes = []
        for e in events:
            ev = e[1]
            if ev["type"]=="set_tempo":
                tempo = ev["tempo"]
            elif ev["type"] == "note_on":
                notes.add(ev["note"])
            elif ev["type"] == "note_off":
                # should be notes.remove() but MIDI files are not coherent
                notes.discard(ev["note"])
            if len(notes) > 0:
                if note not in notes or max(notes) > note:
                    note = max(notes)
                    t = e[0]
                    delay = int((t - last_time) * 10 ** (-3) / 480. * tempo) # in ms
                    last_time = t
                    freq = 2.0 ** ((note - 69) / 12.0) * 440.0 # in Hz
                    period = 1.0 / freq # in seconds
                    # value for compare register with prescaler=8
                    # /2 because the timer counts up twice per period
                    compare = F_CPU * period / 8 / 2
                    if delay == 0 and len(changes) > 0:
                        changes[-1]["freq"] = int(compare)
                    else:
                        changes.append({"delay": delay, "freq": int(compare)})
        array = "const uint16_t " + arrayname + "[] PROGMEM = {\n"
        for change in changes:
            delay = change["delay"]
            assert(delay < 2 ** 16)
            freq = change["freq"]
            #assert(freq < 2 ** 16)
            array += "    " + str(delay) + ", " + str(freq) + ",\n"
        array += "    0\n};"
        hfile.write("extern const uint16_t " + arrayname + "[" + str(2 * len(changes) + 1) + "] PROGMEM;\n\n")
        cfile.write(array)
        cfile.write("\n\n")

with open(hfilename, "w") as hfile:
    with open(cfilename, "w") as cfile:
        cfile.write("#include \"music.h\"\n\n")
        hfile.write("#ifndef MUSIC_H\n#define MUSIC_H\n\n#include <inttypes.h>\n#include <avr/pgmspace.h>\n\n")
        synth=Synth(hfile, cfile)
        # for ... in os.walk("../music"):
        synth.writeCArray("../music/ussr1.mid", "elise")
        hfile.write("#endif")
