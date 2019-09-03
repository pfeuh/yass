/*
 * file : yassConfig.cpp
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

#include "yassConfig.h"

/* bit 0 */ #define YASS_CONFIG_BIT_WEIGHT_OMNI              0x001
/* bit 1 */ #define YASS_CONFIG_BIT_WEIGHT_CLOCK_IN          0x002
/* bit 2 */ #define YASS_CONFIG_BIT_WEIGHT_CLOCK_OUT         0x004
/* bit 3 */ #define YASS_CONFIG_BIT_WEIGHT_KEY_ECHO          0x008
/* bit 4 */ #define YASS_CONFIG_BIT_WEIGHT_CLICK             0x010
/* bit 5 */ #define YASS_CONFIG_BIT_WEIGHT_ARPEGGIATOR       0x020
/* bit 6 */ #define YASS_CONFIG_BIT_WEIGHT_USE_PROG_NUM      0x040
/* bit 7    should not exist, not compatible with 7 bits midi data format */
/* bit 8 */ #define YASS_CONFIG_BIT_WEIGHT_USE_SYSEX         0x100

#define YASS_CONFIG_INDEX_BITS_HI            0
#define YASS_CONFIG_INDEX_BITS_LO            1
#define YASS_CONFIG_INDEX_CHANNEL_IN         2
#define YASS_CONFIG_INDEX_CHANNEL_OUT        3
#define YASS_CONFIG_INDEX_PROGRAM_NUMBER     4

#define bitsHi        data[YASS_CONFIG_INDEX_BITS_HI]
#define bitsLo        data[YASS_CONFIG_INDEX_BITS_LO]
#define channelIn     data[YASS_CONFIG_INDEX_CHANNEL_IN]
#define channelOut    data[YASS_CONFIG_INDEX_CHANNEL_OUT]
#define programNumber data[YASS_CONFIG_INDEX_PROGRAM_NUMBER]

/*******************/
/* Private methods */
/*******************/

inline void YASS_CONFIG::setBit(word weight, bool flag)
{
    word bits = bitsHi << 8 | bitsLo;
    bits &= weight ^ 0x1ff;
    if(flag)
        bits |= weight;
    bitsHi = bits >> 8;
    bitsLo = bits & 0xff;
}

inline bool YASS_CONFIG::getBit(word weight)
{
    word bits = bitsHi << 8 | bitsLo;
    return (bits & weight) == weight;
}

/******************/
/* Public methods */
/******************/

YASS_CONFIG::YASS_CONFIG()
{
    setChannelIn(YASS_CONFIG_DEFAULT_CHANNEL_IN);
    setChannelOut(YASS_CONFIG_DEFAULT_CHANNEL_OUT);
    setOmni(YASS_CONFIG_DEFAULT_OMNI);
    setClockIn(YASS_CONFIG_DEFAULT_CLOCK_IN);
    setClockOut(YASS_CONFIG_DEFAULT_CLOCK_OUT);
    setClick(YASS_CONFIG_DEFAULT_CLICK);
    setArpeggiator(YASS_CONFIG_DEFAULT_ARPEGGIATOR);
    setKeyEcho(YASS_CONFIG_DEFAULT_KEY_ECHO);
    setProgNum(YASS_CONFIG_DEFAULT_PROG_NUM);
    setUseProgNum(YASS_CONFIG_DEFAULT_USE_PROGNUM);
}

void YASS_CONFIG::begin()
{
    
}

void YASS_CONFIG::setProgNum(byte prog_num)
{
    programNumber = prog_num & 0x7f;
}

byte YASS_CONFIG::getProgNum()
{
    return programNumber & 0x7f;
}

void YASS_CONFIG::setUseProgNum(bool flag)
{
    setBit(YASS_CONFIG_BIT_WEIGHT_USE_PROG_NUM, flag);
}

bool YASS_CONFIG::getUseProgNum()
{
    return getBit(YASS_CONFIG_BIT_WEIGHT_USE_PROG_NUM);
}

byte YASS_CONFIG::getChannelIn()
{
    return channelIn;
}

void YASS_CONFIG::setChannelIn(byte channel)
{
    if((YASS_CONFIG_MIN_CHANNEL <= channel) && (YASS_CONFIG_MAX_CHANNEL >= channel))
        channelIn = channel;
}

bool YASS_CONFIG::getOmni()
{
    return getBit(YASS_CONFIG_BIT_WEIGHT_OMNI);
}

void YASS_CONFIG::setOmni(bool flag)
{
    setBit(YASS_CONFIG_BIT_WEIGHT_OMNI, flag);
}

byte YASS_CONFIG::getChannelOut()
{
    return channelOut;
}

void YASS_CONFIG::setChannelOut(byte channel)
{
    if((YASS_CONFIG_MIN_CHANNEL <= channel) && (YASS_CONFIG_MAX_CHANNEL >= channel))
        channelOut = channel;
}

void YASS_CONFIG::setClockIn(bool flag)
{
    setBit(YASS_CONFIG_BIT_WEIGHT_CLOCK_IN, flag);
}

bool YASS_CONFIG::getClockIn()
{
    return getBit(YASS_CONFIG_BIT_WEIGHT_CLOCK_IN);
}

void YASS_CONFIG::setClockOut(bool flag)
{
    setBit(YASS_CONFIG_BIT_WEIGHT_CLOCK_OUT, flag);
}

bool YASS_CONFIG::getClockOut()
{
    return getBit(YASS_CONFIG_BIT_WEIGHT_CLOCK_OUT);
}

void YASS_CONFIG::setClick(bool flag)
{
    setBit(YASS_CONFIG_BIT_WEIGHT_CLICK, flag);
}

bool YASS_CONFIG::getClick()
{
    return getBit(YASS_CONFIG_BIT_WEIGHT_CLICK);
}

void YASS_CONFIG::setKeyEcho(bool flag)
{
    setBit(YASS_CONFIG_BIT_WEIGHT_KEY_ECHO, flag);
}

bool YASS_CONFIG::getKeyEcho()
{
    return getBit(YASS_CONFIG_BIT_WEIGHT_KEY_ECHO);
}

void YASS_CONFIG::setArpeggiator(bool flag)
{
    setBit(YASS_CONFIG_BIT_WEIGHT_ARPEGGIATOR, flag);
}

bool YASS_CONFIG::getArpeggiator()
{
    return getBit(YASS_CONFIG_BIT_WEIGHT_ARPEGGIATOR);
}

void YASS_CONFIG::setUseSysEx(bool flag)
{
    setBit(YASS_CONFIG_BIT_WEIGHT_USE_SYSEX, flag);
}

bool YASS_CONFIG::getUseSysEx()
{
    return getBit(YASS_CONFIG_BIT_WEIGHT_USE_SYSEX);
}

byte* YASS_CONFIG::getDataPointer()
{
    return data;
}

