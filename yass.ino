/*
 * file : yass.ino
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

/***************************************/
/*  YASS - Yet another step sequencer  */
/* MIDI port is on Serial - NANO board */
/***************************************/

#define INCLUDE_MAINTENANCE

/* ok to use a huge quantity of constants */
#include <avr/pgmspace.h>
/* a finite state machine for editing each parameter */
#include "yassEditFsm.h"
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

#ifdef INCLUDE_MAINTENANCE
#include <arduinoDebug.h>
#include "yassMaintenance.h"
#endif
/***********************************/
/* all global data is defined here */
/***********************************/

#include "yass.h"

/*******************************************/
/* Don't know were to put theses functions */
/*******************************************/

void swap(byte* source, byte* target, word nb_bytes)
{
    byte temp_byte;
    while(nb_bytes--)
    {
        temp_byte = *source;
        *source++ = *target;
        *target++ = temp_byte;
    }
}

/************************************/
/* Factorisation of some code parts */
/************************************/

void allNotesOff()
{
    MIDI.sendControlChange(CC_ALL_NOTES_OFF, 0, globConf.getChannelOut());
}

void startDotIn()
{
    display.writeDot(1, MIDI_IN_DOT);
    dotInMonostable.start();
}

void dotInMonostableCallback()
{
    display.writeDot(0, MIDI_IN_DOT);
}

void freezeDisplay(word msec_value)
{
    // useful for some boot splash screens
    unsigned long int milestone = millis() + msec_value;
    while(milestone >= millis())
    {
        gpoTask();
        delay(DISPLAY_MSEC_DURATION);
    }
}

byte getPlayRecordState()
{
    return player.isRunning() + player.isRecording() * 2;
}

bool messageReceived(byte channel)
{
    return (globConf.getOmni() || (globConf.getChannelIn() == channel));
}
    
void changeProgramm()
{
    if(globConf.getUseProgNum())
       MIDI.sendProgramChange(globConf.getProgNum(), globConf.getChannelOut());
}
    
void keyBeep()
{
    if(globConf.getKeyEcho())
        beeper.beep();
}

bool functionWaitsValidation()
{
    bool ret_val;
    switch(editor.getState())
    {
        case EDIT_STATE_ALL_DUMP:
        case EDIT_STATE_ALL_LOAD:
        case EDIT_STATE_ALL_SAVE:
        case EDIT_STATE_GLOB_DUMP:
        case EDIT_STATE_GLOB_LOAD:
        case EDIT_STATE_GLOB_SAVE:
        case EDIT_STATE_INIT_SEQ:
        case EDIT_STATE_COPY:
        case EDIT_STATE_SWAP:
        case EDIT_STATE_SEQ_LOAD_FACTORY:
        case EDIT_STATE_SEQ_LOAD:
        case EDIT_STATE_SEQ_SAVE:
        case EDIT_STATE_SEQ_DUMP:
            ret_val = true;
            break;
        default:
            ret_val = false;
            break;
    }
    return ret_val;
}   

void exitEdit()
{
    if(globEditFlag)
        commandGlobalEdit();
    else if(seqEditFlag)
        commandSequenceEdit();
}

void execContinue()
{
    player.continueSequencer();
    yassState = running;
    keyBeep();
}

void execPause()
{
    player.stopSequencer();
    yassState = paused;
    keyBeep();
}

void execStart()
{
    player.startSequencer();
    yassState = running;
    keyBeep();
}

void execStop()
{
    player.stopSequencer();
    yassState = stopped;
    player.setCurrentSequenceIndex(player.getNextSequenceIndex());
    keyBeep();
}

/************************/
/* MIDI input callbacks */
/************************/

void receiveExclusive(byte * array_ptr, word array_size)
{
    if(globConf.getUseSysEx())
    {
        startDotIn();
        sysEx.executeSysEx(array_ptr, array_size);
    }
}

void receiveNoteOn(byte channel, byte note, byte velocity)
{
    if(messageReceived(channel))
    {
        startDotIn();
        note = (note + transposition) & 0x7f;
        switch(getPlayRecordState())
        {
            case PR_STOP:
                MIDI.sendNoteOn(note, velocity, globConf.getChannelOut());
                break;
            case PR_PLAY_ONLY:
                player.setNextScale(note);
                break;
            case PR_RECORD_ONLY:
            case PR_PLAY_RECORD:
                MIDI.sendNoteOn(note, velocity, globConf.getChannelOut());
                player.addRecordedNote(note, velocity);
                break;
            default:
                break;
        }
    }
}

void receiveNoteOff(byte channel, byte note, byte velocity)
{
    if(messageReceived(channel))
    {
        startDotIn();
        note = (note + transposition) & 0x7f;
        switch(getPlayRecordState())
        {
            case PR_STOP:
                MIDI.sendNoteOff(note, velocity, globConf.getChannelOut());
                break;
            case PR_RECORD_ONLY:
            case PR_PLAY_RECORD:
                MIDI.sendNoteOff(note, velocity, globConf.getChannelOut());
                break;
            default:
                break;
        }
    }
}

void receiveControlChange(byte channel, byte number, byte value)
{
    if(messageReceived(channel))
    {
        startDotIn();
        //~ MIDI.sendNoteOff(number, value, globConf.getChannelOut());
        MIDI.sendControlChange(number, value, globConf.getChannelOut());
    }
}

void receiveClock()
{
    if(globConf.getClockIn())
        if(player.isRunning())
        {
            beatCalc.addTick();
            tickTask();
        }
}

void receiveStart()
{
    if(globConf.getClockIn())
    {
        execStart();
        beatCalc.start();
    }
}

