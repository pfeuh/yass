/*
 * file : yassSequencer.cpp
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

#include "yassSequencer.h"

#define YASS_STATE_STOPPED 1
#define YASS_STATE_RUNNING 2
#define YASS_STATE_RECORDING 3

/*******************/
/* Private methods */
/*******************/

byte YASS_SEQUENCER::getNextNote()
{
    YASS_SEQUENCE* temp_seq = currentSeq;
    byte temp_index = index + 1;
    if(temp_index > currentSeq->getLastStep())
    {
        temp_index = 0;
        temp_seq = nextSeq;
    }
    return temp_seq->getNote(temp_index);
}

void YASS_SEQUENCER::noteOn(byte note, byte velocity)
{
    if(startCallback)
        startCallback(note, velocity);
}

void YASS_SEQUENCER::noteOff(byte note)
{
    if(stopCallback)
        stopCallback(note);
}

void YASS_SEQUENCER::startNote()
{
    currentNote = currentSeq->getNote(index);
    if((currentNote != YASS_SEQUENCE_REST) && (currentNote != YASS_SEQUENCE_TIE))
        currentNote = (currentNote + scale - currentSeq->getFirstNote()) & 0x7f;
    if(currentNote)
        noteOn(currentNote, currentSeq->getData(index));
}

void YASS_SEQUENCER::stepTask()
{
    byte temp_index = index + 1;
    if(temp_index > currentSeq->getLastStep())
    {
        temp_index = 0;
        currentSeq = nextSeq;
        currentSeqIndex = getNextSequenceIndex();
        scale = nextScale;
    }
    if (currentSeq->getNote(temp_index) != YASS_SEQUENCE_TIE)
    {
        index = temp_index;
        startNote();
    }
    else
        index = temp_index;

}

/*
+-----------------------------------+
|          | tick |    tick stop    |
|Groove    | start|short normal long|
|----------+------+------+-----+----|
|1/4       |      |      |     |    |
|          |   0  |  12  | 17  | 22 |
|----------+------+------+-----+----|
|1/8       |      |      |     |    |
|          |   0  |   6  |  8  | 11 |
|          |  12  |  18  | 20  | 23 |
|----------+------+------+-----+----|
|1/12      |      |      |     |    |
|          |   0  |   4  |  6  |  7 |
|          |   8  |  12  | 14  | 15 |
|          |  16  |  20  | 22  | 23 |
|----------+------+------+-----+----|
|1/8 swing |      |      |     |    |
|          |   0  |   8  | 11  | 14 |
|          |  16  |  20  | 22  | 23 |
|----------+------+------+-----+----|
|1/16      |      |      |     |    |
|          |   0  |   3  |  4  |  5 |
|          |   6  |   9  | 10  | 11 |
|          |  12  |  15  | 16  | 17 |
|          |  18  |  21  | 22  | 23 |
|----------+------+------+-----+----|
|1/24      |      |      |     |    |
|          |   0  |   2  |  3  |  3 |
|          |   4  |   6  |  7  |  7 |
|          |   8  |  10  | 11  | 11 |
|          |  12  |  14  | 15  | 15 |
|          |  16  |  18  | 19  | 19 |
|          |  20  |  22  | 23  | 23 |
|----------+------+------+-----+----|
|1/24 swing|      |      |     |    |
|          |   0  |   4  |  6  |  7 |
|          |   8  |  10  | 11  | 11 |
|          |  12  |  16  | 18  | 19 |
|          |  20  |  22  | 23  | 23 |
|----------+------+------+-----+----|
*/

