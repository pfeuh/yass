#!/usr/bin/python
# -*- coding: utf-8 -*-

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
    
    