void receiveStop()
{
    if(globConf.getClockIn())
    {
        //~ if(yassState != stopped)
            //~ execStop();
        //~ else
            execPause();
        beatCalc.stop();
    }
}

void receiveContinue()
{
    if(globConf.getClockIn())
    {
        execContinue();
        beatCalc.start();
    }
}
/***********************/
/* display's callbacks */
/***********************/

void displayTempo()
{
    if(!globConf.getClockIn())
        display.printWord(ticks.getTempo(), DEFAULT_BASE);        
    else
    {
        if(player.isRunning())
            display.printWord(beatCalc.getTempo(), DEFAULT_BASE);
        else
            display.printLut(genericLut, LUT_INDEX_NO_BPM, NB_DIGITS);
    }
}

void displayRecord()
{
    if(editDataFlag)
        display.printWord(player.getCurrentSequence()->getData(player.getRecordIndex()), DEFAULT_BASE);
    else
        display.printLut(noteLabelLut, player.getCurrentSequence()->getNote(player.getRecordIndex()), NB_DIGITS);
}

void displayNoCode()
{
    display.printLut(genericLut, LUT_INDEX_NO_CODE, NB_DIGITS);
}

void displayMidiIn()
{

    if(globConf.getOmni())
        display.printLut(genericLut, LUT_INDEX_OMNI, NB_DIGITS);
    else
        display.printWord(globConf.getChannelIn(), DEFAULT_BASE);
}

void displayMidiOut()
{
    display.printWord(globConf.getChannelOut(), DEFAULT_BASE);
}

void displayProgNum()
{
    if(globConf.getUseProgNum())
        display.printWord(globConf.getProgNum()+1, DEFAULT_BASE);
    else
        display.printLut(genericLut, LUT_INDEX_NONE, NB_DIGITS);
}

void displayArpeggiator()
{
    display.printLut(sqArpLut, globConf.getArpeggiator(), NB_DIGITS);
}

void displayClockIn()
{
    display.printLut(boolLut, globConf.getClockIn(), NB_DIGITS);
}

void displayClockOut()
{
    display.printLut(boolLut, globConf.getClockOut(), NB_DIGITS);
}

void displayKeyEcho()
{
    display.printLut(boolLut, globConf.getKeyEcho(), NB_DIGITS);
}

void displayClick()
{
    display.printLut(boolLut, globConf.getClick(), NB_DIGITS);
}

void displayUseSysEx()
{
    display.printLut(boolLut, globConf.getUseSysEx(), NB_DIGITS);
}

void displayTransposition()
{
    display.printSignedWord(transposition, DEFAULT_BASE);
}

void displayDumpAll()
{
    display.printLut(ioLut, LUT_INDEX_DUMP_ALL, NB_DIGITS);
}

void displayLoadAll()
{
    display.printLut(ioLut, LUT_INDEX_LOAD_ALL, NB_DIGITS);
}

void displaySaveAll()
{
    display.printLut(ioLut, LUT_INDEX_SAVE_ALL, NB_DIGITS);
}

void displayDumpGlo()
{
    display.printLut(ioLut, LUT_INDEX_DUMP_GLO, NB_DIGITS);
}

void displayLoadGlo()
{
    display.printLut(ioLut, LUT_INDEX_LOAD_GLO, NB_DIGITS);
}

void displaySaveGlo()
{
    display.printLut(ioLut, LUT_INDEX_SAVE_GLO, NB_DIGITS);
}
        
void displayGroove()
{
    display.printLut(grooveLut, player.getCurrentSequence()->getGroove(), NB_DIGITS);
}

void displayGateMode()
{
    display.printLut(gateModeLut, player.getCurrentSequence()->getGateMode(), NB_DIGITS);
}

void displayLastStep()
{
    display.printWord(player.getCurrentSequence()->getLastStep() + 1, DEFAULT_BASE);
}

void displayDataMode()
{
    display.printLut(dataModeLut, player.getCurrentSequence()->getDataMode(), NB_DIGITS);
}

void displayCtrlChg()
{
    display.printWord(player.getCurrentSequence()->getControlChange(), DEFAULT_BASE);
}

void displayFixedVelocity()
{
    display.printWord(player.getCurrentSequence()->getFixedVelocity(), DEFAULT_BASE);
}

void displayInitSeq()
{
    display.printLut(grooveLut, initializationIndex, NB_DIGITS);
}

void displayCopySeq()
{
    display.printWord(copySeqIndex + 1, DEFAULT_BASE);
}
void displaySwap()
{
    display.printWord(swapSeqIndex + 1, DEFAULT_BASE);
}

void displaySeqLoadFactory()
{
    display.printWord(romSequenceIndex, DEFAULT_BASE);
}

void displaySeqDump()
{
    display.printLut(ioLut, LUT_INDEX_DUMP_SEQ, NB_DIGITS);
}
void displaySeqLoad()
{
    display.printLut(ioLut, LUT_INDEX_LOAD_SEQ, NB_DIGITS);
}

void displaySeqSave()
{
    display.printLut(ioLut, LUT_INDEX_SAVE_SEQ, NB_DIGITS);
}

/***********************/
/* encoder's callbacks */
/***********************/

void editTempo(bool direction)
{
    if(!globConf.getClockIn())
    {
        if(direction == YASS_ENCODER_DIRECTION_UP)
            ticks.setTempo(ticks.getTempo() + 1);
        else
            ticks.setTempo(ticks.getTempo() - 1);
    }
}

