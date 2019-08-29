#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
1-8 	Piano
9-16 	Chromatic Percussion
17-24 	Organ
25-32 	Guitar
33-40 	Bass
41-48 	Strings
49-56 	Ensemble
57-64 	Brass
65-72 	Reed
73-80 	Pipe
81-88 	Synth Lead
89-96 	Synth Pad
97-104 	Synth Effects
105-112 	Ethnic
113-120 	Percussive
121-128 	Sound Effects
"""

import sys

HEADER = "const char voiceLabelLut[]   PROGMEM = \n"
FAMILIES = "Pia CHP Org Gui BAS Stg Ens Brs Ree Pip Lea Pad Eff Eth Prc SEF".split(" ")
FAMILY_NB_MEMBERS = 8


print FAMILIES

def writeMidiNotesLut():
    sys.stdout.write("\n\n")
    sys.stdout.write(HEADER)
        
    for fname in FAMILIES:
        line = "    "
            
        for index in range(FAMILY_NB_MEMBERS):
            label = fname + str(index + 1)
            line += '"%s" '%label
        
        sys.stdout.write(line)
        sys.stdout.write("\n")

    sys.stdout.write("    ;\n")
    

if __name__ == "__main__":
    
    writeMidiNotesLut()