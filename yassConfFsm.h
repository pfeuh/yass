#ifndef yassConfFsm_h
#define yassConfFsm_h

/*
 * file : yassConfFsm.h
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
#include "yassEncoder.h"

#define YASS_CONF_FSM_VERSION "1.00"

/*********************/
/* states definition */
/*********************/

#define YASS_CONF_FSM_STATE_TEMPO 1
#define YASS_CONF_FSM_STATE_RECORD_EDIT 2

// global parameters
#define YASS_CONF_FSM_FIRST_GLOBAL 16
#define YASS_CONF_FSM_MIDI_IN_CHANNEL YASS_CONF_FSM_FIRST_GLOBAL
#define YASS_CONF_FSM_MIDI_OUT_CHANNEL 17
#define YASS_CONF_FSM_CLOCK_IN 18
#define YASS_CONF_FSM_CLOCK_OUT 19
#define YASS_CONF_FSM_KEY_ECHO 20
#define YASS_CONF_FSM_CLICK 21
#define YASS_CONF_FSM_ARPEGGIATOR 22
#define YASS_CONF_FSM_SPLIT_POINT 23 // should be deleted
#define YASS_CONF_FSM_PROGRAM_NUMBER 24
#define YASS_CONF_FSM_USE_SYSEX 25
#define YASS_CONF_FSM_GLOB_DUMP 29
#define YASS_CONF_FSM_GLOB_LOAD 30
#define YASS_CONF_FSM_GLOB_SAVE 31
#define YASS_CONF_FSM_LAST_GLOBAL YASS_CONF_FSM_GLOB_SAVE

// sequence parameters
#define YASS_CONF_FSM_FIRST_SEQ 32
#define YASS_CONF_FSM_GROOVE YASS_CONF_FSM_FIRST_SEQ
#define YASS_CONF_FSM_GATE_MODE 33
#define YASS_CONF_FSM_LAST_STEP 34
#define YASS_CONF_FSM_DATA_MODE 35
#define YASS_CONF_FSM_CTRL_CHG 36
#define YASS_CONF_FSM_FIX_VEL  37
#define YASS_CONF_FSM_SEQ_DUMP 42
#define YASS_CONF_FSM_COPY 43
#define YASS_CONF_FSM_SWAP 44
#define YASS_CONF_FSM_SEQ_LOAD_FACTORY 45
#define YASS_CONF_FSM_SEQ_LOAD 46
#define YASS_CONF_FSM_SEQ_SAVE 47
#define YASS_CONF_FSM_LAST_SEQ YASS_CONF_FSM_SEQ_SAVE

class YASS_CONF_FSM
{
    public:
        YASS_CONF_FSM();
        void begin(int pin_a, int pin_b);
        void setState(byte state_num, void(*change_data_hook)(bool direction));
        byte getState();
        void sequencer();

    private:
        byte state;
        YASS_ENCODER encoder = YASS_ENCODER();

};

#endif

