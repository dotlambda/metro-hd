#!/usr/bin/env python2

from mido import MidiFile

F_CPU = 16000000.0 # in Hz
INTERRUPT_COMPARE = 128
INTERRUPT_PRESCALER = 8

INTERRUPT_FREQ = F_CPU / (INTERRUPT_PRESCALER * INTERRUPT_COMPARE) # in Hz

hfilename = "music.h"
cfilename = "music.c"

class Synth():
    def __init__(self, hfile, cfile):
        self.hfile = hfile
        self.cfile = cfile
    def writeCArray(self,filename,arrayname,tracks,allow_pause=True,timescale=1.0,pitch=1.0):
        mid = MidiFile(filename)
        events=[]
        for track_index, i in enumerate(tracks):
            track = mid.tracks[i]
            t = 0
            for message in track:
                t+=message.time
                if (message.type=="note_on" or message.type=="note_off") :
                    e={} 
                    if message.velocity==0:
                        e["type"]="note_off"
                    else:
                        e["type"]=message.type
                    e["note"]=message.note
                    events.append((t,track_index,e))
                elif message.type=="set_tempo":
                    e={}
                    e["type"]=message.type
                    e["tempo"]=message.tempo
                    events.append((t,track_index,e))
        tempo = 600000
        events.sort(key = lambda tup: tup[0])
        notes = [set()] * len(tracks)
        note = [None] * len(tracks)
        last_time = 0
        t = 0
        changes = []
        for e in events:
            track = e[1]
            ev = e[2]
            t = e[0]
            delay = timescale * (t-last_time)*10**(-6)/480.*tempo # in seconds
            delay = delay * INTERRUPT_FREQ # in calls of the interrupt
            if filename == "../music/fish forest.MID" and int(delay) == 2500 and ev["type"] == "note_off":
                changes[-1]["delay"] = 3750
                continue
            if ev["type"]=="set_tempo":
                tempo = ev["tempo"]
            elif ev["type"] == "note_on":
                note[track] = ev["note"]
                last_time = t
                freq = pitch * 2.0 ** ((note[track] - 69) / 12.0) * 440.0 # in Hz
                period = INTERRUPT_FREQ / freq # how often the interrupt is called in one period
                increment = 2.0 ** 16 / period
                if delay == 0 and len(changes) > 0 and changes[-1]["track"] == track:
                    changes[-1]["increment"] = int(increment)
                else:
                    changes.append({"delay": int(delay), "track": track, "increment": int(increment)})
            elif ev["type"] == "note_off" and ev["note"] == note[track]:
                note[track] = None
                last_time = t
                increment = 0 # sound off
                if delay == 0.0 and len(changes) > 0 and changes[-1]["track"] == track:
                    changes[-1]["increment"] = increment
                else:
                    changes.append({"delay": int(delay), "track": track, "increment": increment})
        
        array = "const Event " + arrayname + "[] PROGMEM = {\n"
        for i, change in enumerate(changes):
            delay = change["delay"]
            assert(delay < 2 ** 16)
            if i > 0:
                array += ", .delay = " + str(delay) + " } },\n"
            track = change["track"]
            assert(track < 2 ** 2)
            increment = change["increment"]
            assert(increment < 2 ** 14)
            array += "    { { .track = " + str(track) + ", .increment = " + str(increment)
        
        # remove last delay
        i = array[:-1].rfind(".delay = ")
        array = array[:i] + ".delay = 0 } },\n"

        array += "    STOP\n};"
        hfile.write("extern const Event " + arrayname + "[" + str(len(changes) + 1) + "] PROGMEM;\n\n")
        cfile.write(array)
        cfile.write("\n\n")

with open(hfilename, "w") as hfile:
    with open(cfilename, "w") as cfile:
        cfile.write("#include \"music.h\"\n\n")
        hfile.write("#ifndef MUSIC_H\n#define MUSIC_H\n\n#include <inttypes.h>\n#include <avr/pgmspace.h>\n\n")
        hfile.write("// a Change has a size of 32 bits\n"
                  + "typedef struct __attribute__((packed)) {\n"
                  + "    uint16_t delay;\n"
                  + "    unsigned track: 2;\n"
                  + "    unsigned increment: 14;\n"
                  + "} Change;\n")
        hfile.write("typedef union {\n"
                  + "    Change change;\n"
                  + "    uint32_t bits;\n"
                  + "} Event;\n\n")
        hfile.write("#define STOP { { .delay = 0xFFFF, .track = 0, .increment = 0 } }\n\n")
        synth=Synth(hfile, cfile)
        
        synth.writeCArray("../music/Keith-ou-Passe-Bad.mid", "ingame2", [1,4], timescale=3.5) # triangle
        synth.writeCArray("../music/fish forest.MID", "ingame1", [1,3,4], timescale=4)
        
        synth.writeCArray("../music/Boss_Musik_2.mid", "boss1", [0])
        synth.writeCArray("../music/boss6.mid", "boss2", [0], pitch=2)
        synth.writeCArray("../music/Boss_Musik_1_remastered.mid", "boss3", [0])
        synth.writeCArray("../music/organic.MID", "boss4", [1,2,3], timescale=6) 

        synth.writeCArray("../music/Game_Over.mid", "gameover", [0]) 

        hfile.write("#endif")
