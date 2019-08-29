#ifndef yass_h
#define yass_h

/*
 * file : yass.h
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

#define MONOSTABLE_MSEC_DURATION 100

#define KBD_1 '1'
#define KBD_2 '2'
#define KBD_3 '3'
#define KBD_4 '4'
#define KBD_5 '5'
#define KBD_NEXT 'A'
#define KBD_START 'B'
#define KBD_PAUSE 'C'
#define KBD_CONTINUE 'D'
#define KBD_RECORD 'E'
#define KBD_GLOBAL 'F'
#define KBD_SEQUENCE 'G'
#define KBD_REST 'H'
#define KBD_TIE 'I'
#define KBD_MULTI_USAGE 'J'

#define ENC_A A1
#define ENC_B A0

#define BEEP_MSEC_DURATION 40
#define BEEPER_PIN A2

#define LED_1 5
#define LED_2 4
#define LED_3 3
#define LED_4 2
#define LED_5 1
#define LED_START 7
#define LED_RECORD 6
#define LED_GLOBAL 8
#define LED_SEQUENCE 9
#define LED_REST 0
#define LED_TIE 11
#define LED_MULTI_USAGE 10

#define LED_STEP_0  0
#define LED_STEP_1  1
#define LED_STEP_2  2
#define LED_STEP_3  3
#define LED_STEP_4  4
#define LED_STEP_5  5
#define LED_STEP_6  6
#define LED_STEP_7  7
#define LED_STEP_8  8
#define LED_STEP_9  9
#define LED_STEP_10 10
#define LED_STEP_11 11
#define LED_STEP_12 12
#define LED_STEP_13 13
#define LED_STEP_14 14
#define LED_STEP_15 15

#define YASS_GPO_DATA_PIN 2 
#define YASS_GPO_CLOCK_PIN 3 
#define YASS_GPO_LATCH_PIN 4 

#define CC_ALL_NOTES_OFF 120
#define FIRST_MIDI_CHANNEL 1
#define LAST_MIDI_CHANNEL 16
#define FIRST_MIDI_PROG_NUM 0
#define LAST_MIDI_PROG_NUM 127
#define FIRST_MIDI_VELOCITY 0
#define LAST_MIDI_VELOCITY 127
#define MIDI_REST 0
#define FIRST_MIDI_NOTE MIDI_REST
#define MIDI_TIE 128
#define LAST_MIDI_NOTE MIDI_TIE
#define LAST_CTRL_CHG 127
#define LAST_FIXED_VELOCITY 127

#define MIDI_IN_DOT 0
#define MIDI_OUT_DOT 1
#define SEQ_OUT_DOT 2
#define CLICK_DOT 3

#define STAVE_SIZE 12
#define DEFAULT_BPM 120

#define PR_STOP        0
#define PR_PLAY_ONLY   1
#define PR_RECORD_ONLY 2
#define PR_PLAY_RECORD 3

/********************/
/* Some cpp objects */
/********************/

// MIDI interface
midi::MidiInterface<HardwareSerial> MIDI = midi::MidiInterface<HardwareSerial>(Serial);

// led for midi in
YASS_MONOSTABLE dotInMonostable = YASS_MONOSTABLE();
#define DOT_MONO_STABLE_DELAY_MSEC 50

// data editor's finite state machine
YASS_CONF_FSM configurator = YASS_CONF_FSM();

// Keyboard
int colPins[] = {5, 6, 7, 8, 9};
int rowPins[] = {10, 11, 12};
#define NB_KBD_COLUMS (sizeof(colPins)/sizeof(int))
#define NB_KBD_ROWS (sizeof(rowPins)/sizeof(int))
char kbdCodes[] =
    {
    KBD_1, KBD_2, KBD_3, KBD_4, KBD_5,
    KBD_REST, KBD_START, KBD_PAUSE, KBD_CONTINUE, KBD_RECORD,
    KBD_TIE, KBD_MULTI_USAGE, KBD_NEXT, KBD_SEQUENCE, KBD_GLOBAL
    };
