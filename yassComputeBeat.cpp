/*
 * file : yassComputeBeat.cpp
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

#include "yassComputeBeat.h"

/*******************/
/* Private methods */
/*******************/


/******************/
/* Public methods */
/******************/

YASS_COMPUTE_BEAT::YASS_COMPUTE_BEAT()
{
}

void YASS_COMPUTE_BEAT::start()
{
    milestone = millis() + YASS_COMPUTE_BEAT_CYCLE_MSEC_DURATION;
    ticksCounter = 0;
    running = true;
}

void YASS_COMPUTE_BEAT::stop()
{
    running = true;
}

//~ void YASS_COMPUTE_BEAT::sequencer()
//~ {
    //~ if(running)
        //~ if(millis() >= milestone)
        //~ {
            //~ milestone += YASS_COMPUTE_BEAT_CYCLE_MSEC_DURATION;
            //~ // let's compute tempo
            //~ tempo = (ticksCounter * 60 + 12) / 24;
            //~ ticksCounter = 0;
        //~ }
//~ }

void YASS_COMPUTE_BEAT::sequencer()
{
    if(running)
        if(millis() >= milestone)
        {
            milestone += YASS_COMPUTE_BEAT_CYCLE_MSEC_DURATION;
            // let's compute tempo
            tempo = (ticksCounter * 30 + 2) / 24;
            ticksCounter = 0;
        }
}

void YASS_COMPUTE_BEAT::addTick()
{
    ticksCounter++;
}

word YASS_COMPUTE_BEAT::getTempo()
{
    return tempo;
}

