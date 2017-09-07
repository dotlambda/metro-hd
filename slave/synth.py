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
        assert(time < 2 ** 32) # times are stored as uint32_t
        #TODO check relative error
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
            if not (includeTracks==[] or i in includeTracks):
                continue
            t=0
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
        tempo=600000
        events.sort(key=lambda tup: tup[0])
        tones_array = "const PROGMEM uint8_t " + arrayname + "_tones[] = {\n    "
        times_array = "const PROGMEM uint32_t " + arrayname + "_times[] = {\n    "
        nr_note_events = 0
        for e in events:
            time = e[0]*10**(-3)/480.*tempo # in ms
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
        cfile.write(tones_array)
        cfile.write("\n")
        cfile.write(times_array)
        cfile.write("\n\n")

with open(hfilename, "w") as hfile:
    with open(cfilename, "w") as cfile:
        synth=Synth(60 - 36, hfile, cfile)
        # for ... in os.walk("../music"):
        synth.writeCArray("elise2.mid", "elise")