YASS_KEYB keyb = YASS_KEYB(colPins, NB_KBD_COLUMS, rowPins, NB_KBD_ROWS, kbdCodes);

// beeper
YASS_BEEPER beeper = YASS_BEEPER(BEEPER_PIN, BEEP_MSEC_DURATION);

// Gpio shifter (leds & 7 segments display)
YASS_GPO shifter = YASS_GPO(YASS_GPO_DATA_PIN, YASS_GPO_CLOCK_PIN, YASS_GPO_LATCH_PIN);
byte oldGpoData[] = {0, 0, 0, 0, 0};
byte gpoData[]    = {0, 0, 0, 0, 0};
#define NB_GPO_DATA (sizeof(gpoData) / sizeof(byte))

// Leds driver
#define individualLed_ptr (&gpoData[0])
#define stepLed_ptr (&gpoData[2])
YASS_LEDS leds = YASS_LEDS(individualLed_ptr, stepLed_ptr);
#define DEFAULT_BASE 10

// global configuration (finite state machine)
YASS_CONFIG globConf = YASS_CONFIG();

// set of sequences
YASS_SEQUENCE seqs[] = 
{
    YASS_SEQUENCE(),
    YASS_SEQUENCE(),
    YASS_SEQUENCE(),
    YASS_SEQUENCE(),
    YASS_SEQUENCE()
};
#define NB_SEQS (sizeof(seqs) / sizeof(YASS_SEQUENCE))

// sequencer
YASS_SEQUENCER player = YASS_SEQUENCER();

// internal midi clock
YASS_TICKS ticks = YASS_TICKS();

// display 4 digits 7 segments
#define DISPLAY_COMMON_ANODE_SEGMENT_LOGIC 0
#define DISPLAY_COMMON_ANODE_DIGIT_LOGIC 1
unsigned long displayMilestone;
#define DISPLAY_MSEC_DURATION 8
byte digitIndex = 3;
#define NB_DIGITS 4
byte displayBuf[NB_DIGITS];
#define DISPLAY_BASE 16
YASS_DISPLAY display = YASS_DISPLAY(NB_DIGITS, displayBuf);
#define GPO_SEGMENTS_INDEX 4
#define digit_number_ptr (&gpoData[1])
#define segments_ptr (&gpoData[GPO_SEGMENTS_INDEX])

// bpm calculator for external clock
YASS_COMPUTE_BEAT beatCalc = YASS_COMPUTE_BEAT();

// eeprom driver
YASS_EEPROM storage = YASS_EEPROM();

/*************************/
/* Some global variables */
/*************************/

bool globEditFlag;
byte globEditIndex = YASS_CONF_FSM_FIRST_GLOBAL;
bool seqEditFlag;
byte seqEditIndex = YASS_CONF_FSM_FIRST_SEQ;
bool editDataFlag;
byte romSequenceIndex =YASS_ROM_SEQUENCE_FIRST;
byte copySeqIndex = 0;
byte swapSeqIndex = 0;

const byte sequenceLedLut[] PROGMEM = {LED_1, LED_2, LED_3, LED_4, LED_5};

#define LUT_INDEX_NOTI   0
#define LUT_INDEX_OMNI   1
#define LUT_INDEX_NONE   2
#define LUT_INDEX_LOAD   3
#define LUT_INDEX_STOR   4
#define LUT_INDEX_COPY   5
#define LUT_INDEX_PAST   6
#define LUT_INDEX_DUMP   7
#define LUT_INDEX_NO_BPM 8
const char genericLut[]  PROGMEM = 
    "noti" "oMni" "none" "Load" "stor" "copy" "past" "dump" "----";
const char sqArpLut[]     PROGMEM = 
    "sequ" "arpe";
const char boolLut[]     PROGMEM = 
    "off " "on  ";
const char dataModeLut[] PROGMEM = 
    "dyna" "cchg" "fdyn";
const char grooveLut[]   PROGMEM = 
    "4   " "8   " "8t  " "8 SG" "16  " "16t " "16SG";
const char gateModeLut[] PROGMEM = 
    "tIny" "regu" "Long";
