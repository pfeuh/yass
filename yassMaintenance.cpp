/*
 * file : yassMaintenance.cpp
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

#include "yassMaintenance.h"

#define YASS_MAINTENANCE_STATE_READY     1
#define YASS_MAINTENANCE_STATE_STEP_LEDS 2
#define YASS_MAINTENANCE_STATE_INDV_LEDS 3
#define YASS_MAINTENANCE_STATE_DISPLAY   4
#define YASS_MAINTENANCE_STATE_KEYBOARD  5
#define YASS_MAINTENANCE_STATE_BEEPER    6
#define YASS_MAINTENANCE_STATE_ENCODER   7
#define YASS_MAINTENANCE_STATE_EEPROM    8

#define ledNum gpv1
#define ledDirection gpv2
#define YASS_MAINTENANCE_LED_MSEC_DURATION 150

#define segmentWeight gpv1
#define digitNum gpv2
#define YASS_MAINTENANCE_DISPLAY_MSEC_DURATION 300

#define YASS_MAINTENANCE_ENCODER_MSEC_DURATION 1000

const byte sortedLeds[] PROGMEM =
{
    LED_START,
    LED_RECORD,
    LED_1,
    LED_2,
    LED_3,
    LED_4,
    LED_5,
    LED_SEQUENCE,
    LED_GLOBAL,
    LED_MULTI_USAGE,
    LED_TIE,
    LED_REST
};

const bool beeperTestStates[] PROGMEM = {1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0};
#define YASS_MAINTENANCE_NB_BEEPER_TEST_STATES (sizeof(beeperTestStates) / sizeof(bool))
#define YASS_MAINTENANCE_BEEPER_MSEC_DURATION 150
#define beeperIndex gpv1

const char msgKey[] PROGMEM = "key ";
const char msgPressed[] PROGMEM = " pressed.\n";

const char YASS_MAINTENANCE_kbd_1[] PROGMEM = "SEQ1";
const char YASS_MAINTENANCE_kbd_2[] PROGMEM = "SEQ2";
const char YASS_MAINTENANCE_kbd_3[] PROGMEM = "SEQ3";
const char YASS_MAINTENANCE_kbd_4[] PROGMEM = "SEQ4";
const char YASS_MAINTENANCE_kbd_5[] PROGMEM = "SEQ5";
const char YASS_MAINTENANCE_kbd_next[] PROGMEM = "NEXT";
const char YASS_MAINTENANCE_kbd_start[] PROGMEM = "PLAY";
const char YASS_MAINTENANCE_kbd_pause[] PROGMEM = "STOP";
const char YASS_MAINTENANCE_kbd_continue[] PROGMEM = "CONT.";
const char YASS_MAINTENANCE_kbd_record[] PROGMEM = "REC.";
const char YASS_MAINTENANCE_kbd_global[] PROGMEM = "GLOB.";
const char YASS_MAINTENANCE_kbd_sequence[] PROGMEM = "SEQ.";
const char YASS_MAINTENANCE_kbd_rest[] PROGMEM = "REST";
const char YASS_MAINTENANCE_kbd_tie[] PROGMEM = "TIE";
const char YASS_MAINTENANCE_kbd_multi_usage[] PROGMEM = "DATA";

int YASS_MAINTENANCE_encoderData;

void YASS_MAINTENANCE_updateData(bool direction)
{
    if(direction)
        YASS_MAINTENANCE_encoderData++;
    else
        YASS_MAINTENANCE_encoderData--;
}

/*******************/
/* Private methods */
/*******************/

void YASS_MAINTENANCE::romPrint(const char* addr)
{
    while(pgm_read_byte(addr))
        console.write(pgm_read_byte(addr++));
}

void YASS_MAINTENANCE::setState(byte _state)
{
    state = _state;
    resetOutputs();
    gpv1 = 0;
    gpv2 = 0;
    milestone = millis();
}

bool inline YASS_MAINTENANCE::itsTime()
{
    return millis() >= milestone;
}

void YASS_MAINTENANCE::displayVersions()
{
    console.println(F("Yass             " YASS_VERSION));
    console.println(F("Sequencer        " YASS_SEQUENCER_VERSION));
    console.println(F("Sequences        " YASS_SEQUENCE_VERSION));
    console.println(F("Global config.   " YASS_CONFIG_VERSION));
    console.println(F("Keyboard         " YASS_KEYB_VERSION));
    console.println(F("Beeper           " YASS_BEEPER_VERSION));
    console.println(F("Monostable       " YASS_MONOSTABLE_VERSION));
    console.println(F("GP outputs       " YASS_GPO_VERSION));
    console.println(F("Leds driver      " YASS_LEDS_VERSION));
    console.println(F("Display driver   " YASS_DISPLAY_VERSION));
    console.println(F("Ticks generator  " YASS_TICKS_VERSION));
    console.println(F("ROM seqs         " YASS_ROM_SEQUENCES_VERSION));
    console.println(F("sysEx driver     " YASS_SYS_EX_VERSION));
    console.println(F("Eeprom driver    " YASS_EEPROM_VERSION));
    console.println(F("Tempo calculator " YASS_COMPUTE_BEAT_VERSION));
    console.println(F("Arduino debug    " ARDUINO_DEBUG_VERSION "\n"));
}