void editRecord(bool direction)
{
    YASS_SEQUENCE* seq = player.getCurrentSequence();
    byte index  = player.getRecordIndex();
    byte note = seq->getNote(index);
    byte data = seq->getData(index);
    
    if(direction == YASS_ENCODER_DIRECTION_UP)
    {
        if(editDataFlag)
        {
            if(data < LAST_MIDI_VELOCITY)
                seq->setData(index, data + 1);
        }
        else
        {
            if(note < LAST_MIDI_NOTE)
                seq->setNote(index, note + 1);
        }
    }
    else
    {
        if(editDataFlag)
        {
            if(data > FIRST_MIDI_VELOCITY)
                seq->setData(index, data - 1);
        }
        else
        {
            if(note > FIRST_MIDI_NOTE)
                seq->setNote(index, note - 1);
        }
    }
}

void editNoCode(bool direction)
{
    direction++; // just to fake the compiler
}

void editMidiIn(bool direction)
{
    allNotesOff();
    if(direction == YASS_ENCODER_DIRECTION_UP)
    {
        if(globConf.getOmni())
        {
            globConf.setOmni(0);
            globConf.setChannelIn(FIRST_MIDI_CHANNEL);
        }
        else
            if(globConf.getChannelIn() <= LAST_MIDI_CHANNEL)
                globConf.setChannelIn(globConf.getChannelIn() + 1);
    }
    else
    {
        if((globConf.getChannelIn() == FIRST_MIDI_CHANNEL) && !globConf.getOmni())
            globConf.setOmni(1);
        else if(globConf.getChannelIn() > FIRST_MIDI_CHANNEL)
            globConf.setChannelIn(globConf.getChannelIn() - 1);
    }
}

void editMidiOut(bool direction)
{
    allNotesOff();
    player.stopCurrentNote();
    if(direction == YASS_ENCODER_DIRECTION_UP)
        globConf.setChannelOut(globConf.getChannelOut() + 1);
    else
        globConf.setChannelOut(globConf.getChannelOut() - 1);
    allNotesOff();
}

void editProgNum(bool direction)
{
    allNotesOff();
    player.stopCurrentNote();
    byte prog_num = globConf.getProgNum();
    if(direction == YASS_ENCODER_DIRECTION_UP)
    {
        if(globConf.getUseProgNum()) 
        {
            if(prog_num < LAST_MIDI_PROG_NUM)
                globConf.setProgNum(prog_num + 1);
        }
        else
        {
            globConf.setProgNum(FIRST_MIDI_PROG_NUM);
            globConf.setUseProgNum(1);
        }
    }
    else
        if(globConf.getUseProgNum()) 
        {
            if(prog_num > FIRST_MIDI_PROG_NUM)
                globConf.setProgNum(prog_num - 1);
            else
                globConf.setUseProgNum(0);
        }
    changeProgramm() ;
    allNotesOff();
}
    
void editArpeggiator(bool direction)
{
    if(direction == YASS_ENCODER_DIRECTION_UP)
        globConf.setArpeggiator(1);
    else
        globConf.setArpeggiator(0);
}

void editClockIn(bool direction)
{
    if(direction == YASS_ENCODER_DIRECTION_UP)
        globConf.setClockIn(1);
    else
        globConf.setClockIn(0);
}

void editClockOut(bool direction)
{
    if(direction == YASS_ENCODER_DIRECTION_UP)
        globConf.setClockOut(1);
    else
        globConf.setClockOut(0);
}

void editKeyEcho(bool direction)
{
    if(direction == YASS_ENCODER_DIRECTION_UP)
        globConf.setKeyEcho(1);
    else
        globConf.setKeyEcho(0);
}

void editClick(bool direction)
{
    if(direction == YASS_ENCODER_DIRECTION_UP)
        globConf.setClick(1);
    else
        globConf.setClick(0);
}

void editSysEx(bool direction)
{
    if(direction == YASS_ENCODER_DIRECTION_UP)
        globConf.setUseSysEx(1);
    else
        globConf.setUseSysEx(0);
}

void editTransposition(bool direction)
{
    allNotesOff();
    if(direction == YASS_ENCODER_DIRECTION_UP)
        transposition++;
    else
        transposition--;
    allNotesOff();
}

void editGroove(bool direction)
{
    allNotesOff();
    YASS_SEQUENCE* seq = player.getCurrentSequence();
    byte groove = seq->getGroove();
    if(direction == YASS_ENCODER_DIRECTION_UP)
    {
        if(groove < YASS_SEQUENCE_LAST_GROOVE)
            groove += 1;
    }
    else
    {
        if(groove)
            groove -= 1;
    }
    seq->setGroove(groove);
}

void editGateMode(bool direction)
{
    allNotesOff();
    YASS_SEQUENCE* seq = player.getCurrentSequence();
    byte gate_mode = seq->getGateMode();
    if(direction == YASS_ENCODER_DIRECTION_UP)
    {
        if(gate_mode < YASS_SEQUENCE_GATE_LAST)
            gate_mode += 1;
    }
    else
    {
        if(gate_mode)
            gate_mode -= 1;
    }
    seq->setGateMode(gate_mode);
}

void editLastStep(bool direction)
{
    allNotesOff();
    YASS_SEQUENCE* seq = player.getCurrentSequence();
    byte last_step = seq->getLastStep();
    if(direction == YASS_ENCODER_DIRECTION_UP)
    {
        if(last_step < (YASS_SEQUENCE_NB_NOTES - 1))
            seq->setLastStep(seq->getLastStep() + 1);
    }
    else
        if(last_step)
            seq->setLastStep(seq->getLastStep() - 1);
}

