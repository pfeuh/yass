#ifndef yassBeeper_h
#define yassBeeper_h

/*
 * file : yassBeeper.h
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

#define YASS_BEEPER_VERSION "1.00"

class YASS_BEEPER
{
    public:
        YASS_BEEPER(int io_pin, word msec_duration);
        void begin();
        void sequencer();
        void beep();

    private:
        int ioPin;
        int msecDuration;
        unsigned long milestone;
        byte running;

};

#endif

