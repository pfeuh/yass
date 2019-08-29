#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys

for index in range(16, 128):
    label = ""
    if index >= 32:
        label = chr(index)
    if index in (0x5c, 0x60,0x7e, 0x7f):
        label = ""
    sys.stdout.write("   B00000000, // ascii code 0x%02x %s\n"%(index, label))