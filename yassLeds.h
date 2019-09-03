#ifndef yassLeds_h
#define yassLeds_h

/*
 * file :yassLeds.h
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

#define YASS_LEDS_VERSION "1.00"

#define YASS_LEDS_ON 1
#define YASS_LEDS_OFF 0

#define YASS_LEDS_NB_STEP_LEDS 16
#define YASS_LEDS_NB_INDIVIDUAL_LEDS 12

#define LED_1 5
#define LED_2 4
#define LED_3 3
#define LED_4 2
#define LED_5 1
#define LED_START 7
#define LED_RECORD 6
#define LED_GLOBAL 8
#define LED_SEQUENCE 9
#define LED_REST 0
#define LED_TIE 11
#define LED_MULTI_USAGE 10
#define LED_STEP_0  0
#define LED_STEP_1  1
#define LED_STEP_2  2
#define LED_STEP_3  3
#define LED_STEP_4  4
#define LED_STEP_5  5
#define LED_STEP_6  6
#define LED_STEP_7  7
#define LED_STEP_8  8
#define LED_STEP_9  9
#define LED_STEP_10 10
#define LED_STEP_11 11
#define LED_STEP_12 12
#define LED_STEP_13 13
#define LED_STEP_14 14
#define LED_STEP_15 15

class YASS_LEDS
{
    public:
        YASS_LEDS(byte* _individualLeds, byte* _stepLeds);
        void begin();
        void clearLeds();
        void ledOn(byte led_num);
        void stepLedOn(byte led_num);

    private:
        byte* individualLeds;
        byte* stepLeds;
    
        void write(byte* addr, byte col_num, bool value);

};

#endif

