#!/usr/bin/env python2

from mido import MidiFile

n_octave=6

hfilename = "music.h"
cfilename = "music.c"

class Synth():
    def __init__(self, noteOffset, hfile, cfile):
        self.noteOffset = noteOffset
        self.hfile = hfile
        self.cfile = cfile
    def appendTone(self,nr, onOff):
        if nr<0 or nr>n_octave*12:
            print "-----------",nr,"out of range"
            return False
        assert(nr < (1 << 7)) # the most significant bit is used for storing onOff
        # append tone to array
        array = ""
        if onOff:
            array += str(nr | (1 << 7))
        else:
            array += str(nr)
        array += ", "
        return array
    def appendTime(self, time): # time is in ms
        print time
        assert(time < 2 ** 16) # times are stored as uint16_t
        #TODO check relative error of conversion to ms
        # append time to array
        array = str(int(round(time)))
        array += ", "
        return array
    def writeCArray(self,filename,arrayname,includeTracks=[]):
        mid = MidiFile(filename)
        events=[]
        print "Number of tracks:",len(mid.tracks)
        for i, track in enumerate(mid.tracks):
            print('Track {}: {}'.format(i, track.name))
            t = 0
            if not (includeTracks==[] or i in includeTracks):
                continue
            for message in track:
                t+= message.time
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
        tempo=600000
        events.sort(key=lambda tup: tup[0])
        tones_array = "const PROGMEM uint8_t " + arrayname + "_tones[] = {\n    "
        times_array = "const PROGMEM uint16_t " + arrayname + "_times[] = {\n    "
        nr_note_events = 0
        t = 0
        for e in events:
            # calculate the number of ms to wait after previous event
            time = (e[0] - t) * 10**(-3) / 480. * tempo # in ms
            t += e[0]
            ev=e[1]
            if ev["type"]=="note_on":
                print "PLAY  ",ev["note"]-self.noteOffset
                new_tone = self.appendTone(ev["note"]-self.noteOffset,True)
                if new_tone:
                    tones_array += new_tone
                    times_array += self.appendTime(time)
                    nr_note_events += 1
            elif ev["type"]=="note_off":
                print "STOP ",ev["note"]-self.noteOffset
                new_tone = self.appendTone(ev["note"]-self.noteOffset,False)
                if new_tone:
                    tones_array += new_tone
                    times_array += self.appendTime(time)
                    nr_note_events += 1
            elif ev["type"]=="set_tempo":
                tempo=ev["tempo"]
                print "TEMPO ", tempo,"=",60000000./tempo,"BPM"
        tones_array = tones_array[:-2] + "\n};"
        times_array = times_array[:-2] + "\n};"
        hfile.write("extern PROGMEM const uint8_t " + arrayname + "_tones[" + str(nr_note_events) + "];\n")
        hfile.write("extern PROGMEM const uint16_t " + arrayname + "_times[" + str(nr_note_events) + "];\n\n")
        cfile.write(tones_array)
        cfile.write("\n")
        cfile.write(times_array)
        cfile.write("\n\n")

with open(hfilename, "w") as hfile:
    with open(cfilename, "w") as cfile:
        cfile.write("#include \"music.h\"\n\n")
        hfile.write("#ifndef MUSIC_H\n#define MUSIC_H\n\n#include <inttypes.h>\n#include <avr/pgmspace.h>\n\n")
        synth=Synth(60 - 36, hfile, cfile)
        # for ... in os.walk("../music"):
        synth.writeCArray("elise2.mid", "elise")
        hfile.write("#endif")
