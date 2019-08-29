#ifndef yassTicks_h
#define yassTicks_h

/*
 * file : yassTicks.h
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

#define YASS_TICKS_VERSION      "1.00"
#define YASS_TICKS_RESOLUTION   24
#define YASS_TICKS_MIN_BPM      55
#define YASS_TICKS_MAX_BPM      280

class YASS_TICKS
{
    public:
        YASS_TICKS();
        void begin();
        void start();
        void stop();
        void setCallback(void(*_callback)());
        void setTempo(word bpm);
        word getTempo();
        void sequencer();
        bool isRunning();
        byte* getDataPointer();

    private:
        void(*callback)();
        bool runningFlag;
        word tickDuration;
        word tempo;
        unsigned long milestone;
};

#endif

