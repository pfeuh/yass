/*
 * file : yassSequence.cpp
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

#include "yassSequence.h"
#include "yassSequencer.h"
#include "yassConfig.h"

#define YASS_SEQUENCE_INDEX_NOTES         0
#define YASS_SEQUENCE_INDEX_DATA          YASS_SEQUENCE_NB_NOTES
#define YASS_SEQUENCE_INDEX_GROOVE        (YASS_SEQUENCE_NB_NOTES * 2)
#define YASS_SEQUENCE_INDEX_GATEMODE      (YASS_SEQUENCE_INDEX_GROOVE + 1)
#define YASS_SEQUENCE_INDEX_LASTSTEP      (YASS_SEQUENCE_INDEX_GROOVE + 2)
#define YASS_SEQUENCE_INDEX_DATAMODE      (YASS_SEQUENCE_INDEX_GROOVE + 3)
#define YASS_SEQUENCE_INDEX_CONTROLCHANGE (YASS_SEQUENCE_INDEX_GROOVE + 4)
#define YASS_SEQUENCE_INDEX_FIXEDVELOCITY (YASS_SEQUENCE_INDEX_GROOVE + 5)

#define groove seqData[YASS_SEQUENCE_INDEX_GROOVE]
#define gateMode seqData[YASS_SEQUENCE_INDEX_GATEMODE]
#define lastStep seqData[YASS_SEQUENCE_INDEX_LASTSTEP]
#define dataMode seqData[YASS_SEQUENCE_INDEX_DATAMODE]
#define controlChange seqData[YASS_SEQUENCE_INDEX_CONTROLCHANGE]
#define fixedVelocity seqData[YASS_SEQUENCE_INDEX_FIXEDVELOCITY]

#define YASS_SEQUENCE_DEFAULT_NOTE      64
#define YASS_SEQUENCE_DEFAULT_DATA      100
#define YASS_SEQUENCE_DEFAULT_GROOVE    YASS_SEQUENCE_DOUBLE_CROCHE
#define YASS_SEQUENCE_DEFAULT_GATE_MODE YASS_SEQUENCE_GATE_NORMAL
#define YASS_SEQUENCE_DEFAULT_DATA_MODE YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY
#define YASS_SEQUENCE_DEFAULT_CC        0
#define YASS_SEQUENCE_DEFAULT_FIX_VEL   100

/*******************/
/* Private methods */
/*******************/

/******************/
/* Public methods */
/******************/

YASS_SEQUENCE::YASS_SEQUENCE()
{
    initialize(YASS_SEQUENCE_DEFAULT_NOTE);
}

void YASS_SEQUENCE::initialize(byte default_note)
{
    groove = YASS_SEQUENCE_DEFAULT_GROOVE;
    gateMode = YASS_SEQUENCE_DEFAULT_GATE_MODE;
    lastStep = YASS_SEQUENCE_NB_NOTES - 1;
    dataMode = YASS_SEQUENCE_DEFAULT_DATA_MODE;
    controlChange = YASS_SEQUENCE_DEFAULT_CC;
    fixedVelocity = YASS_SEQUENCE_DEFAULT_FIX_VEL;

    for(byte x = 0; x < YASS_SEQUENCE_NB_NOTES; x++)
    {
        setNote(x, default_note);
        setData(x, YASS_SEQUENCE_DEFAULT_DATA);
    }
}

byte YASS_SEQUENCE::getNote(byte index)
{
    return seqData[index + YASS_SEQUENCE_INDEX_NOTES] & 0x7f;
}

void YASS_SEQUENCE::setNote(byte index, byte note)
{
    seqData[index + YASS_SEQUENCE_INDEX_NOTES] = note & 0x7f;
}

byte YASS_SEQUENCE::getData(byte index)
{
    return seqData[index + YASS_SEQUENCE_INDEX_DATA] & 0x7f;
}

void YASS_SEQUENCE::setData(byte index, byte _data)
{
    seqData[index + YASS_SEQUENCE_INDEX_DATA] = _data & 0x7f;
}

void YASS_SEQUENCE::setGateMode(byte mode)
{
    gateMode = mode;
}

byte YASS_SEQUENCE::getGateMode()
{
    return gateMode;
}

void YASS_SEQUENCE::setDataMode(byte mode)
{
    dataMode = mode;
}

byte YASS_SEQUENCE::getDataMode()
{
    return dataMode;
}

void YASS_SEQUENCE::setGroove(byte _groove)
{
    groove = _groove;
}

byte YASS_SEQUENCE::getGroove()
{
    return groove;
}

void YASS_SEQUENCE::setFixedVelocity(byte velocity)
{
    fixedVelocity = velocity;
}

byte YASS_SEQUENCE::getFixedVelocity()
{
    return fixedVelocity;
}

void YASS_SEQUENCE::setLastStep(byte last_step)
{
    lastStep = last_step;
}

byte YASS_SEQUENCE::getLastStep()
{
    return lastStep;
}

void YASS_SEQUENCE::setControlChange(byte cc_num)
{
    controlChange = cc_num;
}

byte YASS_SEQUENCE::getControlChange()
{
    return controlChange;
}

byte YASS_SEQUENCE::getFirstNote()
{
    return seqData[YASS_SEQUENCE_INDEX_NOTES];
}

byte* YASS_SEQUENCE::getDataPointer()
{
    return seqData;
}
