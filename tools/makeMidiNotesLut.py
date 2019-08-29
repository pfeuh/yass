#!/usr/bin/python
# -*- coding: utf-8 -*-

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