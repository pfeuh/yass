#!/usr/bin/python
# -*- coding: utf-8 -*-

 #
 # file : build7SegFontSkeleton.py
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

import sys

for index in range(16, 128):
    label = ""
    if index >= 32:
        label = chr(index)
    if index in (0x5c, 0x60,0x7e, 0x7f):
        label = ""
    sys.stdout.write("   B00000000, // ascii code 0x%02x %s\n"%(index, label))