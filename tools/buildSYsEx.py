#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys

def getSysEx(user_input):
    words = ["%02x "%int(word.strip()) for word in user_input.split()]
    txt = "F0 %02X %02X %02X %s F7\n"%(ord('y'), ord('a'), ord('s'), "".join(words))
    return txt

if __name__ == "__main__":

    sys.stdout.write("Enter exclusive command in decimal : ")
    userInput = raw_input()
    sys.stdout.write(getSysEx(userInput))

