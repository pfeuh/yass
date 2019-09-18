/*
 * file : yassMonostable.cpp
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

#include "yassMonostable.h"

#define YASS_MONOSTABLE_MODE_MONO 1
#define YASS_MONOSTABLE_MODE_BURST 2
#define YASS_MONOSTABLE_MODE_ENDLESS 3

/******************/
/* Public methods */
/******************/

YASS_MONOSTABLE::YASS_MONOSTABLE()
{
}

void YASS_MONOSTABLE::begin()
{
    running = false;
}

void YASS_MONOSTABLE::bind(word delay, void(*_callback)())
{
    duration = delay;
    callback = _callback;
}

bool YASS_MONOSTABLE::isBusy()
{
    return running;
}

void YASS_MONOSTABLE::start()
{
    milestone = millis() + duration;
    running = true;
}

void YASS_MONOSTABLE::stop()
{
    running = false;
}

void YASS_MONOSTABLE::sequencer()
{
    if(running)
    {
        if(milestone <= millis())
        {
            if(callback)
                callback();
            running = false;
        }
    }
}


