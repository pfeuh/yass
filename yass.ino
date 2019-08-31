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

#define YASS_VERSION 0x100

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

// debug tool if needed
//~ #define YASS_DEBUG
#ifdef YASS_DEBUG
#include <arduinoDebug.h>
ARDUINO_DEBUG debug = ARDUINO_DEBUG();
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

void freezeDisplay(word msec_value)
{
    unsigned long int milestone = millis() + msec_value;
    while(milestone >= millis())
    {
        updateDisplay();
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

void setEditNotImplemented()
{
    editor.setState(YASS_CONF_FSM_STATE_TEMPO, &dummyEdit);
}

void setEditTempo()
{
    editor.setState(YASS_CONF_FSM_STATE_TEMPO, &editTempo);
}

bool functionWaitsValidation()
{
    bool ret_val;
    switch(editor.getState())
    {
        case YASS_CONF_FSM_GLOB_DUMP:
        case YASS_CONF_FSM_GLOB_LOAD:
        case YASS_CONF_FSM_GLOB_SAVE:
        case YASS_CONF_FSM_COPY:
        case YASS_CONF_FSM_SWAP:
        case YASS_CONF_FSM_SEQ_LOAD_FACTORY:
        case YASS_CONF_FSM_SEQ_LOAD:
        case YASS_CONF_FSM_SEQ_SAVE:
        case YASS_CONF_FSM_SEQ_DUMP:
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
        MIDI.sendNoteOff(number, value, globConf.getChannelOut());
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
        if(!player.isRunning())
        {
            commandStart();
            beatCalc.start();
        }
}

void receiveStop()
{
    if(globConf.getClockIn())
        if(player.isRunning())
        {
            beatCalc.stop();
            commandStop();
        }
}

void receiveContinue()
{
    if(globConf.getClockIn())
        if(!player.isRunning())
        {
            commandContinue();
            beatCalc.start();
        }

}
/***********************/
/* encoder's callbacks */
/***********************/

void dummyEdit(bool direction)
{
    direction++; // just to fake the compiler
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

void editMidiInChannel(bool direction)
{
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

void editMidiOutChannel(bool direction)
{
    player.stopCurrentNote();
    if(direction == YASS_ENCODER_DIRECTION_UP)
        globConf.setChannelOut(globConf.getChannelOut() + 1);
    else
        globConf.setChannelOut(globConf.getChannelOut() - 1);
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

void editArpeggiator(bool direction)
{
    if(direction == YASS_ENCODER_DIRECTION_UP)
        globConf.setArpeggiator(1);
    else
        globConf.setArpeggiator(0);
}

void editGroove(bool direction)
{
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

void editProgNum(bool direction)
{
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
    
void editDataMode(bool direction)
{
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

void setEditParam(byte state)
{
    switch(state)
    {
        case YASS_CONF_FSM_MIDI_IN_CHANNEL:
            editor.setState(YASS_CONF_FSM_MIDI_IN_CHANNEL, &editMidiInChannel);
            break;
        case YASS_CONF_FSM_MIDI_OUT_CHANNEL:
            editor.setState(YASS_CONF_FSM_MIDI_OUT_CHANNEL, &editMidiOutChannel);
            break;
        case YASS_CONF_FSM_CLOCK_IN:
            editor.setState(YASS_CONF_FSM_CLOCK_IN, &editClockIn);
            break;
        case YASS_CONF_FSM_CLOCK_OUT:
            editor.setState(YASS_CONF_FSM_CLOCK_OUT, &editClockOut);
            break;
        case YASS_CONF_FSM_KEY_ECHO:
            editor.setState(YASS_CONF_FSM_KEY_ECHO, &editKeyEcho);
            break;
        case YASS_CONF_FSM_CLICK:
            editor.setState(YASS_CONF_FSM_CLICK, &editClick);
            break;
        case YASS_CONF_FSM_PROGRAM_NUMBER:
            editor.setState(YASS_CONF_FSM_PROGRAM_NUMBER, &editProgNum);
            break;
        case YASS_CONF_FSM_ARPEGGIATOR:
            editor.setState(YASS_CONF_FSM_PROGRAM_NUMBER, &editArpeggiator);
            break;
        case YASS_CONF_FSM_GLOB_DUMP:
            editor.setState(YASS_CONF_FSM_GLOB_DUMP, &dummyEdit);
            break;
        case YASS_CONF_FSM_GLOB_LOAD:
            editor.setState(YASS_CONF_FSM_GLOB_LOAD, &dummyEdit);
            break;
        case YASS_CONF_FSM_GLOB_SAVE:
            editor.setState(YASS_CONF_FSM_GLOB_SAVE, &dummyEdit);
            break;
        case YASS_CONF_FSM_USE_SYSEX:
            editor.setState(YASS_CONF_FSM_USE_SYSEX, &editSysEx);
            break;
        case YASS_CONF_FSM_GROOVE:
            editor.setState(YASS_CONF_FSM_GROOVE, &editGroove);
            break;
        case YASS_CONF_FSM_GATE_MODE:
            editor.setState(YASS_CONF_FSM_GATE_MODE, &editGateMode);
            break;
        case YASS_CONF_FSM_LAST_STEP:
            editor.setState(YASS_CONF_FSM_LAST_STEP, &editLastStep);
            break;
        case YASS_CONF_FSM_DATA_MODE:
            editor.setState(YASS_CONF_FSM_LAST_STEP, &editDataMode);
            break;
        case YASS_CONF_FSM_CTRL_CHG:
            editor.setState(YASS_CONF_FSM_CTRL_CHG, &editCtrlChg);
            break;
        case YASS_CONF_FSM_FIX_VEL:
            editor.setState(YASS_CONF_FSM_FIX_VEL, &editFixedVelocity);
            break;
        case YASS_CONF_FSM_COPY:
            editor.setState(YASS_CONF_FSM_COPY, &editCopySeq);
            break;
        case YASS_CONF_FSM_SWAP:
            editor.setState(YASS_CONF_FSM_SWAP, &editSwapSeq);
            break;
        case YASS_CONF_FSM_SEQ_LOAD_FACTORY:
            editor.setState(YASS_CONF_FSM_SEQ_LOAD_FACTORY, &editLoadFactory);
            break;
        case YASS_CONF_FSM_SEQ_DUMP:
            editor.setState(YASS_CONF_FSM_SEQ_DUMP, &dummyEdit);
            break;
        case YASS_CONF_FSM_SEQ_LOAD:
            editor.setState(YASS_CONF_FSM_SEQ_LOAD, &dummyEdit);
            break;
        case YASS_CONF_FSM_SEQ_SAVE:
            editor.setState(YASS_CONF_FSM_SEQ_SAVE, &dummyEdit);
            break;
        default:
            break;
    }
}

void setNextGlobalEditParam()
{
    if(globEditIndex >= YASS_CONF_FSM_LAST_GLOBAL)
        globEditIndex = YASS_CONF_FSM_FIRST_GLOBAL;
    else if(globEditIndex == YASS_CONF_FSM_USE_SYSEX)
        globEditIndex = YASS_CONF_FSM_GLOB_DUMP;
    else
        globEditIndex += 1;
    setEditParam(globEditIndex);
}

void setNextSeqEditParam()
{
    if(seqEditIndex >= YASS_CONF_FSM_LAST_SEQ)
        seqEditIndex = YASS_CONF_FSM_FIRST_SEQ;
    else if(seqEditIndex == YASS_CONF_FSM_FIX_VEL)
        seqEditIndex = YASS_CONF_FSM_SEQ_DUMP;
    else
        seqEditIndex += 1;
    setEditParam(seqEditIndex);
}

/****************/
/* HMI commands */
/****************/

void commandStart()
{
    if(player.isStopped())
    {
        changeProgramm();
        player.startSequencer();
        keyBeep();
    }
}

void commandStop()
{
    if(!player.isStopped())
    {
        player.stopSequencer();
        if(!player.isRecording())
            setEditTempo();
        keyBeep();
    }
}

void commandContinue()
{
    if(player.isStopped())
    {
        player.continueSequencer();
        keyBeep();
    }
}

void commandRecord()
{
    if(functionWaitsValidation())
    {
        switch(editor.getState())
        {
            case YASS_CONF_FSM_GLOB_DUMP:
                sysEx.sendGlobal();
                break;
            case YASS_CONF_FSM_GLOB_LOAD:
                storage.loadAll();
                break;
            case YASS_CONF_FSM_GLOB_SAVE:
                storage.saveAll();
                break;
            case YASS_CONF_FSM_SEQ_LOAD:
                storage.loadSequence(player.getCurrentSequenceIndex());
                break;
            case YASS_CONF_FSM_SEQ_SAVE:
                storage.saveSequence(player.getCurrentSequenceIndex());
                break;
            case YASS_CONF_FSM_COPY:
                memcpy(player.getSequence(copySeqIndex)->getDataPointer(), player.getCurrentSequence()->getDataPointer(), YASS_SEQUENCE_DATA_SIZE);
                break;
            case YASS_CONF_FSM_SWAP:
                swap(player.getSequence(swapSeqIndex)->getDataPointer(), player.getCurrentSequence()->getDataPointer(), YASS_SEQUENCE_DATA_SIZE);
                break;
            case YASS_CONF_FSM_SEQ_LOAD_FACTORY:
                YASS_ROM_SEQUENCES_load(romSequenceIndex, player.getCurrentSequence());
                break;
            case YASS_CONF_FSM_SEQ_DUMP:
                sysEx.sendSequence(player.getCurrentSequenceIndex());
                break;
            default:
                break;
        }
        exitEdit();
    }        
    else if(!(globEditFlag |globEditFlag))
    {
        if(player.isRecording())
        {
            player.stopRecordSequencer();
            setEditTempo();
        }
        else
        {
            player.recordSequencer();
            editor.setState(YASS_CONF_FSM_STATE_RECORD_EDIT, &editRecord);
        }
        keyBeep();
    }
}

void commandSeqNum(byte seq_num)
{
    if(!player.isRecording())
    {
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
            setEditParam(seqEditIndex);        
        }
        else
        {
            // exiting sequence edition mode
            setEditTempo();
        }
        keyBeep();
    }
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
            setEditParam(globEditIndex);        
        }
        else
        {
            // exiting global edition mode
            setEditTempo();
        }
        keyBeep();
    }
}

void commandNext()
{
    if(globEditFlag)
    {
        setNextGlobalEditParam();
        keyBeep();
    }
    else if(seqEditFlag)
    {
        setNextSeqEditParam();
        keyBeep();
    }
    else if(editor.getState() == YASS_CONF_FSM_STATE_TEMPO)
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

void startDotIn()
{
    display.writeDot(1, MIDI_IN_DOT);
    dotInMonostable.start();
}

void dotInMonostableCallback()
{
    display.writeDot(0, MIDI_IN_DOT);
}

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
            case KBD_CONTINUE:
                if(!globConf.getClockIn())
                    commandContinue();
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
    YASS_SEQUENCE* seq = player.getCurrentSequence();
    byte record_index = player.getRecordIndex();

    if(player.isRecording())
    {
        if(editDataFlag)
            display.printWord(seq->getData(record_index), DEFAULT_BASE);
        else
            display.printLut(noteLabelLut, seq->getNote(record_index), NB_DIGITS);
    }
    
    else if(globEditFlag)
        switch(globEditIndex)
        {
            case YASS_CONF_FSM_MIDI_IN_CHANNEL:
                if(globConf.getOmni())
                    display.printLut(genericLut, LUT_INDEX_OMNI, NB_DIGITS);
                else
                    display.printWord(globConf.getChannelIn(), DEFAULT_BASE);
                break;
            case YASS_CONF_FSM_MIDI_OUT_CHANNEL:
                display.printWord(globConf.getChannelOut(), DEFAULT_BASE);
                break;
            case YASS_CONF_FSM_CLOCK_IN:
                display.printLut(boolLut, globConf.getClockIn(), NB_DIGITS);
                break;
            case YASS_CONF_FSM_CLOCK_OUT:
                display.printLut(boolLut, globConf.getClockOut(), NB_DIGITS);
                break;
            case YASS_CONF_FSM_KEY_ECHO:
                display.printLut(boolLut, globConf.getKeyEcho(), NB_DIGITS);
                break;
            case YASS_CONF_FSM_CLICK:
                display.printLut(boolLut, globConf.getClick(), NB_DIGITS);
                break;
            case YASS_CONF_FSM_USE_SYSEX:
                display.printLut(boolLut, globConf.getUseSysEx(), NB_DIGITS);
                break;
            case YASS_CONF_FSM_ARPEGGIATOR:
                display.printLut(sqArpLut, globConf.getArpeggiator(), NB_DIGITS);
                break;
            case YASS_CONF_FSM_PROGRAM_NUMBER:
                if(globConf.getUseProgNum())
                    display.printLut(voiceLabelLut, globConf.getProgNum(), NB_DIGITS);
                else
                    display.printLut(genericLut, LUT_INDEX_NONE, NB_DIGITS);
                break;
            case YASS_CONF_FSM_GLOB_DUMP:
                display.printLut(genericLut, LUT_INDEX_DUMP, NB_DIGITS);
                break;
            case YASS_CONF_FSM_GLOB_LOAD:
                display.printLut(genericLut, LUT_INDEX_LOAD, NB_DIGITS);
                break;
            case YASS_CONF_FSM_GLOB_SAVE:
                display.printLut(genericLut, LUT_INDEX_STOR, NB_DIGITS);
                break;
            default:
                display.printLut(genericLut, LUT_INDEX_NOTI, NB_DIGITS);
                break;
        }
        
    else if(seqEditFlag)
        switch(seqEditIndex)
        {
            case YASS_CONF_FSM_GROOVE:
                display.printLut(grooveLut, seq->getGroove(), NB_DIGITS);
                break;
            case YASS_CONF_FSM_GATE_MODE:
                display.printLut(gateModeLut, seq->getGateMode(), NB_DIGITS);
                break;
            case YASS_CONF_FSM_LAST_STEP:
                display.printWord(seq->getLastStep() + 1, DEFAULT_BASE);
                break;
            case YASS_CONF_FSM_DATA_MODE:
                display.printLut(dataModeLut, seq->getDataMode(), NB_DIGITS);
                break;
            case YASS_CONF_FSM_CTRL_CHG:
                display.printWord(seq->getControlChange(), DEFAULT_BASE);
                break;
            case YASS_CONF_FSM_FIX_VEL:
                display.printWord(seq->getFixedVelocity(), DEFAULT_BASE);
                break;
            case YASS_CONF_FSM_SEQ_LOAD_FACTORY:
                display.printWord(romSequenceIndex, DEFAULT_BASE);
                break;
            case YASS_CONF_FSM_COPY:
                display.printWord(copySeqIndex + 1, DEFAULT_BASE);
                break;
            case YASS_CONF_FSM_SWAP:
                display.printWord(swapSeqIndex + 1, DEFAULT_BASE);
                break;
            case YASS_CONF_FSM_SEQ_DUMP:
                display.printLut(genericLut, LUT_INDEX_DUMP, NB_DIGITS);
                break;
            case YASS_CONF_FSM_SEQ_LOAD:
                display.printLut(genericLut, LUT_INDEX_LOAD, NB_DIGITS);
                break;
            case YASS_CONF_FSM_SEQ_SAVE:
                display.printLut(genericLut, LUT_INDEX_STOR, NB_DIGITS);
                break;
            default:
                display.printLut(genericLut, LUT_INDEX_NOTI, NB_DIGITS);
                break;
        }
        
    else
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
    if(player.isRunning())
        leds.ledOn(LED_START);
    if(player.isRecording())
        leds.ledOn(LED_RECORD);
    if(functionWaitsValidation())
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
    setEditTempo();
    
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

    //~ YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_BILLIE_JEAN, &seqs[0]);
    //~ YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_PAN_TEST, &seqs[0]);
    
    //~ YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_MIDNIGHT_EXPRESS, &seqs[1]);
    //~ YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_BLADE_RUNNER, &seqs[2]);
    //~ YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_OXYGENE_4, &seqs[3]);
    //~ YASS_ROM_SEQUENCES_load(YASS_ROM_SEQUENCE_TUBULAR_BELLS, &seqs[4]);

    #ifdef YASS_DEBUG
    debug.begin();
    word keys = keyb.getPicture();
    console.println(keys, HEX);
    #endif

    storage.begin(&globConf, seqs, &ticks);
    if(!(KBD_1_PICTURE_VALUE & keyb.getPicture()))
        // overwritting factory settings
        storage.loadAll();
    else
    {
        // informing we're on factory settings
        display.printLut(genericLut, LUT_INDEX_FACT, NB_DIGITS);
        freezeDisplay(DISPLAY_FACTORY_SPLASH_DURATION);
    }
    
    sysEx.begin(&globConf, seqs);
    sysEx.setSenderCallback(sendExclusive);
    
    //~ commandStop();
    //~ commandSeq1();

    display.begin();
}

void loop()
{
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
    
    gpoTask();
}
