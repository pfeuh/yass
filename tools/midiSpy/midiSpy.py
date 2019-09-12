#!/usr/bin/python
# -*- coding: utf-8 -*-

 #
 # file : midiSpy.py
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

import MIDI
import time
import sys
import midiMessage

def textToBytesList(txt):
    words = [eval("0x" + word) for word in txt.split()]
    return words

if __name__ == "__main__":
    
    sys.argv = [sys.argv[0], "-i", "2", "-o", "4"]
    midiIn, midiOut = MIDI.getInputOutput()

    seq5Request = [0xF0, 0x79, 0x61, 0x73, 0x02, 0x04, 0xF7]   
    seq5Dump = textToBytesList("F0 79 61 73 42 4 32 35 39 3e 39 39 3B 3B 3E 3E 3B 3B 39 39 36 36 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 03 01 0F 00 7F 7F F7")
    seq5 = textToBytesList("F0 79 61 73 42 04 32 35 39 3e 39 39 3B 3B 3E 3E 3B 3B 39 39 36 36 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 03 01 0F 00 7F 7F F7")
    seq1 = textToBytesList("F0 79 61 73 42 00 32 35 39 3e 39 39 3B 3B 3E 3E 3B 3B 39 39 36 36 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 7F 03 01 0F 00 7F 7F F7")
    globalDump = textToBytesList("F0 79 61 73 41 00 01 02 01 00 F7")
    globalRequest = [0xF0, 0x79, 0x61, 0x73, 0x01, 0xF7]
    allRequest = [0xF0, 0x79, 0x61, 0x73, 0x03, 0xF7]
    
    #~ midiOut.write_sys_ex(0, seq5Request)
    #~ print ("command seq5 sended!")
    #~ midiOut.write_sys_ex(0, seq5)
    #~ print ("sequence sended!")
    #~ midiOut.write_sys_ex(0, globRequest)
    #~ print ("command globRequest sended!")
    #~ midiOut.write_sys_ex(0, seq5Dump)
    #~ print ("command seq5Dump sended!")
    #~ midiOut.write_sys_ex(0, globalRequest)
    #~ print ("command globalRequest sended!")
    #~ midiOut.write_sys_ex(0, allRequest)
    #~ print ("command allRequest sended!")
    #~ time.sleep(1.0)

    milestone = time.time() + 15.0
    #~ while milestone >= time.time():
    while 1:
        if midiIn.poll():
            temp_data = midiIn.read(1)
            temp_msg = midiMessage.MIDI_MESSAGE(temp_data[0][0], temp_data[0][1])
            print temp_msg
            #~ temp_msg = midiIn.read(1)
            #~ print temp_msg
        else:
            time.sleep(0.001)
    
    print("\n\nExiting %s"%__file__)
    #~ MIDI.midi.Output.close(midiOut)
    #~ MIDI.midi.Input.close(midiIn)
    midiOut.close()
    midiIn.close()

