#!/usr/bin/python
# -*- coding: utf-8 -*-

 #
 # file : ramComputing.py
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

NB_SEQUENCE_PARAMETERS = 6
NB_GLOBAL_PARAMETERS = 5

def ramComputing(nb_steps, nb_seqs = 1):
    return NB_GLOBAL_PARAMETERS + nb_seqs * (NB_SEQUENCE_PARAMETERS + nb_steps * 2)

if __name__ == "__main__":
    
        for nb_steps in (16, 32, 64):
            for nb_seqs in (4, 5, 8,16):
                sys.stdout.write("%02u seqs %02u steps : %04u RAM bytes\n"%(nb_seqs, nb_steps, ramComputing(nb_steps, nb_seqs)))