const char noteLabelLut[]PROGMEM = 
    "REST" "Db-1" "D-1 " "Eb-1" "E-1 " "F-1 " "Gb-1" "G-1 " "Ab-1" "A-1 " "Bb-1" "B-1 " 
    "C0  " "Db0 " "D0  " "Eb0 " "E0  " "F0  " "Gb0 " "G0  " "Ab0 " "A0  " "Bb0 " "B0  " 
    "C1  " "Db1 " "D1  " "Eb1 " "E1  " "F1  " "Gb1 " "G1  " "Ab1 " "A1  " "Bb1 " "B1  " 
    "C2  " "Db2 " "D2  " "Eb2 " "E2  " "F2  " "Gb2 " "G2  " "Ab2 " "A2  " "Bb2 " "B2  " 
    "C3  " "Db3 " "D3  " "Eb3 " "E3  " "F3  " "Gb3 " "G3  " "Ab3 " "A3  " "Bb3 " "B3  " 
    "C4  " "Db4 " "D4  " "Eb4 " "E4  " "F4  " "Gb4 " "G4  " "Ab4 " "A4  " "Bb4 " "B4  " 
    "C5  " "Db5 " "D5  " "Eb5 " "E5  " "F5  " "Gb5 " "G5  " "Ab5 " "A5  " "Bb5 " "B5  " 
    "C6  " "Db6 " "D6  " "Eb6 " "E6  " "F6  " "Gb6 " "G6  " "Ab6 " "A6  " "Bb6 " "B6  " 
    "C7  " "Db7 " "D7  " "Eb7 " "E7  " "F7  " "Gb7 " "G7  " "Ab7 " "A7  " "Bb7 " "B7  " 
    "C8  " "Db8 " "D8  " "Eb8 " "E8  " "F8  " "Gb8 " "G8  " "Ab8 " "A8  " "Bb8 " "B8  " 
    "C9  " "Db9 " "D9  " "Eb9 " "E9  " "F9  " "Gb9 " "G9  " "TIE " ;
const char voiceLabelLut[]   PROGMEM = 
    "PIA1" "PIA2" "PIA3" "PIA4" "PIA5" "PIA6" "PIA7" "PIA8" 
    "CHP1" "CHP2" "CHP3" "CHP4" "CHP5" "CHP6" "CHP7" "CHP8" 
    "Org1" "Org2" "Org3" "Org4" "Org5" "Org6" "Org7" "Org8" 
    "Gui1" "Gui2" "Gui3" "Gui4" "Gui5" "Gui6" "Gui7" "Gui8" 
    "BAS1" "BAS2" "BAS3" "BAS4" "BAS5" "BAS6" "BAS7" "BAS8" 
    "Stg1" "Stg2" "Stg3" "Stg4" "Stg5" "Stg6" "Stg7" "Stg8" 
    "Ens1" "Ens2" "Ens3" "Ens4" "Ens5" "Ens6" "Ens7" "Ens8" 
    "Brs1" "Brs2" "Brs3" "Brs4" "Brs5" "Brs6" "Brs7" "Brs8" 
    "Ree1" "Ree2" "Ree3" "Ree4" "Ree5" "Ree6" "Ree7" "Ree8" 
    "PIP1" "PIP2" "PIP3" "PIP4" "PIP5" "PIP6" "PIP7" "PIP8" 
    "Lea1" "Lea2" "Lea3" "Lea4" "Lea5" "Lea6" "Lea7" "Lea8" 
    "Pad1" "Pad2" "Pad3" "Pad4" "Pad5" "Pad6" "Pad7" "Pad8" 
    "Eff1" "Eff2" "Eff3" "Eff4" "Eff5" "Eff6" "Eff7" "Eff8" 
    "Eth1" "Eth2" "Eth3" "Eth4" "Eth5" "Eth6" "Eth7" "Eth8" 
    "Prc1" "Prc2" "Prc3" "Prc4" "Prc5" "Prc6" "Prc7" "Prc8" 
    "SEF1" "SEF2" "SEF3" "SEF4" "SEF5" "SEF6" "SEF7" "SEF8" ;
    
#endif
