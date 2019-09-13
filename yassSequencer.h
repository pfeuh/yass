#ifndef yassSequencer_h
#define yassSequencer_h

/*
 * file : yassSequencer.h
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
#include "yassSequence.h"

#define YASS_SEQUENCER_VERSION "1.00"

#define YASS_RESOLUTION 24
#define YASS_SEQUENCER_MIDI_C3 48

class YASS_SEQUENCER
{
    public:
        YASS_SEQUENCER();
        void begin(byte nb_seqs, YASS_SEQUENCE* _seqs, byte max_notes);
        byte getNbSequences();
        void initSequence(byte seq_num);
        void setNextSequenceIndex(byte seq_num);
        void setCurrentSequenceIndex(byte seq_num);
        YASS_SEQUENCE* getNextSequence();
        byte getNextSequenceIndex();
        void setNextScale(byte next_scale);
        byte getNextScale();
        YASS_SEQUENCE* getCurrentSequence();
        YASS_SEQUENCE* getSequence(byte index);
        byte getCurrentSequenceIndex();
        void tickTask();
        void startSequencer();
        void stopSequencer();
        void continueSequencer();
        void startRecordSequencer();
        void stopRecordSequencer();
        bool isRunning();
        bool isStopped();
        bool isRecording();
        byte getNbNotes(byte seq_num);
        void setNbNotes(byte seq_num, byte groove);
        void noteOn(byte note, byte velocity);
        void noteOff(byte note);
        void setStartNoteCallback(void (*start_callback)(byte note, byte velocity));
        void setStopNoteCallback(void (*stop_callback)(byte note));
        void setSendClockCallback(void (*clock_callback)(byte tick_num));
        void setNewStepCallback(void (*new_step_callback)());
        void setNewSequenceCallback(void (*new_sequence_callback)());
        void setStopRecordingCallback(void (*stop_recording_callback)());
        void setStartSequencerCallback(void (*start_sequencer_callback)());
        void setStopSequencerCallback(void (*stop_sequencer_callback)());
        void setContinueSequencerCallback(void (*continue_sequencer_callback)());
        byte getNoteIndex();
        void setNoteIndex(byte _index);
        void stopCurrentNote();
        void addRecordedNote(byte note, byte velocity);
        void gotoNextRecordStep();
        void gotoPreviousRecordStep();
        void editRecordedData(byte velocity);
        byte getRecordIndex();

    private:
        byte state;
        byte nbSeqs;
        YASS_SEQUENCE* seqs;
        YASS_SEQUENCE* nextSeq;
        byte nextSeqIndex;
        YASS_SEQUENCE* currentSeq;
        byte currentSeqIndex;
        byte maxNotes;
        byte tickNum;
        byte oldTickNum;
        byte index;
        byte currentNote;
        byte scale;
        byte nextScale;
        byte recordIndex;
        bool recordIsStarted;
        bool recordFlag;
    
        void (*startCallback)(byte note, byte velocity);
        void (*stopCallback)(byte note);
        void (*clockCallback)(byte tick_num);
        void (*startSequencerCallback)();
        void (*stopSequencerCallback)();
        void (*continueSequencerCallback)();

        byte getNextNote();
        void startNote();
        void stepTask();

        void stateNoire();
        void stateCroche();
        void stateTrioletCroche();
        void stateCrocheSwing();
        void stateDoubleCroche();
        void stateTrioletDoubleCroche();
        void stateDoubleCrocheSwing();

};

#endif