void editDataMode(bool direction)
{
    allNotesOff();
    YASS_SEQUENCE* seq = player.getCurrentSequence();
    byte data_mode = seq->getDataMode();
    if(direction == YASS_ENCODER_DIRECTION_UP)
    {
        if(data_mode < YASS_SEQUENCE_DATA_MODE_LAST)
            data_mode += 1;
    }
    else
    {
        if(data_mode)
            data_mode -= 1;
    }
    seq->setDataMode(data_mode);
}

void editCtrlChg(bool direction)
{
    YASS_SEQUENCE* seq = player.getCurrentSequence();
    player.stopCurrentNote();
    byte ctrl_chg = seq->getControlChange();
    if(direction == YASS_ENCODER_DIRECTION_UP)
    {
        if(ctrl_chg < LAST_CTRL_CHG)
            ctrl_chg += 1;
    }
    else
        if(ctrl_chg)
            ctrl_chg -= 1;
    seq->setControlChange(ctrl_chg);
}
    
void editFixedVelocity(bool direction)
{
    YASS_SEQUENCE* seq = player.getCurrentSequence();
    byte fix_vel = seq->getFixedVelocity();
    if(direction == YASS_ENCODER_DIRECTION_UP)
    {
        if(fix_vel < LAST_FIXED_VELOCITY)
            fix_vel += 1;
    }
    else
        if(fix_vel)
            fix_vel -= 1;
    seq->setFixedVelocity(fix_vel);
}
    

void editInitSeq(bool direction)
{
    if(direction == YASS_ENCODER_DIRECTION_UP)
    {
        if(initializationIndex < YASS_SEQUENCE_LAST_GROOVE)
            initializationIndex += 1;
    }
    else
    {
        if(initializationIndex)
            initializationIndex -= 1;
    }
}

void editSwapSeq(bool direction)
{
    if(direction == YASS_ENCODER_DIRECTION_UP)
    {
        if(swapSeqIndex < (NB_SEQS - 1))
            swapSeqIndex += 1;
    }
    else
        if(swapSeqIndex)
            swapSeqIndex -= 1;
}
    
void editCopySeq(bool direction)
{
    if(direction == YASS_ENCODER_DIRECTION_UP)
    {
        if(copySeqIndex < (NB_SEQS - 1))
            copySeqIndex += 1;
    }
    else
        if(copySeqIndex)
            copySeqIndex -= 1;
}
    
void editLoadFactory(bool direction)
{
    if(direction == YASS_ENCODER_DIRECTION_UP)
    {
        if(romSequenceIndex < YASS_ROM_SEQUENCE_LAST)
            romSequenceIndex += 1;
    }
    else
        if(romSequenceIndex > YASS_ROM_SEQUENCE_FIRST)
            romSequenceIndex -= 1;
}

/***********************************/
/* routing the encoder's callbacks */
/***********************************/

