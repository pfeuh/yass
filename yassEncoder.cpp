/*
 * file : yassEncoder.cpp
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


#include "yassEncoder.h"

#define YASS_ENCODER_NEW_TRACK_A     0x01
#define YASS_ENCODER_OLD_TRACK_A     0x02
#define YASS_ENCODER_NEW_TRACK_B     0x04
#define YASS_ENCODER_OLD_TRACK_B     0x08

#define YASS_ENCODER_TRACK_A_EDGE_MASK (YASS_ENCODER_NEW_TRACK_A | YASS_ENCODER_OLD_TRACK_A)
#define YASS_ENCODER_BUTTON_EDGE_MASK (YASS_ENCODER_NEW_BUTTON | YASS_ENCODER_OLD_BUTTON)

/*******************/
/* Private methods */
/*******************/

void YASS_ENCODER::trigDebouncer()
{
    milestone = millis() + YASS_ENCODER_DEBOUCE_DURATION_MSEC;
    debouncing = true;
}

/******************/
/* Public methods */
/******************/

YASS_ENCODER::YASS_ENCODER()
{
}

void YASS_ENCODER::begin(int pin_track_a, int pin_track_b)
{
    pinTrackA = pin_track_a;
    pinTrackB = pin_track_b;
    pinMode(pinTrackA, INPUT_PULLUP);
    pinMode(pinTrackB, INPUT_PULLUP);
    bits = YASS_ENCODER_OLD_TRACK_A | YASS_ENCODER_OLD_TRACK_B;
    debouncing = false;
}

void YASS_ENCODER::bind(void(*change_data_hook)(bool direction))
{
    changeDataHook   = change_data_hook;
}

void YASS_ENCODER::sequencer()
{
    if(debouncing)
        if(millis() >= milestone)
            debouncing = false;

    if(!debouncing)
    {
        
        
        // let's take a picture
        bits = ((bits & (~YASS_ENCODER_NEW_TRACK_A)) |   digitalRead(pinTrackA));
        bits = ((bits & (~YASS_ENCODER_NEW_TRACK_B)) |  (digitalRead(pinTrackB) << 2));
        
        // is there a falling edge on track A?
        if((bits &  YASS_ENCODER_TRACK_A_EDGE_MASK) == YASS_ENCODER_OLD_TRACK_A)
        {
            if(changeDataHook)
            {
                // OK, there is one, clock wise or counter clock wise?
                if(bits &  YASS_ENCODER_NEW_TRACK_B)
                {
                    // clockwise
                    changeDataHook(YASS_ENCODER_DIRECTION_UP);
                }
                else
                {
                    // counter clockwise
                    changeDataHook(YASS_ENCODER_DIRECTION_DOWN);
                }
                trigDebouncer();
            }
        }
            
        // let's push current bits to old bits
        bits <<= 1;
    }
}

