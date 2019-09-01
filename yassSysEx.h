#ifndef yassSysEx_h
#define yassSysEx_h

/*
 * file : yassSysEx.h
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
#include <MIDI.h>

// to get the objects to dump
#include "yassSequencer.h"
#include "yassConfig.h"

#define YASS_SYS_EX_VERSION "1.00"

#define YASS_SYS_EX_BUFFER_SIZE 64

#define YASS_SYS_EX_GLOB_DUMP_REQUEST 1
#define YASS_SYS_EX_SEQ_DUMP_REQUEST  2
#define YASS_SYS_EX_GLOB_DUMP         65
#define YASS_SYS_EX_SEQ_DUMP          66

#define YASS_SYS_EX_ACKNOWLEDGE       126
#define YASS_SYS_EX_ERROR             127

#define YASS_SYS_EX_ERR_UNKNOWN         1
#define YASS_SYS_EX_ERR_NOT_IMPLEMENTED 2
#define YASS_SYS_EX_ERR_BAD_PARAMETER   3

class YASS_SYS_EX
{
    public:
        YASS_SYS_EX();
        void begin(YASS_CONFIG* config_ptr, YASS_SEQUENCE* seqs_ptr);
        bool isMessageForUs();
        void executeSysEx(byte * array_ptr, word array_size);
        void setSenderCallback(void (*callback)(byte* addr, word size));
        void sendSequence(byte seq_num);
        void sendGlobal();
        void sendAcknowledge();
        void sendError(byte err_num);
        void parseSequence(byte seq_num);
        void parseGlobal();

    private:
        YASS_CONFIG* configPtr;
        YASS_SEQUENCE* seqsPtr;
        byte buffer[YASS_SYS_EX_BUFFER_SIZE];
        word bufferIndex;
        byte* inputPtr;
        word inputSize;
        void (*senderCallback)(byte* addr, word size);
        byte state;
    
        void push(byte value);
        byte pop();
        void openSysExMessage();
        void closeAndSendSysExMessage();

};

#endif