void setEditState(byte state)
{
    switch(state)
    {
        case EDIT_STATE_TEMPO:
            editor.setState(EDIT_STATE_TEMPO, &editTempo);
            displayCallback = &displayTempo;
            break;
        case EDIT_STATE_RECORD:
            editor.setState(EDIT_STATE_RECORD, &editRecord);
            displayCallback = &displayRecord;
            break;
        case EDIT_STATE_MIDI_IN:
            editor.setState(EDIT_STATE_MIDI_IN, &editMidiIn);
            displayCallback = &displayMidiIn;
            break;
        case EDIT_STATE_MIDI_OUT:
            editor.setState(EDIT_STATE_MIDI_OUT, &editMidiOut);
            displayCallback = &displayMidiOut;
            break;
        case EDIT_STATE_PROGRAM_NUMBER:
            editor.setState(EDIT_STATE_PROGRAM_NUMBER, &editProgNum);
            displayCallback = &displayProgNum;
            break;
        case EDIT_STATE_ARPEGGIATOR:
            editor.setState(EDIT_STATE_ARPEGGIATOR, &editArpeggiator);
            displayCallback = &displayArpeggiator;
            break;
        case EDIT_STATE_CLOCK_IN:
            editor.setState(EDIT_STATE_CLOCK_IN, &editClockIn);
            displayCallback = &displayClockIn;
            break;
        case EDIT_STATE_CLOCK_OUT:
            editor.setState(EDIT_STATE_CLOCK_OUT, &editClockOut);
            displayCallback = &displayClockOut;
            break;
        case EDIT_STATE_KEY_ECHO:
            editor.setState(EDIT_STATE_KEY_ECHO, &editKeyEcho);
            displayCallback = &displayKeyEcho;
            break;
        case EDIT_STATE_CLICK:
            editor.setState(EDIT_STATE_CLICK, &editClick);
            displayCallback = &displayClick;
            break;
        case EDIT_STATE_USE_SYSEX:
            editor.setState(EDIT_STATE_USE_SYSEX, &editSysEx);
            displayCallback = &displayUseSysEx;
            break;
        case EDIT_STATE_TRANSPOSITION:
            editor.setState(EDIT_STATE_TRANSPOSITION, &editTransposition);
            displayCallback = &displayTransposition;
            break;
        case EDIT_STATE_ALL_DUMP:
            editor.setState(EDIT_STATE_ALL_DUMP, &editNoCode);
            displayCallback = &displayDumpAll;
            break;
        case EDIT_STATE_ALL_LOAD:
            editor.setState(EDIT_STATE_ALL_LOAD, &editNoCode);
            displayCallback = &displayLoadAll;
            break;
        case EDIT_STATE_ALL_SAVE:
            editor.setState(EDIT_STATE_ALL_SAVE, &editNoCode);
            displayCallback = &displaySaveAll;
            break;
        case EDIT_STATE_GLOB_DUMP:
            editor.setState(EDIT_STATE_GLOB_DUMP, &editNoCode);
            displayCallback = &displayDumpGlo;
            break;
        case EDIT_STATE_GLOB_LOAD:
            editor.setState(EDIT_STATE_GLOB_LOAD, &editNoCode);
            displayCallback = &displayLoadGlo;
            break;
        case EDIT_STATE_GLOB_SAVE:
            editor.setState(EDIT_STATE_GLOB_SAVE, &editNoCode);
            displayCallback = &displaySaveGlo;
            break;
        case EDIT_STATE_GROOVE:
            editor.setState(EDIT_STATE_GROOVE, &editGroove);
            displayCallback = &displayGroove;
            break;
        case EDIT_STATE_GATE_MODE:
            editor.setState(EDIT_STATE_GATE_MODE, &editGateMode);
            displayCallback = &displayGateMode;
            break;
        case EDIT_STATE_LAST_STEP:
            editor.setState(EDIT_STATE_LAST_STEP, &editLastStep);
            displayCallback = &displayLastStep;
            break;
        case EDIT_STATE_DATA_MODE:
            editor.setState(EDIT_STATE_DATA_MODE, &editDataMode);
            displayCallback = &displayDataMode;
            break;
        case EDIT_STATE_CTRL_CHG:
            editor.setState(EDIT_STATE_CTRL_CHG, &editCtrlChg);
            displayCallback = &displayCtrlChg;
            break;
        case EDIT_STATE_FIX_VEL:
            editor.setState(EDIT_STATE_FIX_VEL, &editFixedVelocity);
            displayCallback = &displayFixedVelocity;
            break;
        case EDIT_STATE_INIT_SEQ:
            editor.setState(EDIT_STATE_INIT_SEQ, &editInitSeq);
            displayCallback = &displayInitSeq;
            break;
        case EDIT_STATE_SWAP:
            editor.setState(EDIT_STATE_SWAP, &editSwapSeq);
            displayCallback = &displaySwap;
            break;
        case EDIT_STATE_COPY:
            editor.setState(EDIT_STATE_COPY, &editCopySeq);
            displayCallback = &displayCopySeq;
            break;
        case EDIT_STATE_SEQ_LOAD_FACTORY:
            editor.setState(EDIT_STATE_SEQ_LOAD_FACTORY, &editLoadFactory);
            displayCallback = &displaySeqLoadFactory;
            break;
        case EDIT_STATE_SEQ_DUMP:
            editor.setState(EDIT_STATE_SEQ_DUMP, &editNoCode);
            displayCallback = &displaySeqDump;
            break;
        case EDIT_STATE_SEQ_LOAD:
            editor.setState(EDIT_STATE_SEQ_LOAD, &editNoCode);
            displayCallback = &displaySeqLoad;
            break;
        case EDIT_STATE_SEQ_SAVE:
            editor.setState(EDIT_STATE_SEQ_SAVE, &editNoCode);
            displayCallback = &displaySeqSave;
            break;
        default:
            editor.setState(EDIT_STATE_NO_CODE, &editNoCode);
            displayCallback = &displayNoCode;
            break;
    }
}

void setEditParam(bool direction)
{
    byte state = editor.getState();
    
    if(direction == PARAM_INCREASE)
    {
        switch(state)
        {
            case EDIT_STATE_LAST_SEQ:
                state = EDIT_STATE_FIRST_SEQ;
                break;
            case EDIT_STATE_FIX_VEL:
                state = EDIT_STATE_INIT_SEQ;
                break;
            case EDIT_STATE_LAST_GLOBAL:
                state = EDIT_STATE_FIRST_GLOBAL;
                break;
            default:
            // let's switch to next state
                state ++;
                break;
        }
    }
    else
    {
        switch(state)
        {
            case EDIT_STATE_FIRST_SEQ:
                state = EDIT_STATE_LAST_SEQ;
                break;
            case EDIT_STATE_INIT_SEQ:
                state = EDIT_STATE_FIX_VEL;
                break;
            case EDIT_STATE_FIRST_GLOBAL:
                state = EDIT_STATE_LAST_GLOBAL;
                break;
            default:
                // let's switch to previous state
                state--;
            break;
        }
    }
    
    if(globEditFlag)
        globEditIndex = state;
    else if(seqEditFlag)
        seqEditIndex = state;
    
    setEditState(state);
}
/****************/
/* HMI commands */
/****************/

void commandStart()
{
    switch(yassState)
    {
        case stopped:
            changeProgramm();
            execStart();
            break;
        case paused:
            execContinue();
            break;
        case running:
            execPause();
            break;
    }
}

void commandStop()
{
    execStop();
    if(!player.isRecording())
        setEditState(EDIT_STATE_TEMPO);
}

