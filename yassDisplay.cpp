/*
 * file : yassDisplay.cpp
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

#include "yassDisplay.h"

#define _A 0x01
#define _B 0x02
#define _C 0x04
#define _D 0x08
#define _E 0x10
#define _F 0x20
#define _G 0x40

#define YASS_DISPLAY_DOT_WEIGHT      0x80
#define YASS_DISPLAY_SEGMENTS_WEIGHT 0x7f
#define ASCII_INDEX_MASK             0x7f

const byte YASS_DISPLAY_FONT[] PROGMEM =
{
    _A | _B | _C | _D | _E | _F,      // 0
    _B | _C,                          // 1
    _A | _B | _D | _E | _G,           // 2
    _A | _B | _C | _D | _G,           // 3
    _B | _C | _F | _G,                // 4
    _A | _C | _D | _F | _G,           // 5
    _A | _C | _D | _E | _F | _G,      // 6
    _A | _B | _C,                     // 7
    _A | _B | _C | _D | _E | _F | _G, // 8
    _A | _B | _C | _D | _F | _G,      // 9
    _A | _B | _C | _E | _F | _G,      // 0x0a
    _C | _D | _E | _F | _G,           // 0x0b
    _A | _D | _E | _F,                // 0x0c
    _B | _C | _D | _E | _G,           // 0x0d
    _A | _D | _E | _F | _G,           // 0x0e
    _A | _E | _F | _G,                // 0x0f
    B00000001,                        // 0x10 
    B00000010,                        // 0x11 
    B00000100,                        // 0x12 
    B00001000,                        // 0x13 
    B00010000,                        // 0x14 
    B00100000,                        // 0x15 
    B01000000,                        // 0x16 
    B10000000,                        // 0x17 
    B00010001,                        // 0x18 
    B00100010,                        // 0x19 
    B01000100,                        // 0x1a 
    B10001000,                        // 0x1b 
    B00010001,                        // 0x1c 
    B00100010,                        // 0x1d 
    B01000100,                        // 0x1e 
    B10001000,                        // 0x1f 
    B00000000,                        // ascii code 0x20  
    B00000000,                        // ascii code 0x21 !
    B00000000,                        // ascii code 0x22 "
    B00000000,                        // ascii code 0x23 #
    B00000000,                        // ascii code 0x24 $
    B00000000,                        // ascii code 0x25 %
    B00000000,                        // ascii code 0x26 &
    B00000000,                        // ascii code 0x27 '
    B00000000,                        // ascii code 0x28 (
    B00000000,                        // ascii code 0x29 )
    B00000000,                        // ascii code 0x2a *
    B00000000,                        // ascii code 0x2b +
    B00000000,                        // ascii code 0x2c ,
    B01000000,                        // ascii code 0x2d -
    B00000000,                        // ascii code 0x2e .
    B00000000,                        // ascii code 0x2f /
    _A | _B | _C | _D | _E | _F,      // ascii code 0x30 0
    _B | _C,                          // ascii code 0x31 1
    _A | _B | _D | _E | _G,           // ascii code 0x32 2
    _A | _B | _C | _D | _G,           // ascii code 0x33 3
    _B | _C | _F | _G,                // ascii code 0x34 4
    _A | _C | _D | _F | _G,           // ascii code 0x35 5
    _A | _C | _D | _E | _F | _G,      // ascii code 0x36 6
    _A | _B | _C,                     // ascii code 0x37 7
    _A | _B | _C | _D | _E | _F | _G, // ascii code 0x38 8
    _A | _B | _C | _D | _F | _G,      // ascii code 0x39 9
    B00000000,                        // ascii code 0x3a :
    B00000000,                        // ascii code 0x3b ;
    B00000000,                        // ascii code 0x3c <
    B00000000,                        // ascii code 0x3d =
    B00000000,                        // ascii code 0x3e >
    B00000000,                        // ascii code 0x3f ?
    B00000000,                        // ascii code 0x40 @
    _A | _B | _C | _E | _F | _G,      // ascii code 0x41 A
    _C | _D | _E | _F | _G,           // ascii code 0x42 B
    _A | _D | _E | _F,                // ascii code 0x43 C
    _B | _C | _D | _E | _G,           // ascii code 0x44 D
    _A | _D | _E | _F | _G,           // ascii code 0x45 E
    _A | _E | _F | _G,                // ascii code 0x46 F
    _A | _C | _D | _E | _F ,          // ascii code 0x47 G
    B01110110,                        // ascii code 0x48 H
    B00110000,                        // ascii code 0x49 I
    B00001110,                        // ascii code 0x4a J
    B00000000,                        // ascii code 0x4b K
    B00111000,                        // ascii code 0x4c L
    B00110111,                        // ascii code 0x4d M
    B01010100,                        // ascii code 0x4e N
    _A | _B | _C | _D | _E | _F,      // ascii code 0x4f O
    B01110011,                        // ascii code 0x50 P
    B01100111,                        // ascii code 0x51 Q
    B01010000,                        // ascii code 0x52 R
    B01101101,                        // ascii code 0x53 S
    B01110000,                        // ascii code 0x54 T
    B00111110,                        // ascii code 0x55 U
    B00111000,                        // ascii code 0x76 V
    B01110100,                        // ascii code 0x77 W
    B01010010,                        // ascii code 0x78 X
    B01100110,                        // ascii code 0x59 Y
    _A | _B | _D | _E | _G,           // ascii code 0x5a Z
    B00111001,                        // ascii code 0x5b [
    B00000000,                        // ascii code 0x5c 
    B00001111,                        // ascii code 0x5d ]
    B00000000,                        // ascii code 0x5e ^
    B00001000,                        // ascii code 0x5f _
    B00000000,                        // ascii code 0x60 
    _A | _B | _C | _E | _F | _G,      // ascii code 0x61 a
    _C | _D | _E | _F | _G,           // ascii code 0x62 b
    _D | _E | _G,                     // ascii code 0x63 c
    _B | _C | _D | _E | _G,           // ascii code 0x64 d
    _A | _D | _E | _F | _G,           // ascii code 0x65 e
    _A | _E | _F | _G,                // ascii code 0x66 f
    _A | _B | _C | _D | _F | _G,      // ascii code 0x67 g
    B01110100,                        // ascii code 0x68 h
    B00010000,                        // ascii code 0x69 i
    B00001110,                        // ascii code 0x6a j
    B00000000,                        // ascii code 0x6b k
    B00110000,                        // ascii code 0x6c l
    B00110111,                        // ascii code 0x6d m
    B01010100,                        // ascii code 0x6e n
    B01011100,                        // ascii code 0x6f o
    B01110011,                        // ascii code 0x70 p
    B01100111,                        // ascii code 0x71 q
    B01010000,                        // ascii code 0x72 r
    B01101101,                        // ascii code 0x73 s
    B01110000,                        // ascii code 0x74 t
    B00011100,                        // ascii code 0x75 u
    B00111000,                        // ascii code 0x76 v
    B01110100,                        // ascii code 0x77 w
    B01010010,                        // ascii code 0x78 x
    B01101110,                        // ascii code 0x79 y
    _A | _B | _D | _E | _G,           // ascii code 0x7a z
    B00000000,                        // ascii code 0x7b {
    B00110000,                        // ascii code 0x7c |
    B00000000,                        // ascii code 0x7d }
    B00000000,                        // ascii code 0x7e 
    B00000000,                        // ascii code 0x7f 
};

/*******************/
/* Private methods */
/*******************/

