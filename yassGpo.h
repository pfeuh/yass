#ifndef yassGpo_h
#define yassGpo_h

/*
 * file : yassGpo.h
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

/* driver for shift register 74LS595 (serial bits output) */

#include <Arduino.h>

#define YASS_GPO_VERSION "1.00"

class YASS_GPO
{
    public:
        YASS_GPO(int data_pin, int clock_pin, int latch_pin);
        void begin();
        void write(byte value);
        void validate();

    private:
        int dataPin;
        int clockPin;
        int latchPin;

};

#endif

