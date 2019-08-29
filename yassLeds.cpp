/*
 * file : yassLeds.cpp
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

#include "yassLeds.h"

/*******************/
/* Private methods */
/*******************/

void YASS_LEDS::write(byte* addr, byte led_num, bool value)
{
    byte row_num = led_num / 8;
    byte col_num = led_num % 8;
    addr += row_num;
    byte out_byte = *addr;
    if(value)
        out_byte &= ~(1 << col_num);
    else
        out_byte |= 1 << col_num;
    *addr = out_byte;
}

/******************/
/* Public methods */
/******************/

YASS_LEDS::YASS_LEDS(byte* _individualLeds, byte* _stepLeds)
{
    individualLeds = _individualLeds;
    stepLeds = _stepLeds;
}

void YASS_LEDS::begin()
{
    clearLeds();
}

void YASS_LEDS::clearLeds()
{
    *stepLeds = 0xff;
    *(stepLeds + 1) = 0xff;
    *individualLeds = 0xff;
    *(individualLeds + 1) &= 0xf0;
    *(individualLeds + 1) |= 0x0f;
}

void YASS_LEDS::ledOn(byte led_num)
{
    write(individualLeds, led_num, YASS_LEDS_ON);
}

void YASS_LEDS::stepLedOn(byte led_num)
{   
    write(stepLeds, led_num, YASS_LEDS_ON);
}
