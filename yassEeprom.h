#ifndef yassEeprom_h
#define yassEeprom_h

/*
 * file : yassEeprom.h
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
#include "yassConfig.h"
#include "yassSequence.h"
#include "yassTicks.h"

#define YASS_EEPROM_VERSION "1.00"

#define YASS_EEPROM_BASE_GLOBAL 0
#define YASS_EEPROM_BASE_TEMPO (YASS_EEPROM_BASE_GLOBAL + YASS_CONFIG_DATA_SIZE)
#define YASS_EEPROM_BASE_SEQ1 (YASS_EEPROM_BASE_TEMPO + sizeof(word))
#define YASS_EEPROM_BASE_SEQ2 (YASS_EEPROM_BASE_SEQ1 + YASS_SEQUENCE_DATA_SIZE)
#define YASS_EEPROM_BASE_SEQ3 (YASS_EEPROM_BASE_SEQ2 + YASS_SEQUENCE_DATA_SIZE)
#define YASS_EEPROM_BASE_SEQ4 (YASS_EEPROM_BASE_SEQ3 + YASS_SEQUENCE_DATA_SIZE)
#define YASS_EEPROM_BASE_SEQ5 (YASS_EEPROM_BASE_SEQ4 + YASS_SEQUENCE_DATA_SIZE)

#define NB_SEQS 5

class YASS_EEPROM
{
    public:
        YASS_EEPROM();
        void begin(YASS_CONFIG* config_ptr, YASS_SEQUENCE* seqs_ptr, YASS_TICKS* ticks_ptr);
        void saveSequence(byte seq_num);
        void loadSequence(byte seq_num);
        void saveAll();
        void loadAll();
    
    private:
        byte* buffer;
        YASS_CONFIG* configPtr;
        YASS_SEQUENCE* seqsPtr;
        YASS_TICKS* ticksPtr;

        void read(word base, byte* target, word nb_bytes);
        void write(byte* source, word base, word nb_bytes);
    
};


#endif