void commandRecord()
{
    if(player.isRecording())
    {
        player.stopRecordSequencer();
        setEditState(EDIT_STATE_TEMPO);
        keyBeep();
    }
    else
    {
        if(functionWaitsValidation())
        {
            allNotesOff();
            switch(editor.getState())
            {
                // states for global edition
                case EDIT_STATE_ALL_DUMP:
                    sysEx.sendAll();
                    break;
                case EDIT_STATE_ALL_LOAD:
                    storage.loadAll();
                    break;
                case EDIT_STATE_ALL_SAVE:
                    storage.saveAll();
                    break;
                case EDIT_STATE_GLOB_DUMP:
                    sysEx.sendGlobal();
                    break;
                case EDIT_STATE_GLOB_LOAD:
                    storage.loadGlobal();
                    break;
                case EDIT_STATE_GLOB_SAVE:
                    storage.saveGlobal();
                    break;
                // states for current sequence edition
                case EDIT_STATE_INIT_SEQ:
                    player.getCurrentSequence()->initialize();
                    player.getCurrentSequence()->setGroove(initializationIndex);
                    break;
                case EDIT_STATE_SWAP:
                    swap(player.getSequence(swapSeqIndex)->getDataPointer(), player.getCurrentSequence()->getDataPointer(), YASS_SEQUENCE_DATA_SIZE);
                    break;
                case EDIT_STATE_COPY:
                    memcpy(player.getSequence(copySeqIndex)->getDataPointer(), player.getCurrentSequence()->getDataPointer(), YASS_SEQUENCE_DATA_SIZE);
                    break;
                case EDIT_STATE_SEQ_LOAD_FACTORY:
                    YASS_ROM_SEQUENCES_load(romSequenceIndex, player.getCurrentSequence());
                    break;
                case EDIT_STATE_SEQ_LOAD:
                    storage.loadSequence(player.getCurrentSequenceIndex());
                    break;
                case EDIT_STATE_SEQ_SAVE:
                    storage.saveSequence(player.getCurrentSequenceIndex());
                    break;
                case EDIT_STATE_SEQ_DUMP:
                    sysEx.sendSequence(player.getCurrentSequenceIndex());
                    break;
                default:
                    break;
            }
            exitEdit();
            setEditState(EDIT_STATE_TEMPO);
            keyBeep();
        }
        else
        {
            globEditFlag = false;
            seqEditFlag = false;
            player.startRecordSequencer();
            setEditState(EDIT_STATE_RECORD);
            keyBeep();
            //~ if(!(globEditFlag | seqEditFlag))
            //~ {
                //~ player.startRecordSequencer();
                //~ setEditState(EDIT_STATE_RECORD);
                //~ keyBeep();
            //~ }
        }
    }
}

void commandSeqNum(byte seq_num)
{
    if(!player.isRecording())
    {
        if(yassState == stopped)
        {
            player.setNextSequenceIndex(seq_num);
            player.setCurrentSequenceIndex(seq_num);
        }
        else
            player.setNextSequenceIndex(seq_num);
        keyBeep();
    }
}

void commandSeq1()
{
    commandSeqNum(0);
}

void commandSeq2()
{
    commandSeqNum(1);
}

void commandSeq3()
{
    commandSeqNum(2);
}

void commandSeq4()
{
    commandSeqNum(3);
}

void commandSeq5()
{
    commandSeqNum(4);
}

void commandSequenceEdit()
{
    if(!player.isRecording())
    {
        seqEditFlag ^= 1;
        if(seqEditFlag)
        {
            // entering sequence edition mode
            globEditFlag = false;
            setEditState(seqEditIndex);        
        }
        else
        {
            // exiting sequence edition mode
            setEditState(EDIT_STATE_TEMPO);
        }
    }
    else
        seqEditFlag = false;
    keyBeep();
}

void commandGlobalEdit()
{
    if(!player.isRecording())
    {
        globEditFlag ^= 1;
        if(globEditFlag)
        {
            // entering global edition mode
            seqEditFlag = false;
            setEditState(globEditIndex);        
        }
        else
        {
            // exiting global edition mode
            setEditState(EDIT_STATE_TEMPO);
        }
    }
    else
        seqEditFlag = false;
    keyBeep();
}

void commandNext()
{
    if(globEditFlag | seqEditFlag)
    {
        setEditParam(PARAM_INCREASE);
        keyBeep();
    }
    else if(editor.getState() == EDIT_STATE_TEMPO)
    {
        if(!globConf.getClockIn())
        {
            ticks.setTempo(DEFAULT_BPM);
            keyBeep();
        }
    }
    else if(player.isRecording())
    {
        player.gotoNextRecordStep();
        keyBeep();
    }
}

void commandPrevious()
{
    if(globEditFlag | seqEditFlag)
    {
        setEditParam(PARAM_DECREASE);
        keyBeep();
    }
    else if(player.isRecording())
    {
        player.gotoPreviousRecordStep();
        keyBeep();
    }
}

void commandMultiUsage()
{
    if(player.isRecording())
        editDataFlag ^= 1;
    else
        player.stopCurrentNote();
    keyBeep();
}

void commandTie()
{
    if(player.isRecording())
        player.getCurrentSequence()->setNote(player.getRecordIndex(), MIDI_TIE);
}

void commandRest()
{
    if(player.isRecording())
        player.getCurrentSequence()->setNote(player.getRecordIndex(), MIDI_REST);
}

/***********************/
/* end of HMI commands */
/***********************/

void playerNoteOn(byte note, byte data)
{
    YASS_SEQUENCE* seq = player.getCurrentSequence();
    byte channel_out = globConf.getChannelOut();
    byte cc_number = seq->getControlChange();
    byte fixed_velocity = seq->getFixedVelocity();
    
    switch(seq->getDataMode())
    {
        case YASS_SEQUENCE_DATA_MODE_VELOCITY:
            MIDI.sendNoteOn(note, data, channel_out);
            break;
        case YASS_SEQUENCE_DATA_MODE_CTRL_CHG:        
            MIDI.sendNoteOn(note, fixed_velocity, channel_out);
            MIDI.sendControlChange(cc_number, data, channel_out);
            break;
        case YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY:
            MIDI.sendNoteOn(note, fixed_velocity, channel_out);
            break;
        default:
            break;
    }
    display.writeDot(1, SEQ_OUT_DOT);
}

void playerNoteOff(byte note)
{
    MIDI.sendNoteOff(note, 0, globConf.getChannelOut());
    display.writeDot(0, SEQ_OUT_DOT);
}

