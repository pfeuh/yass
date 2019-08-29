/*
 * file : yassKeyb.cpp
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

#include "yassKeyb.h"

/*******************/
/* Private methods */
/*******************/

byte YASS_KEYB::getState(byte column, byte row)
{
    return (codes[column + row * nbColumns] & 0x80) == 0x80;
}

void YASS_KEYB::setState(byte column, byte row, byte state)
{
    if(state)
        codes[column + row * nbColumns] |= 0x80;
    else
        codes[column + row * nbColumns] &= 0x7f;
}

byte YASS_KEYB::getCode(byte column, byte row)
{
    return codes[column + row * nbColumns] & 0x7f;
}

/******************/
/* Public methods */
/******************/

YASS_KEYB::YASS_KEYB(int* column_pins, byte nb_columns, int* row_pins, byte nb_rows, char* _codes)
{
    columnPins = column_pins;
    nbColumns = nb_columns;
    rowPins = row_pins;
    nbRows = nb_rows;
    codes = _codes;
    pushHandler = 0;
}

void YASS_KEYB::begin()
{
    for(byte x=0; x < nbColumns; x++)
    {
        pinMode(columnPins[x], INPUT_PULLUP);
    }
    for(byte x=0; x < nbRows; x++)
    {
        pinMode(rowPins[x], OUTPUT);
        digitalWrite(rowPins[x], 1);        
    }
}

void YASS_KEYB::setPushHandler(YASS_KEYB_type_callback callback)
{
    pushHandler = callback;
}

byte YASS_KEYB::available()
{
    return _available;
}

byte YASS_KEYB::read()
{
    _available = 0;
    return buffer;
}

void YASS_KEYB::sequencer()
{
    byte current_state;
    byte old_state;
    for(byte y=0; y < nbRows; y++)
    {
        digitalWrite(rowPins[y], 0);        
        for(byte x=0; x < nbColumns; x++)
        {
            current_state = digitalRead(columnPins[x]);
            old_state = getState(x, y);
            if(!current_state)
            {
                if(old_state)
                {
                    if(pushHandler)
                        pushHandler(getCode(x, y));
                    else
                    {
                        _available = 1;
                        buffer = getCode(x, y);
                    }
                }
            }
            setState(x, y, current_state);
        }
        digitalWrite(rowPins[y], 1);
    }
}

