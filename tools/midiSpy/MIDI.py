#!/usr/bin/python
# -*- coding: utf-8 -*-

 #
 # file : MIDI.py
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

from pygame import midi
import time
import sys

midi.init()

def selectOutput():
    index = 0
    midi_output = None
    while 1:
        description =  midi.get_device_info(index)
        if description != None:
            if description[3] == 1: # it's a midi output
                print("%i %s"%(index, description[1]))
            index += 1
        else:
            break
    txt = input("Select a midi output device")
    if txt != "":
        midi_output = midi.Output(int(txt))
    return midi_output

def selectInput():
    index = 0
    midi_input = None
    while 1:
        description =  midi.get_device_info(index)
        if description != None:
            if description[3] == 0: # it's a midi output
                print("%i %s"%(index, description[1]))
            index += 1
        else:
            break
    txt = input("Select a midi input device")
    if txt != "":
        midi_input = midi.Input(int(txt))
    return midi_input

def getInputOutput():
    midi_input = None
    midi_output = None

    for x, arg in enumerate(sys.argv):
        if arg == "-i":
            if len(sys.argv) <= x + 1:
                raise Exception("Bad parameter")
            else:
                midi_input = midi.Input(int(sys.argv[x+1]))
        if arg == "-o":
            if len(sys.argv) <= x + 1:
                raise Exception("Bad parameter")
            else:
                midi_output = midi.Output(int(sys.argv[x+1]))

    if midi_input == None:
        midi_input = selectInput()
    if midi_output == None:
        midi_output = selectOutput()

    return midi_input, midi_output

if __name__ == "__main__":

    print getInputOutput()
    
    