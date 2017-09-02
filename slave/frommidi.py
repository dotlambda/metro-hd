#! /usr/bin/env python2

import os
import midi

with open('music.h', 'w') as hfile:
    with open('music.c', 'w') as cfile:
        hfile.write('#ifndef MUSIC_H\n#define MUSIC_H\n\n')
        hfile.write('#include <inttypes.h>\n\n')
        hfile.write('#include <avr/pgmspace.h>')
        cfile.write('#include "music.h"\n\n')

        for dirpath, dirnames, filenames in os.walk('../music'):
            dirnames.sort()
            filenames.sort()
            for filename in filenames:
                base = filename.split('.')[0]
                extension = filename.split('.')[1]
                if extension != 'mid':
                    continue

                pattern = midi.read_midifile(dirpath + '/' + filename)
                tracks = []
                us_per_tick = 0
                for track in pattern:
                    name = ''
                    tones = []
                    delays = []
                    for event in track:
                        if isinstance(event, midi.SetTempoEvent):
                            us_per_tick = 60 * 100000 / event.bpm / pattern.resolution
                        elif isinstance(event, midi.TrackNameEvent):
                            name = event.text
                        elif isinstance(event, midi.NoteOnEvent):
                            on = event.velocity != 0
                            tones.append((event.pitch, on))
                            delays.append(event.tick * us_per_tick) # in ms
                        elif isinstance(event, midi.NoteOffEvent):
                            tones.append((event.pitch, False))
                            delays.append(event.tick * us_per_tick) # in ms
                    if len(tones) != 0:
                        tracks.append((name, tones, delays))

                tones_array = 'const PROGMEM uint8_t ' + base + '_tones[' + str(len(tracks)) + '][] = {\n'
                events_array = 'const PROGMEM uint8_t ' + base + '_events[' + str(len(tracks)) + '][] = {\n'
                delays_array = 'const PROGMEM uint16_t ' + base + '_delays[' + str(len(tracks)) + '][] = {\n'
                for name, tones, delays in tracks:
                    tones_array += '    // ' + name + '\n    { '
                    events_array += '    // ' + name + '\n    { '
                    delays_array += '    // ' + name + '\n    { '
                    for i in range(len(tones)):
                        # In tones.h, the tones begin with the A1, which is pitch 33
                        # http://newt.phys.unsw.edu.au/jw/notes.html
                        tone = tones[i][0] - 33
                        assert(tone > 0)
                        # There are 6 octaves in tones.h
                        assert(tone < 72)
                        tones_array += str(tone) + ', '
                        if tones[i][1]:
                            events_array += '1, '
                        else:
                            events_array += '0, '
                        delays_array += str(int(delays[i])) + ', '
                    tones_array = tones_array[:-2] + ' },\n'
                    events_array = events_array[:-2] + ' },\n'
                    delays_array = delays_array[:-2] + ' },\n'
                tones_array = tones_array[:-2] + '\n};\n'
                events_array = events_array[:-2] + '\n};\n'
                delays_array = delays_array[:-1] + '\n};\n\n'

                hfile.write('extern PROGMEM const uint8_t ' + base + '_tones**;\n')
                hfile.write('extern PROGMEM const uint8_t ' + base + '_events**;\n')
                hfile.write('extern PROGMEM const uint16_t ' + base + '_delays**;\n\n')
                cfile.write(tones_array)
                cfile.write(events_array)
                cfile.write(delays_array)

        hfile.write('\n#endif')
