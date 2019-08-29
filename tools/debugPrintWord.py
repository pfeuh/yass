#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys

def printWord(value, base):
    for digit_num in range(NB_DIGITS -1, -1, -1):
        rest = value % base;
        value = value / base;
        sys.stdout.write("%01x"%rest)

NB_DIGITS = 4

printWord(1234, 16)