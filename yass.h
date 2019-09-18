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

#define ENC_A A1
#define ENC_B A0

#define PARAM_DECREASE false
#define PARAM_INCREASE true

#define CC_ALL_NOTES_OFF 120
#define FIRST_MIDI_CHANNEL 1
#define LAST_MIDI_CHANNEL 16
#define FIRST_MIDI_PROG_NUM 0
#define LAST_MIDI_PROG_NUM 127
#define FIRST_MIDI_VELOCITY 0
#define LAST_MIDI_VELOCITY 127
#define DEFAULT_MIDI_VELOCITY 100
#define MIDI_REST 0
#define FIRST_MIDI_NOTE MIDI_REST
#define MIDI_TIE 128
#define LAST_MIDI_NOTE MIDI_TIE
#define LAST_CTRL_CHG 127
#define LAST_FIXED_VELOCITY 127

#define STAVE_SIZE 12
#define DEFAULT_BPM 120

#define PR_STOP        0
#define PR_PLAY_ONLY   1
#define PR_RECORD_ONLY 2
#define PR_PLAY_RECORD 3

/*********************/
/* states definition */
/*********************/

#define EDIT_STATE_TEMPO            1
#define EDIT_STATE_RECORD           2
#define EDIT_STATE_NO_CODE          3

// global parameters
#define EDIT_STATE_MIDI_IN          16
#define EDIT_STATE_MIDI_OUT         17
#define EDIT_STATE_PROGRAM_NUMBER   18
#define EDIT_STATE_ARPEGGIATOR      19
#define EDIT_STATE_CLOCK_IN         20
#define EDIT_STATE_CLOCK_OUT        21
#define EDIT_STATE_KEY_ECHO         22
#define EDIT_STATE_CLICK            23
#define EDIT_STATE_USE_SYSEX        24
#define EDIT_STATE_TRANSPOSITION    25
#define EDIT_STATE_ALL_DUMP         26
#define EDIT_STATE_ALL_LOAD         27
#define EDIT_STATE_ALL_SAVE         28
#define EDIT_STATE_GLOB_DUMP        29
#define EDIT_STATE_GLOB_LOAD        30
#define EDIT_STATE_GLOB_SAVE        31

#define EDIT_STATE_FIRST_GLOBAL     EDIT_STATE_MIDI_IN
#define EDIT_STATE_LAST_GLOBAL      EDIT_STATE_GLOB_SAVE

// sequence parameters
#define EDIT_STATE_GROOVE           32
#define EDIT_STATE_GATE_MODE        33
#define EDIT_STATE_LAST_STEP        34
#define EDIT_STATE_DATA_MODE        35
#define EDIT_STATE_CTRL_CHG         36
#define EDIT_STATE_FIX_VEL          37
// here rooms for 3 extra states
#define EDIT_STATE_INIT_SEQ         41
#define EDIT_STATE_SWAP             42
#define EDIT_STATE_COPY             43
#define EDIT_STATE_SEQ_LOAD_FACTORY 44
#define EDIT_STATE_SEQ_DUMP         45
#define EDIT_STATE_SEQ_LOAD         46
#define EDIT_STATE_SEQ_SAVE         47

#define EDIT_STATE_FIRST_SEQ        EDIT_STATE_GROOVE
#define EDIT_STATE_LAST_SEQ         EDIT_STATE_SEQ_SAVE

/********************/
/* Some cpp objects */
/********************/

// MIDI interface
midi::MidiInterface<HardwareSerial> MIDI = midi::MidiInterface<HardwareSerial>(Serial);

// delay before noteOff when changing current step with PREV and NEXT
YASS_MONOSTABLE recordedNoteNoteOff = YASS_MONOSTABLE();
#define RECORDED_NOTE_NOTE_OFF_DELAY_MSEC 200

// led for midi in
YASS_MONOSTABLE dotInMonostable = YASS_MONOSTABLE();
#define DOT_MONO_STABLE_DELAY_MSEC 50

// data editor's finite state machine
YASS_EDIT_FSM editor = YASS_EDIT_FSM();

// Keyboard
#define KBD_NO_KEY                      0x0000
#define KBD_1_PICTURE_VALUE             0x0001
#define KBD_2_PICTURE_VALUE             0x0002
#define KBD_3_PICTURE_VALUE             0x0004
#define KBD_4_PICTURE_VALUE             0x0008
#define KBD_5_PICTURE_VALUE             0x0010
#define KBD_REST_PICTURE_VALUE          0x0020
#define KBD_START_PICTURE_VALUE         0x0040
#define KBD_PAUSE_PICTURE_VALUE         0x0080
#define KBD_PREVIOUS_PICTURE_VALUE      0x0100
#define KBD_RECORD_PICTURE_VALUE        0x0200
#define KBD_TIE_PICTURE_VALUE           0x0400
#define KBD_MULTI_USAGE_PICTURE_VALUE   0x0800
#define KBD_NEXT_PICTURE_VALUE          0x1000
#define KBD_SEQUENCE_PICTURE_VALUE      0x2000
#define KBD_GLOBAL_PICTURE_VALUE        0x4000
int colPins[] = {5, 6, 7, 8, 9};
int rowPins[] = {10, 11, 12};
#define NB_KBD_COLUMS (sizeof(colPins)/sizeof(int))
#define NB_KBD_ROWS (sizeof(rowPins)/sizeof(int))
char kbdCodes[] =
    {
    KBD_1, KBD_2, KBD_3, KBD_4, KBD_5,
    KBD_REST, KBD_START, KBD_PAUSE, KBD_PREVIOUS, KBD_RECORD,
    KBD_TIE, KBD_MULTI_USAGE, KBD_NEXT, KBD_SEQUENCE, KBD_GLOBAL
    };