void YASS_MAINTENANCE::prompt()
{
    console.println(F("\n? invoke this menu"));
    console.println(F("1 test step leds"));
    console.println(F("2 test individual leds"));
    console.println(F("3 test display"));
    console.println(F("4 test keyboard"));
    console.println(F("5 test beeper"));
    console.println(F("6 test encoder"));
    console.println(F("7 dump eeprom"));
    console.println(F("s stop test"));
    console.println(F("v display versions"));
}

void YASS_MAINTENANCE::resetOutputs()
{
    display->clear();
    display->writeDot(0, 0);
    display->writeDot(0, 1);
    display->writeDot(0, 2);
    display->writeDot(0, 3);
    leds->clearLeds();
}

void YASS_MAINTENANCE::stateStepLeds()
{
    if(itsTime())
    {
        milestone += YASS_MAINTENANCE_LED_MSEC_DURATION;
        leds->clearLeds();
        leds->stepLedOn(ledNum);
        if(ledDirection == 0)
        {
            ledNum++;
            if(ledNum == YASS_LEDS_NB_STEP_LEDS)
            {
                ledNum -= 2;
                ledDirection = 1;
            }
        }
        else
        {
            ledNum--;
            if(ledNum == 255)
            {
                ledNum += 2;
                ledDirection = 0;
            }
        }
    }
}

void YASS_MAINTENANCE::stateIndividualLeds()
{
    if(itsTime())
    {
        milestone += YASS_MAINTENANCE_LED_MSEC_DURATION;
        leds->clearLeds();
        leds->ledOn(pgm_read_byte(sortedLeds + ledNum));
        if(ledDirection == 0)
        {
            ledNum++;
            if(ledNum == YASS_LEDS_NB_INDIVIDUAL_LEDS)
            {
                ledNum -= 2;
                ledDirection = 1;
            }
        }
        else
        {
            ledNum--;
            if(ledNum == 255)
            {
                ledNum += 2;
                ledDirection = 0;
            }
        }
    }
}

void YASS_MAINTENANCE::stateDisplay()
{
    if(itsTime())
    {
        milestone += YASS_MAINTENANCE_DISPLAY_MSEC_DURATION;
        byte* digit_ptr = display->getDigitsPtr();
        //~ display->clear();
        resetOutputs();
        *(digit_ptr + digitNum) = segmentWeight;
        digitNum += 1;
        if(digitNum == display->getNbDigits())
        {
            digitNum = 0;
            segmentWeight <<= 1;
            if(!segmentWeight)
                segmentWeight = 1;
            }
        }
}

void YASS_MAINTENANCE::stateBeeper()
{
    if(itsTime())
    {
        milestone += YASS_MAINTENANCE_BEEPER_MSEC_DURATION;
        if(pgm_read_byte(beeperTestStates + beeperIndex))
            beeper->beep();
        beeperIndex += 1;
        if(beeperIndex == YASS_MAINTENANCE_NB_BEEPER_TEST_STATES)
            beeperIndex = 0;
    }
}

void YASS_MAINTENANCE::keyMessage(const char* msg)
{
    romPrint(msgKey);
    romPrint(msg);
    romPrint(msgPressed);
}

void YASS_MAINTENANCE::stateKeyboard()
{
    if(keyb->available())
        switch(keyb->read())
        {
            case KBD_1:
                keyMessage(YASS_MAINTENANCE_kbd_1);
                break;
            case KBD_2:
                keyMessage(YASS_MAINTENANCE_kbd_2);
                break;
            case KBD_3:
                keyMessage(YASS_MAINTENANCE_kbd_3);
                break;
            case KBD_4:
                keyMessage(YASS_MAINTENANCE_kbd_4);
                break;
            case KBD_5:
                keyMessage(YASS_MAINTENANCE_kbd_5);
                break;
            case KBD_NEXT:
                keyMessage(YASS_MAINTENANCE_kbd_next);
                break;
            case KBD_START:
                keyMessage(YASS_MAINTENANCE_kbd_start);
                break;
            case KBD_PAUSE:
                keyMessage(YASS_MAINTENANCE_kbd_pause);
                break;
            case KBD_CONTINUE:
                keyMessage(YASS_MAINTENANCE_kbd_continue);
                break;
            case KBD_RECORD:
                keyMessage(YASS_MAINTENANCE_kbd_record);
                break;
            case KBD_GLOBAL:
                keyMessage(YASS_MAINTENANCE_kbd_global);
                break;
            case KBD_SEQUENCE:
                keyMessage(YASS_MAINTENANCE_kbd_sequence);
                break;
            case KBD_REST:
                keyMessage(YASS_MAINTENANCE_kbd_rest);
                break;
            case KBD_TIE:
                keyMessage(YASS_MAINTENANCE_kbd_tie);
                break;
            case KBD_MULTI_USAGE:
                keyMessage(YASS_MAINTENANCE_kbd_multi_usage);
                break;
            default:
                break;
        }
}

