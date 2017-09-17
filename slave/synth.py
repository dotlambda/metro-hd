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
        array = "const uint16_t " + arrayname + "[] PROGMEM = {\n"
        for change in changes:
            delay = change["delay"]
            #assert(delay < 2 ** 16)
            track = change["track"]
            increment = change["increment"]
            assert(increment < 2 ** 16)
            array += "    " + str(delay) + ", " + str(track) + ", " + str(increment) + ",\n"
        array += "    STOP\n};"
        hfile.write("extern const uint16_t " + arrayname + "[" + str(3 * len(changes) + 1) + "] PROGMEM;\n\n")
        cfile.write(array)
        cfile.write("\n\n")

with open(hfilename, "w") as hfile:
    with open(cfilename, "w") as cfile:
        cfile.write("#include \"music.h\"\n\n")
        hfile.write("#ifndef MUSIC_H\n#define MUSIC_H\n\n#include <inttypes.h>\n#include <avr/pgmspace.h>\n\n")
        hfile.write("#define STOP 0xFFFF\n\n")
        synth=Synth(hfile, cfile)
        # for ... in os.walk("../music"):
        #synth.writeCArray("../../bomb_explosion_1.wav.mid", "elise", True)
        #synth.writeCArray("../music/sevenfortyam.mid", "splash", True)
        #synth.writeCArray("../music/Combat 2.mid", "splash", True, 4, [3])
        #synth.writeCArray("../music/SERAPHO.MID", "splash", True, 1, [1], .5)
        #synth.writeCArray("../music/Boss_Musik_2.mid", "boss1", True)
        #synth.writeCArray("../music/Boss_Musik_1_remastered.mid", "boss3", True)
        #synth.writeCArray("../music/title.mid", "splash", True)
        #synth.writeCArray("../music/Ingame_Musik_Tief.mid", "ingame", True)
        #synth.writeCArray("../music/Game_Over.mid", "gameover", True) 
        #synth.writeCArray("../music/combat.mid", "splash", True, timescale = 0.8)
        
        #synth.writeCArray("../music/Keith-ou-Passe-Bad.mid", "splash", [1,4], timescale=3.5) # triangle
        #synth.writeCArray("../music/HP1.MID", "splash", [1,2,14], timescale=4)
        #synth.writeCArray("../music/PER.MID", "splash", [1], timescale=4)
        synth.writeCArray("../music/boss6.mid", "boss2", [0], timescale=2)

        hfile.write("#endif")
