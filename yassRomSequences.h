#ifndef yassRomSequences_h
#define yassRomSequences_h

/*
 * file : yassRomSequences.h
 * Copyright (c) pfeuh <ze.pfeuh@gmail>
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Arduino.h>
#include "yassSequence.h"

#define YASS_ROM_SEQUENCES_VERSION "1.00"

#define YASS_ROM_SEQUENCE_BILLIE_JEAN       1
#define YASS_ROM_SEQUENCE_MIDNIGHT_EXPRESS  2
#define YASS_ROM_SEQUENCE_BLADE_RUNNER      3
#define YASS_ROM_SEQUENCE_DADY_COOL         4
#define YASS_ROM_SEQUENCE_MARCIA_BAILAR     5
#define YASS_ROM_SEQUENCE_TAKE_FIVE         6
#define YASS_ROM_SEQUENCE_LATIN_BOLERO      7
#define YASS_ROM_SEQUENCE_WOOGIE_BOOGIE     8
#define YASS_ROM_SEQUENCE_TUBULAR_BELLS     9
#define YASS_ROM_SEQUENCE_OXYGENE_4         10
#define YASS_ROM_SEQUENCE_CLAVE             11
#define YASS_ROM_SEQUENCE_MINOR_ARPEGGIO    12
#define YASS_ROM_SEQUENCE_MAJOR_ARPEGGIO    13
#define YASS_ROM_SEQUENCE_DIM_ARPEGGIO      14
#define YASS_ROM_SEQUENCE_PENTA_ARPEGGIO    15
#define YASS_ROM_SEQUENCE_AUG_ARPEGGIO      16

#define YASS_ROM_SEQUENCE_FIRST             YASS_ROM_SEQUENCE_BILLIE_JEAN
#define YASS_ROM_SEQUENCE_LAST              YASS_ROM_SEQUENCE_CLAVE

void YASS_ROM_SEQUENCES_load(byte rom_seq_num, YASS_SEQUENCE* seq);

#endif