void YASS_MAINTENANCE::stateEncoder()
{
    if(itsTime())
    {
        milestone += YASS_MAINTENANCE_ENCODER_MSEC_DURATION;
        console.println(YASS_MAINTENANCE_encoderData);
    }
}

void YASS_MAINTENANCE::stateEeprom()
{
    console.println(F("Global configuration"));
    debug->dumpEeprom(YASS_EEPROM_BASE_GLOBAL, YASS_CONFIG_DATA_SIZE);
    
    word ptr = YASS_EEPROM_BASE_SEQ1;
    for(byte x = 0; x < NB_SEQS; x++)
    {
        console.print(F("Sequence "));
        console.println(x+1);
        debug->dumpEeprom(ptr, YASS_SEQUENCE_DATA_SIZE);
        ptr += YASS_SEQUENCE_DATA_SIZE;
    }
    state = YASS_MAINTENANCE_STATE_READY;
}

/******************/
/* Public methods */
/******************/

YASS_MAINTENANCE::YASS_MAINTENANCE()
{
}

void YASS_MAINTENANCE::begin(
        YASS_MONOSTABLE* dot_in_monostable,
        YASS_CONF_FSM* _editor,
        YASS_KEYB* _keyb,
        YASS_BEEPER* _beeper,
        YASS_GPO* shifter,
        YASS_CONFIG* glob_conf,
        YASS_SEQUENCER* _player,
        YASS_TICKS* _ticks,
        YASS_DISPLAY* _display,
        YASS_LEDS* _leds,
        YASS_COMPUTE_BEAT* beat_calc,
        YASS_EEPROM* _storage,
        YASS_SYS_EX* sys_ex,
        ARDUINO_DEBUG* _debug)
{
    dotInMonostable = dot_in_monostable;
    editor = _editor;
    keyb = _keyb;
    beeper = _beeper;
    shifter = shifter;
    globConf = glob_conf;
    player = _player;
    ticks = _ticks;
    display = _display;
    leds = _leds,
    beatCalc = beat_calc;
    storage = _storage;
    sysEx = sys_ex;
    debug = _debug;
    
    console.flush();
    console.begin(9600);
    editor->getEncoder()->bind(YASS_MAINTENANCE_updateData);
    resetOutputs();
    console.println(F("---*** YASS MAINTENANCE ***---\n"));
    prompt();
    
}

void YASS_MAINTENANCE::sequencer()
{
    if(console.available())
    {
        switch (console.read())
        {
            case '?':
                setState(YASS_MAINTENANCE_STATE_READY);
                prompt();
                break;
            case '1':
                setState(YASS_MAINTENANCE_STATE_STEP_LEDS);
                console.println(F("\nTesting step leds."));
                break;
            case '2':
                setState(YASS_MAINTENANCE_STATE_INDV_LEDS);
                console.println(F("\nTesting individual leds."));
                break;
            case '3':
                setState(YASS_MAINTENANCE_STATE_DISPLAY);
                segmentWeight = 1;
                console.println(F("\nTesting display's segments."));
                break;
            case '4':
                setState(YASS_MAINTENANCE_STATE_KEYBOARD);
                console.println(F("\nTesting keyboard. Hit keys"));
                break;
            case '5':
                setState(YASS_MAINTENANCE_STATE_BEEPER);
                console.println(F("\nTesting beeper."));
                break;
            case '6':
                setState(YASS_MAINTENANCE_STATE_ENCODER);
                console.println(F("\nTesting encoder. Change value manually."));
                break;
            case '7':
                setState(YASS_MAINTENANCE_STATE_EEPROM);
                console.println(F("\nDumping eeprom."));
                break;
            case 'v':
                displayVersions();
                break;
            case 's':
                console.println(F("\nTest stopped."));
                setState(YASS_MAINTENANCE_STATE_READY);
                break;
            default:
                break;
        }
    }
    switch (state)
    {
        case YASS_MAINTENANCE_STATE_STEP_LEDS:
            stateStepLeds();
            break;
        case YASS_MAINTENANCE_STATE_INDV_LEDS:
            stateIndividualLeds();
            break;
        case YASS_MAINTENANCE_STATE_DISPLAY:
            stateDisplay();
            break;
        case YASS_MAINTENANCE_STATE_KEYBOARD:
            stateKeyboard();
            break;
        case YASS_MAINTENANCE_STATE_BEEPER:
            stateBeeper();
            break;
        case YASS_MAINTENANCE_STATE_ENCODER:
            stateEncoder();
            break;
        case YASS_MAINTENANCE_STATE_EEPROM:
            stateEeprom();
            break;
        case YASS_MAINTENANCE_STATE_READY:
        default:
            break;
    }
}

