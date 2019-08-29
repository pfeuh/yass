/*
 * file : yassBeeper.cpp
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

#include "yassBeeper.h"

/*******************/
/* Private methods */
/*******************/


/******************/
/* Public methods */
/******************/

YASS_BEEPER::YASS_BEEPER(int io_pin, word msec_duration)
{
    ioPin = io_pin;
    msecDuration = msec_duration;
}

void YASS_BEEPER::begin()
{
    digitalWrite(ioPin, 0);
    pinMode(ioPin, OUTPUT);
    running = 0;
}

void YASS_BEEPER::sequencer()
{
    if(running)
    {
        if(milestone <= millis())
        {
            milestone += msecDuration;
            digitalWrite(ioPin, 0);
            running = 0;
        }
    }
}

void YASS_BEEPER::beep()
{
    milestone = millis() + msecDuration;
    digitalWrite(ioPin, 1);
    running = 1;
}


