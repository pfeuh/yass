#ifndef yassMaintenance_h
#define yassMaintenance_h

/*
 * file : yassMaintenance.h
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
/* ok to use a huge quantity of constants */
#include <avr/pgmspace.h>
/* a finite state machine for editing each parameter */
#include "yassConfFsm.h"
/* HMI parts */
#include "yassKeyb.h"
#include "yassBeeper.h"
#include "yassMonostable.h"
#include "yassGpo.h"
#include "yassLeds.h"
#include "yassDisplay.h"
/* Sequencer parts */
#include "yassTicks.h"
#include "yassSequencer.h"
#include "yassRomSequences.h"
#include "yassSequence.h"
#include "yassConfig.h"
/* MIDI driver */
#include <MIDI.h>
#include "yassSysEx.h"
/* storage driver */
#include "yassEeprom.h"
/* miscellaneous */
#include "yassComputeBeat.h"
#include <arduinoDebug.h>

#define YASS_VERSION "1.00"

// must be a static function, not a method
void YASS_MAINTENANCE_updateData(bool direction);

class YASS_MAINTENANCE
{
    public:
        YASS_MAINTENANCE();
        void begin(
            YASS_MONOSTABLE*,
            YASS_CONF_FSM*,
            YASS_KEYB*,
            YASS_BEEPER*,
            YASS_GPO*,
            YASS_CONFIG*,
            YASS_SEQUENCER*,
            YASS_TICKS*,
            YASS_DISPLAY*,
            YASS_LEDS*,
            YASS_COMPUTE_BEAT*,
            YASS_EEPROM*,
            YASS_SYS_EX*,
            ARDUINO_DEBUG*);
        void sequencer();

    private:
        byte state;
        byte gpv1; // general purpose variables
        byte gpv2;
        unsigned long int milestone;
        int userValue;
    
        YASS_MONOSTABLE* dotInMonostable;
        YASS_CONF_FSM* editor;
        YASS_KEYB* keyb;
        YASS_BEEPER* beeper;
        YASS_GPO* shifter;
        YASS_CONFIG* globConf;
        YASS_SEQUENCER* player;
        YASS_TICKS* ticks;
        YASS_DISPLAY* display;
        YASS_LEDS* leds;
        YASS_COMPUTE_BEAT* beatCalc;
        YASS_EEPROM* storage;
        YASS_SYS_EX* sysEx;
        ARDUINO_DEBUG* debug;
    
        void displayVersions();
        void resetOutputs();
        void prompt();
        void setState(byte _state);
        bool itsTime();
        void stateStepLeds();
        void stateIndividualLeds();
        void stateDisplay();
        void stateBeeper();
        void romPrint(const char* addr);
        void keyMessage(const char* addr);
        void stateKeyboard();
        void stateEncoder();
        void stateEeprom();
    
};

#endif

