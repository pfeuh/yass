#ifndef yassEncoder_h
#define yassEncoder_h

/*
 * file : yassEncoder.h
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

/* modified version without push/pull button for Yass software */
#define YASS_ENCODER_VERSION "1.01a"

#define YASS_ENCODER_DIRECTION_UP true
#define YASS_ENCODER_DIRECTION_DOWN false
#define YASS_ENCODER_DEBOUCE_DURATION_MSEC 20

class YASS_ENCODER
{
    public:
        YASS_ENCODER();
        void begin(int pin_track_a, int pin_track_b);
        void sequencer();
        void bind(void(*change_data_hook)(bool direction));

    private:
        int pinTrackA;
        int pinTrackB;
        byte bits;
        void(*changeDataHook)(bool direction);
        void trigDebouncer();
        bool debouncing;
        unsigned long milestone;
};

#endif
