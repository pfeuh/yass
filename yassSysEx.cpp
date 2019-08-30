/*
 * file : yassSysEx.cpp
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

#include "yassSysEx.h"

// TODO: this constant it everywere, should make it global
#define NB_SEQS 5

#define START_OF_SYS_EX 0xf0
#define END_OF_SYS_EX   0xf7

#define MANUFACTURER_BYTE_1 'y'
#define MANUFACTURER_BYTE_2 'a'
#define MANUFACTURER_BYTE_3 's'

#define YASS_DEBUG
#ifdef YASS_DEBUG
#include <arduinoDebug.h>
ARDUINO_DEBUG debug = ARDUINO_DEBUG();
#endif

/*******************/
/* Private methods */
/*******************/

void YASS_SYS_EX::push(byte value)
{
    *(buffer + (bufferIndex++)) = value;
}

byte YASS_SYS_EX::pop()
{
    return *inputPtr++;
}

void YASS_SYS_EX::openSysExMessage()
{
    bufferIndex = 0;
    push(START_OF_SYS_EX);
    push(MANUFACTURER_BYTE_1);
    push(MANUFACTURER_BYTE_2);
    push(MANUFACTURER_BYTE_3);
}

void YASS_SYS_EX::closeAndSendSysExMessage()
{
    push(END_OF_SYS_EX);
    senderCallback(buffer, bufferIndex);
}

/******************/
/* Public methods */
/******************/

YASS_SYS_EX::YASS_SYS_EX()
{
}

void YASS_SYS_EX::begin(YASS_CONFIG* config_ptr, YASS_SEQUENCE* seqs_ptr)
{
        configPtr = config_ptr;
        seqsPtr = seqs_ptr;
        senderCallback = NULL;
}

bool YASS_SYS_EX::isMessageForUs()
{
    // don't ask twice !!!
    pop(); // first byte is 0xf0, it has to be popped
    return (pop() == MANUFACTURER_BYTE_1) &&\
           (pop() == MANUFACTURER_BYTE_2) &&\
           (pop() == MANUFACTURER_BYTE_3);
}

void YASS_SYS_EX::executeSysEx(byte * array_ptr, word array_size)
{
    inputPtr = array_ptr;
    inputSize = array_size;
    
    if(isMessageForUs())
    {
        byte command = pop();
        //~ debug.begin();
        //~ console.print(F("command is "));
        //~ console.println(command);
        //~ console.print(F("\n\n"));
        
        switch(command)
        {
            case YASS_SYS_EX_GLOB_DUMP_REQUEST:
                sendError(YASS_SYS_EX_ERR_NOT_IMPLEMENTED);
                break;
            case YASS_SYS_EX_SEQ_DUMP_REQUEST:
                sendSequence(pop());
                break;
            default:
                sendError(YASS_SYS_EX_ERR_UNKNOWN);
                break;
        }
    }
}

void YASS_SYS_EX::setSenderCallback(void (*callback)(byte* addr, word size))
{
    senderCallback = callback;
}

void YASS_SYS_EX::sendSequence(byte seq_num)
{
    if(senderCallback)
    {
        if(seq_num < NB_SEQS)
        {
            openSysExMessage();
            
            push(YASS_SYS_EX_SEQ_DUMP);  
            push(seq_num);
            
            byte* data_ptr = seqsPtr[seq_num].getDataPointer();
            for(byte x = 0; x < YASS_SEQUENCE_DATA_SIZE; x++)
                push(*data_ptr++);
            
            closeAndSendSysExMessage();
        }
        else
            sendError(YASS_SYS_EX_ERR_BAD_PARAMETER);
    }
}

void YASS_SYS_EX::sendError(byte err_num)
{
    if(senderCallback)
    {
        openSysExMessage();
        
        push(YASS_SYS_EX_ERROR);  
        push(err_num);
        
        closeAndSendSysExMessage();
    }
}

