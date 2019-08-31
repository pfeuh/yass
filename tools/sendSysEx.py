#!/usr/bin/python
# -*- coding: utf-8 -*-

from pygame import midi
import time
import sys

midi.init()
midi_device_id = 0
while 1:
    description =  midi.get_device_info(midi_device_id)
    if description != None:
        # if description[3] == 1, it's an output
        if description[3] == 1:
            print midi_device_id, description[1]
        midi_device_id += 1
    else:
        break
    
midiout_id  = None
if len(sys.argv) != 1:
    midiout_id  = int(sys.argv[1])
else:
    midiout_id = int(raw_input("---------------------------\nselect midi output:"))
print "---------------------------\nselected midi output: %i", midiout_id
midiout = midi.Output(midiout_id)

def textToBytesList(txt):
    words = [eval("0x" + word) for word in txt.split()]
    return words

if __name__ == "__main__":

    seq5Request = [0xF0, 0x79, 0x61, 0x73, 0x02, 0x04, 0xF7]   
    seq5 = textToBytesList("F0 79 61 73 42 04 32 35 39 3e 39 39 3B 3B 3E 3E 3B 3B 39 39 36 36 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 03 01 0F 00 7F 7F F7")
    seq1 = textToBytesList("F0 79 61 73 42 00 32 35 39 3e 39 39 3B 3B 3E 3E 3B 3B 39 39 36 36 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 03 01 0F 00 7F 7F F7")
    globRequest = [0xF0, 0x79, 0x61, 0x73, 0x01, 0x04, 0xF7]
    
    #~ midiout.write_sys_ex(0, Seq5Request)
    #~ print ("command seq5 sended!")
    #~ midiout.write_sys_ex(0, seq5)
    #~ print ("sequence sended!")
    midiout.write_sys_ex(0, globRequest)
    print ("command globRequest sended!")
    time.sleep(1)

