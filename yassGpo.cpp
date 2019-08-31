/*
 * file : yassGpo.cpp
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

#include "yassGpo.h"

/*******************/
/* Private methods */
/*******************/


/******************/
/* Public methods */
/******************/

YASS_GPO::YASS_GPO(int data_pin, int clock_pin, int latch_pin)
{
    dataPin  = data_pin;
    clockPin = clock_pin;
    latchPin = latch_pin;
}

void YASS_GPO::begin()
{
    pinMode(dataPin , OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    digitalWrite(clockPin, 0);
    digitalWrite(latchPin, 0);
}

void YASS_GPO::write(byte value)
{
    for(byte bit_num = 7; bit_num != 255; bit_num--)
    {
        byte bit = (value & (1 << bit_num)) != 0;
        digitalWrite(dataPin, bit);
        digitalWrite(clockPin, 1);
        digitalWrite(clockPin, 0);
    }
}

void YASS_GPO::validate()
{
    digitalWrite(latchPin, 1);
    digitalWrite(latchPin, 0);
}
