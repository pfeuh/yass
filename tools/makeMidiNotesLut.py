#!/usr/bin/python
# -*- coding: utf-8 -*-

 #
 # file : makeMidiNotesLut.py
 # Copyright (c) pfeuh <ze.pfeuh@gmail>
 # All rights reserved.
 # 
 # This program is free software: you can redistribute it and/or modify
 # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation, either version 3 of the License, or
 # (at your option) any later version.
 # 
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU General Public License for more details.
 # 
 # You should have received a copy of the GNU General Public License
 # along with this program.  If not, see <http://www.gnu.org/licenses/>.
 #

import sys

HEADER = "const char noteLabelLut[]   PROGMEM = \n"

NOTE_LABELS = ["C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"]

STAVE_SIZE = 12
STAVE_LAST_NOTE = STAVE_SIZE - 1

def writeMidiNotesLut():
    sys.stdout.write("\n\n")
    sys.stdout.write(HEADER)
        
    for x in range(129):
        if x % STAVE_SIZE == 0:
            line = "    "
            
        stave_num = (x / STAVE_SIZE) - 1
        note_num = x % STAVE_SIZE
        #~ sys.stdout.write("%u %u %u\n"%(x, stave_num, note_num))

        if x == 0:
            label = "REST"
        elif x == 128:
            label = "TIE "
        else:
            label = "%-4s"%(NOTE_LABELS[note_num] + str(stave_num))
            assert len(label) == 4
        
        line += '"%s" '%label
        
        if x % STAVE_SIZE == STAVE_LAST_NOTE:
            sys.stdout.write(line)
            sys.stdout.write("\n")

    if line != "    ":
        sys.stdout.write(line)
        sys.stdout.write("\n")
    sys.stdout.write("    ;\n")
    

if __name__ == "__main__":
    
    writeMidiNotesLut()