void YASS_SEQUENCER::stateNoire()
{
    // This code is generated by tools/makeGrooves.py
    switch(currentSeq->getGateMode())
    {
        case YASS_SEQUENCE_GATE_SHORT:
            switch(tickNum)
            {
                case 12:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        case YASS_SEQUENCE_GATE_NORMAL:
            switch(tickNum)
            {
                case 17:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        case YASS_SEQUENCE_GATE_LONG:
            switch(tickNum)
            {
                case 22:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    
    switch(tickNum)
    {
        case 0:
            stepTask();
            break;
        default:
            break;
    }
}

void YASS_SEQUENCER::stateCroche()
{
    // This code is generated by tools/makeGrooves.py
    switch(currentSeq->getGateMode())
    {
        case YASS_SEQUENCE_GATE_SHORT:
            switch(tickNum)
            {
                case 6:
                case 18:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        case YASS_SEQUENCE_GATE_NORMAL:
            switch(tickNum)
            {
                case 8:
                case 20:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        case YASS_SEQUENCE_GATE_LONG:
            switch(tickNum)
            {
                case 11:
                case 23:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    
    switch(tickNum)
    {
        case 0:
        case 12:
            stepTask();
            break;
        default:
            break;
    }
}

void YASS_SEQUENCER::stateTrioletCroche()
{
    // This code is generated by tools/makeGrooves.py
    switch(currentSeq->getGateMode())
    {
        case YASS_SEQUENCE_GATE_SHORT:
            switch(tickNum)
            {
                case 4:
                case 12:
                case 20:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        case YASS_SEQUENCE_GATE_NORMAL:
            switch(tickNum)
            {
                case 6:
                case 14:
                case 22:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        case YASS_SEQUENCE_GATE_LONG:
            switch(tickNum)
            {
                case 7:
                case 15:
                case 23:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    
    switch(tickNum)
    {
        case 0:
        case 8:
        case 16:
            stepTask();
            break;
        default:
            break;
    }
}

void YASS_SEQUENCER::stateCrocheSwing()
{
    // This code is generated by tools/makeGrooves.py
    switch(currentSeq->getGateMode())
    {
        case YASS_SEQUENCE_GATE_SHORT:
            switch(tickNum)
            {
                case 8:
                case 20:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        case YASS_SEQUENCE_GATE_NORMAL:
            switch(tickNum)
            {
                case 11:
                case 22:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        case YASS_SEQUENCE_GATE_LONG:
            switch(tickNum)
            {
                case 14:
                case 23:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    
    switch(tickNum)
    {
        case 0:
        case 16:
            stepTask();
            break;
        default:
            break;
    }
}

void YASS_SEQUENCER::stateDoubleCroche()
{
    // This code is generated by tools/makeGrooves.py
    switch(currentSeq->getGateMode())
    {
        case YASS_SEQUENCE_GATE_SHORT:
            switch(tickNum)
            {
                case 3:
                case 9:
                case 15:
                case 21:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        case YASS_SEQUENCE_GATE_NORMAL:
            switch(tickNum)
            {
                case 4:
                case 10:
                case 16:
                case 22:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        case YASS_SEQUENCE_GATE_LONG:
            switch(tickNum)
            {
                case 5:
                case 11:
                case 17:
                case 23:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    
    switch(tickNum)
    {
        case 0:
        case 6:
        case 12:
        case 18:
            stepTask();
            break;
        default:
            break;
    }
}

void YASS_SEQUENCER::stateTrioletDoubleCroche()
{
    // This code is generated by tools/makeGrooves.py
    switch(currentSeq->getGateMode())
    {
        case YASS_SEQUENCE_GATE_SHORT:
            switch(tickNum)
            {
                case 2:
                case 6:
                case 10:
                case 14:
                case 18:
                case 22:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        case YASS_SEQUENCE_GATE_NORMAL:
            switch(tickNum)
            {
                case 3:
                case 7:
                case 11:
                case 15:
                case 19:
                case 23:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        case YASS_SEQUENCE_GATE_LONG:
            switch(tickNum)
            {
                case 3:
                case 7:
                case 11:
                case 15:
                case 19:
                case 23:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    
    switch(tickNum)
    {
        case 0:
        case 4:
        case 8:
        case 12:
        case 16:
        case 20:
            stepTask();
            break;
        default:
            break;
    }
}

void YASS_SEQUENCER::stateDoubleCrocheSwing()
{
    // This code is generated by tools/makeGrooves.py
    switch(currentSeq->getGateMode())
    {
        case YASS_SEQUENCE_GATE_SHORT:
            switch(tickNum)
            {
                case 4:
                case 10:
                case 16:
                case 22:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        case YASS_SEQUENCE_GATE_NORMAL:
            switch(tickNum)
            {
                case 6:
                case 11:
                case 18:
                case 23:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        case YASS_SEQUENCE_GATE_LONG:
            switch(tickNum)
            {
                case 7:
                case 11:
                case 19:
                case 23:
                    stopCurrentNote();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    
    switch(tickNum)
    {
        case 0:
        case 8:
        case 12:
        case 20:
            stepTask();
            break;
        default:
            break;
    }
}

/******************/
/* Public methods */
/******************/

YASS_SEQUENCER::YASS_SEQUENCER()
{
}

void YASS_SEQUENCER::begin(byte nb_seqs, YASS_SEQUENCE* _seqs, byte max_notes)
{
    nbSeqs = nb_seqs;
    seqs = _seqs;
    maxNotes = max_notes;
    scale = 0;
    nextScale = 0;
    nextSeq = &seqs[0];
    currentSeq = &seqs[0];
    startCallback = 0;
    stopCallback = 0;
    clockCallback = 0;
    startSequencerCallback = 0;
    stopSequencerCallback = 0;
    continueSequencerCallback = 0;
    scale = YASS_SEQUENCER_MIDI_C3;
    nextScale = YASS_SEQUENCER_MIDI_C3;
    stopSequencer();
}

byte YASS_SEQUENCER::getNbSequences()
{
    return nbSeqs;
}

void YASS_SEQUENCER::setNextSequenceIndex(byte seq_num)
{
    if(seq_num < nbSeqs)
    {
        nextSeq = &seqs[seq_num];
        nextSeqIndex = seq_num;
        //~ currentSeq = nextSeq;
        //~ currentSeqIndex = nextSeqIndex;       
    }
}

YASS_SEQUENCE* YASS_SEQUENCER::getNextSequence()
{
    return nextSeq;
}

byte YASS_SEQUENCER::getNextSequenceIndex()
{
    return nextSeqIndex;
}

void YASS_SEQUENCER::setNextScale(byte next_scale)
{
        nextScale = next_scale;
        //~ nextScale = next_scale % 12;
}

byte YASS_SEQUENCER::getNextScale()
{
    return nextScale;
}

YASS_SEQUENCE* YASS_SEQUENCER::getCurrentSequence()
{
    return currentSeq;
}

YASS_SEQUENCE* YASS_SEQUENCER::getSequence(byte index)
{
    return &seqs[index];
}

byte YASS_SEQUENCER::getCurrentSequenceIndex()
{
    return currentSeqIndex;
}

void YASS_SEQUENCER::setStartNoteCallback(void (*start_callback)(byte note, byte velocity))
{
    startCallback = start_callback;
}

void YASS_SEQUENCER::setStopNoteCallback(void (*stop_callback)(byte note))
{
    stopCallback = stop_callback;
}

void YASS_SEQUENCER::setSendClockCallback(void (*clock_callback)(byte tick_num))
{
    clockCallback = clock_callback;
}

void YASS_SEQUENCER::setStartSequencerCallback(void (*start_sequencer_callback)())
{
    startSequencerCallback = start_sequencer_callback;
}

void YASS_SEQUENCER::setStopSequencerCallback(void (*stop_sequencer_callback)())
{
    stopSequencerCallback = stop_sequencer_callback;
}

void YASS_SEQUENCER::setContinueSequencerCallback(void (*continue_sequencer_callback)())
{
    continueSequencerCallback = continue_sequencer_callback;
}

void YASS_SEQUENCER::startSequencer()
{
    state = YASS_STATE_RUNNING;
    currentSeq = nextSeq;
    tickNum = YASS_RESOLUTION - 1;
    currentNote = 0;
    index = maxNotes - 1;
    if(startSequencerCallback)
        startSequencerCallback();
}

void YASS_SEQUENCER::stopSequencer()
{
    noteOff(currentNote);
    state = YASS_STATE_STOPPED;
    if(stopSequencerCallback)
        stopSequencerCallback();

}

void YASS_SEQUENCER::continueSequencer()
{
    state = YASS_STATE_RUNNING;
    if(continueSequencerCallback)
        continueSequencerCallback();
}

void YASS_SEQUENCER::recordSequencer()
{
    recordFlag = true;
    recordIsStarted = false;
    recordIndex = 0;
}

void YASS_SEQUENCER::stopRecordSequencer()
{
    recordFlag = false;
}

bool YASS_SEQUENCER::isRunning()
{
    return state == YASS_STATE_RUNNING;
}

bool YASS_SEQUENCER::isRecording()
{
    return recordFlag;
}

bool YASS_SEQUENCER::isStopped()
{
    return state == YASS_STATE_STOPPED;
}

byte YASS_SEQUENCER::getNbNotes(byte seq_num)
{
    return seqs[seq_num].getLastStep() + 1;
}

void YASS_SEQUENCER::setNbNotes(byte seq_num, byte nb_notes)
{
    seqs[seq_num].setLastStep(nb_notes - 1);
}

void YASS_SEQUENCER::tickTask()
{
    if(!isRunning())
        return;
    
    tickNum = (tickNum + 1) % YASS_RESOLUTION;

    if(clockCallback)
        clockCallback(tickNum);
    
    switch(currentSeq->getGroove())
    {
        case YASS_SEQUENCE_NOIRE:
            stateNoire();
            break;
        case YASS_SEQUENCE_CROCHE:
            stateCroche();
            break;
        case YASS_SEQUENCE_TRIOLET_CROCHE:
            stateTrioletCroche();
            break;
        case YASS_SEQUENCE_CROCHE_SWING:
            stateCrocheSwing();
            break;
        case YASS_SEQUENCE_DOUBLE_CROCHE:
            stateDoubleCroche();
            break;
        case YASS_SEQUENCE_TRIOLET_DOUBLE_CROCHE:
            stateTrioletDoubleCroche();
            break;
        case YASS_SEQUENCE_DOUBLE_CROCHE_SWING:
            stateDoubleCrocheSwing();
            break;
        default:
            break;
    }
    oldTickNum = tickNum;
}

byte YASS_SEQUENCER::getNoteIndex()
{
    return index;
}

void YASS_SEQUENCER::setNoteIndex(byte _index)
{
    index = _index % maxNotes;
}

void YASS_SEQUENCER::stopCurrentNote()
{
    if(getNextNote() != YASS_SEQUENCE_TIE)
        if(currentNote)
            noteOff(currentNote);
}

void YASS_SEQUENCER::addRecordedNote(byte note, byte velocity)
{
    if(isRecording())
    {
        if(recordIsStarted)
            recordIndex = (recordIndex + 1) % YASS_SEQUENCE_NB_NOTES;
        recordIsStarted = true;
        
        //~ if(index >= maxNotes)
            //~ if(stopRecordingCallback)
                //~ stopRecordingCallback();

        currentSeq->setNote(recordIndex, note);
        currentSeq->setData(recordIndex, velocity);
    }
}

void YASS_SEQUENCER::editRecordedData(byte velocity)
{
    if(isRecording())
        currentSeq->setData(recordIndex, velocity);
}

void YASS_SEQUENCER::gotoNextRecordStep()
{
    recordIndex = (recordIndex + 1) % YASS_SEQUENCE_NB_NOTES;
    recordIsStarted = false;
}

byte YASS_SEQUENCER::getRecordIndex()
{
    return recordIndex;
}
