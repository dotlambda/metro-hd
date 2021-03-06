#! /usr/bin/env python3

import os
from skimage import io
from numpy import transpose
from math import ceil

with open('sprites.h', 'w') as hfile:
    with open('sprites.c', 'w') as cfile:
        hfile.write('#ifndef SPRITES_H\n#define SPRITES_H\n\n')
        hfile.write('#include <inttypes.h>\n\n')
        hfile.write('#include <avr/pgmspace.h>')
        cfile.write('#include "sprites.h"\n\n')

        for dirpath, dirnames, filenames in os.walk('../sprites'):
            dirnames.sort()
            filenames.sort()
            for filename in filenames:
                base, extension = os.path.splitext(filename)
                if extension != '.png':
                    continue

                img = io.imread(dirpath + '/' + filename, as_grey=True)
                images = [img]
                names = [base]
                
                if filename[:5] == 'floor':
                    images.append(transpose(img))
                    names.append(base + '_rotated')
                
                for name, image in zip(names, images):
                    # colour values are floats between 0.0 and 1.0
                    assert image.min() >= 0.0
                    assert image.max() <= 1.0

                    width = image.shape[1]
                    height = image.shape[0]
                    length = width * ceil(height / 4)

                    carray = 'const PROGMEM uint8_t ' + name + '[' + str(length) + '] = {\n    '
                    for y in range(0, height, 4):
                        for x in range(width):
                            carray += '0b'
                            for i in [3,2,1,0]:
                                if y + i < height:
                                    if image[y+i, x] < 1/4: # black
                                        carray += '11'
                                    elif image[y+i, x] < 2/4:
                                        carray += '10'
                                    elif image[y+i, x] < 3/4:
                                        carray += '01'
                                    else:
                                        carray += '00'
                                else:
                                    carray += '00'
                            carray += ', '
                        carray += '\n    '
                    carray = carray[:-7] + '\n};\n\n'
                    
                    hfile.write('// ' + str(image.shape[0]) + 'x' + str(image.shape[1]) + '\n')
                    hfile.write('extern PROGMEM const uint8_t ' + name + '[' + str(length) + '];\n\n')
                    cfile.write(carray)
        hfile.write('\n#endif')
