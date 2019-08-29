#ifndef display7seg_h
#define display7seg_h

/*
 * file : yassDisplay.h
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

#define YASS_DISPLAY_VERSION "1.00"

class YASS_DISPLAY
{
    public:
        YASS_DISPLAY(byte nb_digits, byte* _digits);
        void begin();
        byte getGlyphe(byte glyphe_num);
        void printGlyphe(byte value, byte digit_num);
        void printHexByte(byte value, byte digit_num);
        void printHexWord(word value, byte digit_num);
        void printWord(word value, byte base);
        void printLut(const char* lut, byte index, byte word_size);
        void writeDot(bool value, byte digit_num);
        void clear();
        void scroll();
        void write(char car);

    private:
        byte nbDigits;
        byte* digits;
        byte cursor;
    
};

#endif
