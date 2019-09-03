/*
 * file : yassEeprom.cpp
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

#include <EEPROM.h>
#include "yassEeprom.h"

#ifndef wpeek
#define wpeek pgm_read_word
#endif

const word seqAddr[] PROGMEM= 
{
    YASS_EEPROM_BASE_SEQ1,
    YASS_EEPROM_BASE_SEQ2,
    YASS_EEPROM_BASE_SEQ3,
    YASS_EEPROM_BASE_SEQ4,
    YASS_EEPROM_BASE_SEQ5
};

/*******************/
/* Private methods */
/*******************/

void YASS_EEPROM::read(word base, byte* target, word nb_bytes)
{
    while(nb_bytes--)
         *target++ = EEPROM.read(base++);
}

void YASS_EEPROM::write(byte* source, word base, word nb_bytes)
{
    while(nb_bytes--)
         EEPROM.update(base++, *source++);
}

/******************/
/* Public methods */
/******************/

YASS_EEPROM::YASS_EEPROM()
{
}

void YASS_EEPROM::begin(YASS_CONFIG* config_ptr, YASS_SEQUENCE* seqs_ptr, YASS_TICKS* ticks_ptr)
{
        configPtr = config_ptr;
        seqsPtr = seqs_ptr;
        ticksPtr = ticks_ptr;
}

void YASS_EEPROM::saveSequence(byte seq_num)
{
    write(seqsPtr[seq_num].getDataPointer(), wpeek(seqAddr + seq_num), YASS_SEQUENCE_DATA_SIZE);
}

void YASS_EEPROM::loadSequence(byte seq_num)
{
    read(wpeek(seqAddr + seq_num), seqsPtr[seq_num].getDataPointer(), YASS_SEQUENCE_DATA_SIZE);
}

void YASS_EEPROM::saveAll()
{
    write(configPtr->getDataPointer(), YASS_EEPROM_BASE_GLOBAL, YASS_CONFIG_DATA_SIZE);
    
    // tempo
    write(ticksPtr->getDataPointer(), YASS_EEPROM_BASE_TEMPO, sizeof(word));
    
    // sequences
    for(byte seq_num = 0; seq_num < NB_SEQS; seq_num++)
        saveSequence(seq_num);
}

void YASS_EEPROM::loadAll()
{
    // global configuration
    read(YASS_EEPROM_BASE_GLOBAL, configPtr->getDataPointer(), YASS_CONFIG_DATA_SIZE);
    
    // tempo ...
    read(YASS_EEPROM_BASE_TEMPO, ticksPtr->getDataPointer(), sizeof(word));
    // ... needs to be recomputed, this is triggered by setTempo()
    ticksPtr->setTempo(ticksPtr->getTempo());
    
    // sequences
    for(byte seq_num = 0; seq_num < NB_SEQS; seq_num++)
        loadSequence(seq_num);
}

