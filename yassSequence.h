#ifndef yassSequence_h
#define yassSequence_h

/*
 * file : yassSequence.h
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

#define YASS_SEQUENCE_VERSION "1.00"

#define YASS_SEQUENCE_NOIRE 0
#define YASS_SEQUENCE_CROCHE 1
#define YASS_SEQUENCE_TRIOLET_CROCHE 2
#define YASS_SEQUENCE_CROCHE_SWING 3
#define YASS_SEQUENCE_DOUBLE_CROCHE 4
#define YASS_SEQUENCE_TRIOLET_DOUBLE_CROCHE 5
#define YASS_SEQUENCE_DOUBLE_CROCHE_SWING 6
#define YASS_SEQUENCE_LAST_GROOVE YASS_SEQUENCE_DOUBLE_CROCHE_SWING
#define YASS_SEQUENCE_GROOVE_MASK 7

#define YASS_SEQUENCE_GATE_SHORT  0
#define YASS_SEQUENCE_GATE_NORMAL 1
#define YASS_SEQUENCE_GATE_LONG   2
#define YASS_SEQUENCE_GATE_LAST YASS_SEQUENCE_GATE_LONG

#define YASS_SEQUENCE_DATA_MODE_VELOCITY  0
#define YASS_SEQUENCE_DATA_MODE_CTRL_CHG 1
#define YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY 2
#define YASS_SEQUENCE_DATA_MODE_LAST YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY

#define YASS_SEQUENCE_DATA_MODE_VELOCITY 0
#define YASS_SEQUENCE_DATA_MODE_CC 1
#define YASS_SEQUENCE_DATA_MODE_NONE 2
#define YASS_SEQUENCE_DATA_MODE_MAX YASS_SEQUENCE_DATA_MODE_NONE

#define YASS_SEQUENCE_REST 0
#define YASS_SEQUENCE_TIE  128

#define YASS_SEQUENCE_NB_NOTES 16

#define YASS_SEQUENCE_NB_INDIVIDUAL_BYTES 6
#define YASS_SEQUENCE_NB_TABLE_BYTES (2 * YASS_SEQUENCE_NB_NOTES)
#define YASS_SEQUENCE_DATA_SIZE (YASS_SEQUENCE_NB_TABLE_BYTES + YASS_SEQUENCE_NB_INDIVIDUAL_BYTES)

class YASS_SEQUENCE
{
    public:
        YASS_SEQUENCE();
        void initialize();
        byte getFirstNote();

        byte getNote(byte index);
        void setNote(byte index, byte note);
        byte getData(byte index);
        void setData(byte index, byte _data);
    
        void setGroove(byte _groove);
        byte getGroove();
        void setLastStep(byte last_step);
        byte getLastStep();
        void setGateMode(byte mode);
        byte getGateMode();
        byte getDataMode();
        void setDataMode(byte mode);
        void setControlChange(byte cc_num);
        byte getControlChange();
        void setFixedVelocity(byte velocity);
        byte getFixedVelocity();
        byte* getDataPointer();

    private:
        //this bytes should be saved/loaded as parts of the sequence
        byte seqData[YASS_SEQUENCE_DATA_SIZE];

};

#endif
