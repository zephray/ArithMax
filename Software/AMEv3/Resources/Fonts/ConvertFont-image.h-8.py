#!/usr/bin/env python

__license__ = """
Copyright (c) 2014 StarBrilliant <m13253@hotmail.com>

This file is part of ArithMax firmware.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this program.  If not,
see <http://www.gnu.org/licenses/>.
"""

import sys


def main():
    sys.stdout.write('/*\n *')
    sys.stdout.write(__license__.replace('\n', '\n * '))
    sys.stdout.write('''
 */
#ifndef __IMAGE_H_FONT_8
#define __IMAGE_H_FONT_8
/* This file is generated automatically by ConvertFont-image.h-8.py, do not edit it. */
const unsigned char Font_Ascii_6X8E[96*8] = {
''')
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        line = line.split()
        if line[0] == 'ENCODING':
            charenc = int(line[1])  # Assume stored in order
        elif line[0] == 'BBX':
            sizex, sizey, offx, offy = map(int, line[1:5])
        elif line[0] == 'BITMAP':
            bitmap = [0]*(7-sizey-offy)
            while True:
                line = sys.stdin.readline().strip()
                if line == 'ENDCHAR':
                    bitmap.extend([0]*(8-len(bitmap)) )
                    output_char(bitmap, charenc, sys.stdout)
                    break
                else:
                    bitmap.append(int(line, 16)>>offx)

    sys.stdout.write('''};
#endif
''')


def output_char(bitmap, charenc, f):
    hexarray = ','.join(map(lambda x: '0x%02x' % x, bitmap))
    f.write('    %s, // %r, %d\n' % (hexarray, chr(charenc), charenc-32))


if __name__ == '__main__':
    main()
