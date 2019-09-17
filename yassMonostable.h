#ifndef yassMonostable_h
#define yassMonostable_h

/*
 * file : yassMonostable.h
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

// 2019 09 17 - v1.01 - adds method isRunning()
#define YASS_MONOSTABLE_VERSION "1.01"

class YASS_MONOSTABLE
{
    public:
        YASS_MONOSTABLE();
        void begin();
        void bind(word delay, void(*_callback)());
        void start();
        void stop();
        void sequencer();
        bool isBusy();

    private:
        word duration;
        void(*callback)();
        bool running;
        unsigned long milestone;

};

#endif