void sendClock(byte tick_num)
{
    // routing clock to MIDI out if needed
    if(globConf.getClockOut())
        MIDI.sendRealTime(midi::Clock);

    if(!tick_num)
    {
        if(globConf.getClick())
            // send click on buzzer if needed
            beeper.beep();
        // send click on a display's dot
        display.writeDot(1, CLICK_DOT);
    }
    else if(tick_num == (YASS_RESOLUTION / 2))
        display.writeDot(0, CLICK_DOT);
}

void sendStart()
{
    if(globConf.getClockOut())
        MIDI.sendRealTime(midi::Start);
}

void sendStop()
{
    if(globConf.getClockOut())
        MIDI.sendRealTime(midi::Stop);
}

void sendContinue()
{
    if(globConf.getClockOut())
        MIDI.sendRealTime(midi::Continue);
}

void sendExclusive(byte* addr, word size)
{
    MIDI.sendSysEx(size, addr);
}

/**************/
/* some tasks */
/**************/

void keybTask()
{
    if(keyb.available())
        switch(keyb.read())
        {
            case KBD_1:
                commandSeq1();
                break;
            case KBD_2:
                commandSeq2();
                break;
            case KBD_3:
                commandSeq3();
                break;
            case KBD_4:
                commandSeq4();
                break;
            case KBD_5:
                commandSeq5();
                break;
            case KBD_NEXT:
                commandNext();
                break;
            case KBD_START:
                if(!globConf.getClockIn())
                    commandStart();
                break;
            case KBD_PAUSE:
                if(!globConf.getClockIn())
                    commandStop();
                break;
            case KBD_PREVIOUS:
                commandPrevious();
                break;
            case KBD_RECORD:
                commandRecord();
                break;
            case KBD_GLOBAL:
                commandGlobalEdit();
                break;
            case KBD_SEQUENCE:
                commandSequenceEdit();
                break;
            case KBD_REST:
                commandRest();
                break;
            case KBD_TIE:
                commandTie();
                break;
            case KBD_MULTI_USAGE:
                commandMultiUsage();
                break;
            default:
                break;
        }
}

void updateDisplay()
{
    if(displayCallback)
        displayCallback();
}

void UpdateLeds()
{
    // first let's clear all leds
    leds.clearLeds();
    
    YASS_SEQUENCE* seq = player.getCurrentSequence();
    byte note_index = player.getNoteIndex();
    byte seq_index = player.getCurrentSequenceIndex();
    byte next_seq_index = player.getNextSequenceIndex();
    byte note = seq->getNote(player.getNoteIndex());
    
    // then eventually switch on some of them
    
    // step leds
    if(globEditFlag)
        leds.stepLedOn(globEditIndex % 16);
    else if(seqEditFlag)
        leds.stepLedOn(seqEditIndex % 16);
    else
        leds.stepLedOn(note_index);
    if(player.isRecording()) 
        if(millis() & 0x80)
            leds.stepLedOn(player.getRecordIndex());

    // multi usage led
    if(player.isRecording() & editDataFlag)
        leds.ledOn(LED_MULTI_USAGE);

    // sequence leds
    leds.ledOn(pgm_read_byte(sequenceLedLut + seq_index));
    if(next_seq_index != seq_index)
        if(millis() & 0x80)
            leds.ledOn(pgm_read_byte(sequenceLedLut + next_seq_index));
    
    // edition leds
    if(seqEditFlag)
        leds.ledOn(LED_SEQUENCE);
    if(globEditFlag)
        leds.ledOn(LED_GLOBAL);
    
    // transport leds   
    switch(yassState)
    {
        case stopped:
            break;
        case paused:
            if(millis() & 0x100)
                leds.ledOn(LED_START);
            break;
        case running:
            leds.ledOn(LED_START);
            break;
    }
    
    if(player.isRecording())
        leds.ledOn(LED_RECORD);
    else if(functionWaitsValidation())
        if(!(millis() & 0x80))
            leds.ledOn(LED_RECORD);
    
    // leds for note attributes (rest and tie)
    if (note == MIDI_TIE)
        leds.ledOn(LED_TIE);
    if (note == MIDI_REST)
        leds.ledOn(LED_REST);
}

void preTickTask()
{
    if(!globConf.getClockIn())
        tickTask();
}

void tickTask()
{
    player.tickTask();
}

void gpoTask()
{
    // this function sends bit after bit all outcoming data to shift registers
    
    if(displayMilestone <= millis())
    {
        // let's display the next digit for DISPLAY_MSEC_DURATION milliseconds
        displayMilestone += DISPLAY_MSEC_DURATION;
        digitIndex = (digitIndex + 1) & 3;
        *digit_number_ptr = ((*digit_number_ptr) & 0x0f) | (0x10 << digitIndex);
        *segments_ptr = displayBuf[digitIndex];
    }
    
    bool gpo_changed = false;
    for(byte x = 0;x < NB_GPO_DATA; x++)
    {
        if(oldGpoData[x] != gpoData[x])
        {
            gpo_changed = true;
            break;
        }
    }
    // does the job only if something has changed
    if(gpo_changed)
    {
        for(byte index = NB_GPO_DATA - 1;index != 255; index--)
        {
            if((index == GPO_SEGMENTS_INDEX) && (DISPLAY_COMMON_ANODE_SEGMENT_LOGIC == 0))
                shifter.write(gpoData[index] ^ 255);
            else
                shifter.write(gpoData[index]);
            oldGpoData[index] = gpoData[index];
        }    
        shifter.validate();
    }
}

/*****************************/
/* regular Arduino functions */
/*****************************/

