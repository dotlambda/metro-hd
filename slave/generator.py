#!/usr/bin/env python2

# Python Skript zur Erzeugung von Waveform-Lookuptablen
# Copyright: Benjamin Reh, 2015
# Source: http://joannna.iwr.uni-heidelberg.de/resources/synthesizer/generator.py

import math

f_osc=16000000
f_sample=f_osc/64/25
# Wie viele Samples minimal?
n_samples_min=100

print "sample frequency:", f_sample,"Hz"



class toneTable:
	def __init__(self):
		self.toneList=[]
		self.toneLength=[]
		self.toneOffset=[]
		self.offset=0;
	
	def makeTone(self,f):
		f=float(f)
		#Anzahl der Samples bei gegebener Frequenz fuer eine Periode
		n_samples_float=f_sample/f
		n_samples=int(n_samples_float+.5)

		#Anzahl der Perioden zur Erhoehung der Genauigkeit
		n_periods=int(float(n_samples_min)/n_samples+.5)
		#Mindestens eine Periode sollte es schon sein
		n_periods=max(1,n_periods)
		#~ n_periods=1
		
		#Anzahl erneut berechnen
		n_samples_float=f_sample/f*n_periods
		n_samples=int(n_samples_float+.5)
		
		
		print "Anzahl Perioden: ",n_periods
		print "Anzahl Samples: ",n_samples,n_samples_float
		f_real=float((f_sample))/(n_samples)*n_periods
		print "tatsaechliche Frequenz", f_real,"anstatt",f,"Fehler",100.-(f_real)/f*100.,"%"
		
		
		self.toneLength.append(n_samples)
		self.toneOffset.append(self.offset)
		self.offset+=n_samples;
		for i in range(n_samples):
			x=float(i*n_periods)/float(n_samples)*math.pi*2.
			value=math.sin(x)
			#~ value=(math.sin(x)+math.sin(3*x)/6.)/1.5
			
			value=int((value+1.)/2. * 255.)
			
			self.toneList.append(value)
			
	def writeCFile(self,filename):
		ramUsed=0
		flashUsed=0
		f=open(filename,"w")
		f.write("#include <inttypes.h>\n")
		f.write("#include <avr/pgmspace.h>\n")
		f.write("const uint8_t tones[] PROGMEM ={")
		s=""
		for v in self.toneList:
			s+=str(v)+","
			flashUsed+=1;
		#letztes Komma zu viel
		s=s[0:-1]
		f.write(s+"};\n")
		f.write("uint16_t toneLength[]={")
		s=""
		for v in self.toneLength:
			s+=str(v)+","
			ramUsed+=2;
		#letztes Komma zu viel
		s=s[0:-1]
		f.write(s+"};\n")
		f.write("uint16_t toneOffset[]={")
		s=""
		for v in self.toneOffset:
			s+=str(v)+","
			ramUsed+=1;
		#letztes Komma zu viel
		s=s[0:-1]
		f.write(s+"};\n")
		f.write("#define NO_TONES "+str(len(self.toneLength))+"\n")
		f.close()
		print "RAM", ramUsed
		print "flash", flashUsed
		
		
		



t=toneTable()
#Kammerton A (440Hz) +/- 3 Oktaven
for i in range(-3*12,12*3):
	f=440 * math.pow(2,(i-10)/12.)
	#~ print i,f
	t.makeTone(f);


t.writeCFile("tone.h")