/******************/
/* Public methods */
/******************/

YASS_DISPLAY::YASS_DISPLAY(byte nb_digits, byte* _digits)
{
    nbDigits = nb_digits;
    digits = _digits;   
}

void YASS_DISPLAY::begin()
{
    // let's start with an empty display ...
    clear();
    // ... even the dots
    for(byte x = 0; x < nbDigits; x++)
        writeDot(0, x);
}

byte YASS_DISPLAY::getGlyphe(byte glyphe_num)
{
    return pgm_read_byte(YASS_DISPLAY_FONT + (glyphe_num & ASCII_INDEX_MASK));
}

void YASS_DISPLAY::printGlyphe(byte value, byte digit_num)
{
    if(digit_num < nbDigits)
        digits[digit_num] = (digits[digit_num] & YASS_DISPLAY_DOT_WEIGHT) | getGlyphe(value);
}

void YASS_DISPLAY::printHexByte(byte value, byte digit_num)
{
    printGlyphe((value & 0xf0) >> 4, digit_num++);
    printGlyphe( value & 0x0f, digit_num);
}

void YASS_DISPLAY::printHexWord(word value, byte digit_num)
{
    printHexByte((value & 0xff00) >> 8, digit_num);
    digit_num += 2;
    printHexByte(value & 0x00ff, digit_num);
}

void YASS_DISPLAY::printWord(word value, byte base)
{
    byte stack[nbDigits];
    for(byte idx = 0; idx < nbDigits; idx++)
    {
        stack[idx] = value % base;
        value = value / base;
    }
    clear();
    bool started = false;
    for(byte idx = nbDigits - 1; idx != 255; idx--)
    {
        // don't display leading zeros
        if(stack[idx])
            started = true;
        if(started)
            write(stack[idx]);
    }
    if(!started)
        write(0);
}

void YASS_DISPLAY::printLut(const char* lut, byte index, byte word_size)
{
    clear();
    lut += index * word_size;
    for(byte count = 0; count < word_size; count++)
        write(pgm_read_byte(lut++));
}

void YASS_DISPLAY::writeDot(bool value, byte digit_num)
{
    byte dot_weight = 0;
    if(value)
        dot_weight = YASS_DISPLAY_DOT_WEIGHT;
    digits[digit_num] = (digits[digit_num] & YASS_DISPLAY_SEGMENTS_WEIGHT) | dot_weight;
}

void YASS_DISPLAY::clear()
{
    // dots are not cleared. It is not a bug, it's a feature
    for(byte digit_num = 0; digit_num < nbDigits; digit_num++)
        digits[digit_num] &= YASS_DISPLAY_DOT_WEIGHT;
    cursor = 0;
}

void YASS_DISPLAY::scroll()
{
    if(nbDigits > 1)
    {
        byte car;
        for(byte x = 1; x < nbDigits; x++)
        {
            car = digits[x] & YASS_DISPLAY_SEGMENTS_WEIGHT;
            digits[x-1] = (digits[x-1] & YASS_DISPLAY_DOT_WEIGHT) | car;
        }
    }
}

void YASS_DISPLAY::write(char car)
{
    if(cursor >= (nbDigits))
    {
        scroll();
        printGlyphe((byte)car, nbDigits - 1);
    }
    else
        printGlyphe((byte)car, cursor++);
}

byte* YASS_DISPLAY::getDigitsPtr()
{
    return digits;
}

byte YASS_DISPLAY::getNbDigits()
{
    return nbDigits;
}

