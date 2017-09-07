#!/usr/bin/python

import serial, time
from mido import MidiFile

n_octave=6

# $ pip install mido

class Synth():
	def __init__(self):

		self.ser = serial.Serial()
		self.ser.port = "/dev/ttyS0"
		self.ser.baudrate = 9600
		self.ser.bytesize = serial.EIGHTBITS #number of bits per bytes
		self.ser.parity = serial.PARITY_NONE #set parity check: no parity
		self.ser.stopbits = serial.STOPBITS_ONE #number of stop bits
		self.ser.xonxoff = False     #disable software flow control
		self.ser.rtscts = False     #disable hardware (RTS/CTS) flow control
		self.ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
		self.noteOffset=60-36
			
		try: 
			self.ser.open()
		except Exception, e:
			print "error open serial port: " + str(e)
			exit()
		self.ser.write(chr(255));

	def setTone(self,nr, onOff):
		if nr<0 or nr>n_octave*12:
			print "-----------",nr,"out of range"
			return
		if onOff:
			self.ser.write(chr(nr|(1<<7)))
		else:
			self.ser.write(chr(nr))
	def playMidi(self,filename,includeTracks=[]):
		mid = MidiFile(filename)
		events=[]
		print "Number of tracks:",len(mid.tracks)
		tempo=600000
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
		events.sort(key=lambda tup: tup[0])
		last_time=0
		t=0
		for e in events:
			t=e[0]
			time.sleep(((t-last_time)*10**(-6)/480.*tempo))
			last_time=t
			ev=e[1]
			if ev["type"]=="note_on":
				print "PLAY  ",ev["note"]-self.noteOffset
				self.setTone(ev["note"]-self.noteOffset,True);
			elif ev["type"]=="note_off":
				print "STOP ",ev["note"]-self.noteOffset
				self.setTone(ev["note"]-self.noteOffset,False);
			elif ev["type"]=="set_tempo":
					tempo=ev["tempo"]
					print "TEMPO ", tempo,"=",60000000./tempo,"BPM"
					
					


synth=Synth()


synth.noteOffset=60-36
synth.playMidi("elise2.mid")

#~ synth.setTone(0,True)
#~ time.sleep(1)
#~ synth.setTone(0,False)