void setup()
{
    beeper.begin();
    keyb.begin();
    
    editor.begin(ENC_A, ENC_B);
    setEditState(EDIT_STATE_TEMPO);
    
    shifter.begin();
    leds.begin();

    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.turnThruOff();
    MIDI.setHandleSystemExclusive(receiveExclusive);
    MIDI.setHandleNoteOn(receiveNoteOn);
    MIDI.setHandleNoteOff(receiveNoteOff);
    MIDI.setHandleControlChange(receiveControlChange);
    MIDI.setHandleClock(receiveClock);
    MIDI.setHandleStart(receiveStart);
    MIDI.setHandleStop(receiveStop);
    MIDI.setHandleContinue(receiveContinue);
    
    dotInMonostable.begin();
    dotInMonostable.bind(DOT_MONO_STABLE_DELAY_MSEC, dotInMonostableCallback);
    
    ticks.begin();
    ticks.setCallback(preTickTask);
    ticks.setTempo(DEFAULT_BPM);
    ticks.start();
    
    player.begin(NB_SEQS, seqs, YASS_SEQUENCE_NB_NOTES);
    player.setStartNoteCallback(playerNoteOn);
    player.setStopNoteCallback(playerNoteOff);
    player.setSendClockCallback(sendClock);
    player.setStartSequencerCallback(sendStart);
    player.setStopSequencerCallback(sendStop);
    player.setContinueSequencerCallback(sendContinue);
    yassState = stopped;

    storage.begin(&globConf, seqs);
    
    // special start mode?
    switch(keyb.getPicture())
    {
        case KBD_4_PICTURE_VALUE + KBD_5_PICTURE_VALUE:
            // informing we're on factory settings
            display.printLut(genericLut, LUT_INDEX_FCT0, NB_DIGITS);
            freezeDisplay(DISPLAY_FACTORY_SPLASH_DURATION);
            break;
        case KBD_1_PICTURE_VALUE + KBD_5_PICTURE_VALUE:
            YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_BILLIE_JEAN, &seqs[0]);
            YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_MIDNIGHT_EXPRESS, &seqs[1]);
            YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_BLADE_RUNNER, &seqs[2]);
            YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_OXYGENE_4, &seqs[3]);        
            YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_TUBULAR_BELLS, &seqs[4]);
            // informing we're on ROM1
            display.printLut(genericLut, LUT_INDEX_FCT1, NB_DIGITS);
            freezeDisplay(DISPLAY_FACTORY_SPLASH_DURATION);
            break;
        case KBD_2_PICTURE_VALUE + KBD_5_PICTURE_VALUE:
            YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_WOOGIE_BOOGIE, &seqs[0]);
            YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_MARCIA_BAILAR, &seqs[1]);
            YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_LATIN_BOLERO, &seqs[2]);
            YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_CLAVE, &seqs[3]);        
            YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_TAKE_FIVE, &seqs[4]);
            // informing we're on ROM2
            display.printLut(genericLut, LUT_INDEX_FCT2, NB_DIGITS);
            freezeDisplay(DISPLAY_FACTORY_SPLASH_DURATION);
            break;
        case KBD_3_PICTURE_VALUE + KBD_5_PICTURE_VALUE:
            YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_MINOR_ARPEGGIO, &seqs[0]);
            YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_MAJOR_ARPEGGIO, &seqs[1]);
            YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_DIM_ARPEGGIO, &seqs[2]);
            YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_PENTA_ARPEGGIO, &seqs[3]);        
            YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_AUG_ARPEGGIO, &seqs[4]);
            // informing we're on ROM3
            display.printLut(genericLut, LUT_INDEX_FCT3, NB_DIGITS);
            freezeDisplay(DISPLAY_FACTORY_SPLASH_DURATION);
            break;
        #ifdef INCLUDE_MAINTENANCE
        case KBD_SEQUENCE_PICTURE_VALUE + KBD_GLOBAL_PICTURE_VALUE:
            serialDebug = true;
            maintenance.begin(
                &editor,
                &keyb,
                &beeper,
                &globConf,
                &player,
                &display,
                &leds,
                &debug);
            // informing we're on debug mode
            display.printLut(genericLut, LUT_INDEX_DEBUG, NB_DIGITS);
            freezeDisplay(DISPLAY_FACTORY_SPLASH_DURATION);
            break;
        #endif
        case KBD_NO_KEY:
        default:
            // overwritting factory settings
            storage.loadAll();
            break;
    }
    
    sysEx.begin(&globConf, seqs);
    sysEx.setSenderCallback(sendExclusive);
    
    display.begin();
    // displays application name...    
    display.printLut(genericLut, LUT_INDEX_APP_NAME, NB_DIGITS);
    freezeDisplay(DISPLAY_FACTORY_SPLASH_DURATION);
    // ... and version
    display.printLut(genericLut, LUT_INDEX_APP_VERSION, NB_DIGITS);
    freezeDisplay(DISPLAY_FACTORY_SPLASH_DURATION);
}

void loop()
{
#ifdef INCLUDE_MAINTENANCE
    if(!serialDebug)
    {
#endif
        MIDI.read();
        dotInMonostable.sequencer();
        
        ticks.sequencer(); // if a tick occurs, player.tickTask runs one time

        keyb.sequencer();
        keybTask();
        
        editor.sequencer();
        
        beeper.sequencer();
        
        UpdateLeds();
        updateDisplay();
        beatCalc.sequencer();
        
        sysEx.sequencer();
        
        gpoTask();
#ifdef INCLUDE_MAINTENANCE
    }
    else
    {
        maintenance.sequencer();
        editor.getEncoder()->sequencer();
        beeper.sequencer();
        keyb.sequencer();
        gpoTask();
    }
#endif
}