YASS_KEYB keyb = YASS_KEYB(colPins, NB_KBD_COLUMS, rowPins, NB_KBD_ROWS, kbdCodes);

// beeper
#define BEEP_MSEC_DURATION 40
#define BEEPER_PIN A2
YASS_BEEPER beeper = YASS_BEEPER(BEEPER_PIN, BEEP_MSEC_DURATION);

// Gpio shifter (leds & 7 segments display)
#define YASS_GPO_DATA_PIN 2 
#define YASS_GPO_CLOCK_PIN 3 
#define YASS_GPO_LATCH_PIN 4 
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
//~ #define NB_SEQS (sizeof(seqs) / sizeof(YASS_SEQUENCE))

// sequencer
YASS_SEQUENCER player = YASS_SEQUENCER();

// internal midi clock
YASS_TICKS ticks = YASS_TICKS();

// display 4 digits 7 segments
#define DISPLAY_COMMON_ANODE_SEGMENT_LOGIC 0
#define DISPLAY_COMMON_ANODE_DIGIT_LOGIC 1
#define MIDI_IN_DOT 0
#define MIDI_OUT_DOT 1
#define SEQ_OUT_DOT 2
#define CLICK_DOT 3
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
#define DISPLAY_FACTORY_SPLASH_DURATION 1300

// bpm calculator for external clock
YASS_COMPUTE_BEAT beatCalc = YASS_COMPUTE_BEAT();

// eeprom driver
YASS_EEPROM storage = YASS_EEPROM();

// midi dump driver
YASS_SYS_EX sysEx = YASS_SYS_EX();

#ifdef INCLUDE_MAINTENANCE
// some function to display RAM, ROM, EEPROM and
// to display on a 9600bd terminal despite midi driver
ARDUINO_DEBUG debug = ARDUINO_DEBUG();
// special maintenance object - not supposed to be used by users
YASS_MAINTENANCE maintenance = YASS_MAINTENANCE();
#endif

/*************************/
/* Some global variables */
/*************************/

enum _yassState {stopped=1, paused=2, running=3};
enum _yassState yassState;

bool globEditFlag;
byte globEditIndex = EDIT_STATE_FIRST_GLOBAL;
bool seqEditFlag;
byte seqEditIndex = EDIT_STATE_FIRST_SEQ;
bool editDataFlag;
byte romSequenceIndex =YASS_ROM_SEQUENCE_FIRST;
byte copySeqIndex = 0;
byte swapSeqIndex = 0;
void (*displayCallback)() = NULL;
signed char transposition = 0;
byte defaultNote = 64;

#ifdef INCLUDE_MAINTENANCE
bool serialDebug = false;
#endif

const byte sequenceLedLut[] PROGMEM = {LED_1, LED_2, LED_3, LED_4, LED_5};

#define LUT_INDEX_DUMP_SEQ 0
#define LUT_INDEX_LOAD_SEQ 1
#define LUT_INDEX_SAVE_SEQ 2
#define LUT_INDEX_DUMP_ALL 3
#define LUT_INDEX_LOAD_ALL 4
#define LUT_INDEX_SAVE_ALL 5
#define LUT_INDEX_DUMP_GLO 6
#define LUT_INDEX_LOAD_GLO 7
#define LUT_INDEX_SAVE_GLO 8

const char ioLut[]  PROGMEM = 
    "Dseq" "Lseq" "Sseq" "DaLL" "LAll" "SAll" "Dglo" "Lglo" "Sglo";

#define LUT_INDEX_NO_CODE     0
#define LUT_INDEX_OMNI        1
#define LUT_INDEX_NONE        2
#define LUT_INDEX_COPY        3
#define LUT_INDEX_PAST        4
#define LUT_INDEX_NO_BPM      5
#define LUT_INDEX_FCT0        6
#define LUT_INDEX_FCT1        7
#define LUT_INDEX_FCT2        8
#define LUT_INDEX_DEBUG       9
#define LUT_INDEX_FCT3        10
#define LUT_INDEX_APP_NAME    11
#define LUT_INDEX_APP_VERSION 12

const char genericLut[]  PROGMEM = 
    "noco" "oMni" "none" "copy" "past" "----" "init" "ron1" "ron2"
    "dbug" "ron3" "YASS" YASS_VERSION;
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
    
#endif
