#!/usr/bin/env python
# coding: utf-8

 #
 # file : midiMessage.py
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

from midiConstants import *

class MIDI_MESSAGE():
    def __init__(self, bytes = [], number = None):
        self.__bytes = bytes
        self.__number = number
        #~ print "bytes", self.__bytes
        #~ print "number", self.__number

    def get14Bits(self, raw_value1, raw_value2):
        return "%i"%(raw_value1 * 0x80 +  raw_value2)

    def getCommand(self):
        if len(self.__bytes):
            return self.__bytes[0]

    def getBytes(self, start = 0, stop = None):
        ret_str = ""
        if stop == None:
            stop = len(self.__bytes)
        for x in range(start, stop):
            ret_str += str(self.__bytes[x]) + " "
        return ret_str

    def __str__(self):
        ret_string = ""
        if len(self.__bytes)!= 0:
            #~ temp_cmd = self.__bytes[0] & 0xF0
            temp_cmd = self.__bytes[0]
            temp_channel = (self.__bytes[0] & 0x0F) + 1
            if   (temp_cmd & 0xf0) == MIDI_CMD_OFF:
                ret_string = "OFF(%i) "%temp_channel
                ret_string += self.getBytes(1,3)
            elif (temp_cmd & 0xf0) == MIDI_CMD_ON:
                ret_string = "ON(%i) "%temp_channel
                ret_string += self.getBytes(1,3)
            elif (temp_cmd & 0xf0) == MIDI_CMD_CC:
                ret_string = "CC(%i) "%temp_channel
                ret_string += self.getBytes(1,3)
            elif (temp_cmd & 0xf0) == MIDI_CMD_PC:
                ret_string = "PC(%i) "%temp_channel
                ret_string += self.getBytes(1,2)
            elif temp_cmd == MIDI_RT_CLOCK:
                ret_string = "clock"
            elif temp_cmd == MIDI_RT_START:
                ret_string = "start"
            elif temp_cmd == MIDI_RT_STOP:
                ret_string = "stop"
            elif temp_cmd == MIDI_RT_CONT:
                ret_string = "continue"
            else:
                ret_string = ""
                for x in range(len(self.__bytes)):
                    ret_string += "%02X "%self.__bytes[x]
        else:
            ret_string = "??? "
            for x in range(len(self.__bytes)):
                ret_string += "%02X "%self.__bytes[x]
        return ret_string
        
if __name__ == "__main__":
    
    for message in [
        MIDI_MESSAGE((MIDI_CMD_OFF, 10, 11, 12, 13),),
        MIDI_MESSAGE((MIDI_CMD_OFF+15, 10, 11, 12, 13),),
        MIDI_MESSAGE((MIDI_CMD_ON, 10, 11, 12, 13),),
        MIDI_MESSAGE((MIDI_CMD_CC, 10, 11, 12, 13),),
        MIDI_MESSAGE((MIDI_CMD_PC, 10, 11, 12, 13),),
        MIDI_MESSAGE((255, 10, 11, 12, 13),),
        MIDI_MESSAGE((17, 10, 11, 12, 13),),
        MIDI_MESSAGE((MIDI_RT_CLOCK,),),
        MIDI_MESSAGE((MIDI_RT_START,),),
        MIDI_MESSAGE((MIDI_RT_STOP,),),
        MIDI_MESSAGE((MIDI_RT_CONT,),),
        ]:
        print message, " - ", message.getBytes()
