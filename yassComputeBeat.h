#ifndef yassComputeBeat_h
#define yassComputeBeat_h

/*
 * file : yassComputeBeat.h
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

#define YASS_COMPUTE_BEAT_VERSION "1.00"

#define YASS_COMPUTE_BEAT_CYCLE_MSEC_DURATION 1000

class YASS_COMPUTE_BEAT
{
    public:
        YASS_COMPUTE_BEAT();
        void start();
        void stop();
        void sequencer();
        void addTick();
        word getTempo();

    private:
        word ticksCounter;
        word tempo;
        unsigned long milestone;
        bool running;

};

#endif

