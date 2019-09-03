/*
 * file : yassRomSequences.cpp
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
#include "yassSequencer.h"
#include "yassRomSequences.h"
#include "yassSequencer.h"
#include "yassSequence.h"

#define REST YASS_SEQUENCE_REST
#define TIE YASS_SEQUENCE_TIE
#define CC_PAN 8

const byte PROGMEM yassRomSeqs[] =
{
/* ---*** Billie Jean ***--- */
    /* notes     */ 0x32, 0x2d, 0x30, 0x32, 0x30, 0x2d, 0x2b, 0x2d, REST, REST, REST, REST, REST, REST, REST, REST, 
    /* data      */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
    /* groove    */ YASS_SEQUENCE_CROCHE,
    /* gate mode */ YASS_SEQUENCE_GATE_LONG,
    /* last note */ 0x07,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY,
    /* cc num    */ 0x7f,
    /* fix. vel. */ 0x64,

    /* ---*** Midnight Express ***--- */
    /* notes     */ 0x32, 0x32, 0x32, 0x32, 0x30, 0x32, 0x35, 0x32, 0x2b, 0x2d, 0x2d, 0x2d, 0x2b, 0x2d, 0x30, 0x34, 
    /* data      */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
    /* groove    */ YASS_SEQUENCE_CROCHE,
    /* gate mode */ YASS_SEQUENCE_GATE_NORMAL,
    /* last note */ 0x0f,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY,
    /* cc num    */ 0x7f,
    /* fix. vel. */ 0x64,

    /* ---*** Blade runner ***--- */
    /* notes     */ 0x32, 0x32, 0x34, 0x35, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, 
    /* data      */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
    /* groove    */ YASS_SEQUENCE_DOUBLE_CROCHE,
    /* gate mode */ YASS_SEQUENCE_GATE_NORMAL,
    /* last note */ 0x03,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY,
    /* cc num    */ 0x7f,
    /* fix. vel. */ 0x64,

    /* ---*** Dady Cool ***--- */
    /* notes     */ 0x32, 0x32, 0x2d, 0x32, 0x31, 0x31, 0x2c, 0x31, 0x30, 0x30, 0x2b, 0x30, 0x31, 0x31, 0x2c, 0x31, 
    /* data      */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
    /* groove    */ YASS_SEQUENCE_CROCHE,
    /* gate mode */ YASS_SEQUENCE_GATE_NORMAL,
    /* last note */ 0x0f,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY,
    /* cc num    */ 0x7f,
    /* fix. vel. */ 0x64,

    /* ---*** Marcia Bailar ***--- */
    /* notes     */ 0x32, REST, REST, 0x36, 0x37, REST, REST, 0x33, 0x34, REST, REST, 0x38, 0x39, 0x38, 0x39, 0x34, 
    /* data      */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
    /* groove    */ YASS_SEQUENCE_CROCHE,
    /* gate mode */ YASS_SEQUENCE_GATE_LONG,
    /* last note */ 0x0f,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY,
    /* cc num    */ 0x7f,
    /* fix. vel. */ 0x64,

    /* ---*** Take Five ***--- */
    /* notes     */ 0x32, REST, REST, 0x32, REST, REST, 0x2d, REST, 0x2d, REST, REST, REST, REST, REST, REST, REST, 
    /* data      */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
    /* groove    */ YASS_SEQUENCE_CROCHE,
    /* gate mode */ YASS_SEQUENCE_GATE_NORMAL,
    /* last note */ 0x09,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY,
    /* cc num    */ 0x7f,
    /* fix. vel. */ 0x64,

    /* ---*** Latin Bolero ***--- */
    /* notes     */ 0x32, TIE , TIE , TIE , 0x39, REST, 0x2d, REST, REST, REST, REST, REST, REST, REST, REST, REST, 
    /* data      */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
    /* groove    */ YASS_SEQUENCE_CROCHE,
    /* gate mode */ YASS_SEQUENCE_GATE_LONG,
    /* last note */ 0x07,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY,
    /* cc num    */ 0x7f,
    /* fix. vel. */ 0x64,

    /* ---*** Boogie woogie ***--- */
    /* notes     */ 0x32, 0x32, 0x36, 0x36, 0x39, 0x39, 0x3b, 0x3b, 0x3e, 0x3e, 0x3b, 0x3b, 0x39, 0x39, 0x36, 0x36, 
    /* data      */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
    /* groove    */ YASS_SEQUENCE_CROCHE_SWING,
    /* gate mode */ YASS_SEQUENCE_GATE_NORMAL,
    /* last note */ 0x0f,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY,
    /* cc num    */ 0x7f,
    /* fix. vel. */ 0x64,

    /* ---*** Tubular bells ***--- */
    /* notes     */ 0x32, 0x32, 0x32, 0x80, 0x37, 0x39, 0x3e, 0x80, 0x3e, 0x3c, 0x39, 0x37, 0x3b, 0x3c, 0x35, 0x34, 
    /* data      */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
    /* groove    */ YASS_SEQUENCE_DOUBLE_CROCHE,
    /* gate mode */ YASS_SEQUENCE_GATE_NORMAL,
    /* last note */ 0x0f,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY,
    /* cc num    */ 0x7f,
    /* fix. vel. */ 0x64,

    /* ---*** Oxygene4 ***--- */
    /* notes     */ 0x32, 0x2d, 0x30, 0x32, TIE , 0x2d, 0x30, 0x2d, REST, REST, REST, REST, REST, REST, REST, REST, 
    /* data      */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
    /* groove    */ YASS_SEQUENCE_CROCHE_SWING,
    /* gate mode */ YASS_SEQUENCE_GATE_NORMAL,
    /* last note */ 0x07,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY,
    /* cc num    */ 0x7f,
    /* fix. vel. */ 0x64,
    
    /* ---*** clave ***--- */
    /* notes     */ 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 
    /* data      */ 0x7f, 0x64, 0x64, 0x7f, 0x64, 0x64, 0x7f, 0x64, 0x7f, 0x7f, 0x7f, 0x7f, 0x64, 0x7f, 0x64, 0x7f, 
    /* groove    */ YASS_SEQUENCE_DOUBLE_CROCHE,
    /* gate mode */ YASS_SEQUENCE_DATA_MODE_CTRL_CHG,
    /* last note */ 0x0f,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_VELOCITY,
    /* cc num    */ 0x00,
    /* fix. vel. */ 0x64,

    /* ---*** Minor arpeggio ***--- */
    /* notes     */ 0x32, 0x35, 0x39, 0x3e, 0x39, 0x35, 0x32, 0x2d, 0x29, 0x26, REST, REST, REST, REST, REST, REST, 
    /* data      */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
    /* groove    */ YASS_SEQUENCE_TRIOLET_DOUBLE_CROCHE,
    /* gate mode */ YASS_SEQUENCE_GATE_NORMAL,
    /* last note */ 0x0b,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY,
    /* cc num    */ 0x7f,
    /* fix. vel. */ 0x64,

    /* ---*** Major arpeggio ***--- */
    /* notes     */ 0x32, 0x36, 0x39, 0x3e, 0x39, 0x36, 0x32, 0x2d, 0x2a, 0x26, REST, REST, REST, REST, REST, REST, 
    /* data      */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
    /* groove    */ YASS_SEQUENCE_TRIOLET_DOUBLE_CROCHE,
    /* gate mode */ YASS_SEQUENCE_GATE_NORMAL,
    /* last note */ 0x0b,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY,
    /* cc num    */ 0x7f,
    /* fix. vel. */ 0x64,

    /* ---*** Diminued arpeggio ***--- */
    /* notes     */ 0x32, 0x35, 0x38, 0x3b, 0x3e, 0x41, 0x3e, 0x3b, 0x38, 0x35, 0x32, REST, REST, REST, REST, REST, 
    /* data      */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
    /* groove    */ YASS_SEQUENCE_TRIOLET_DOUBLE_CROCHE,
    /* gate mode */ YASS_SEQUENCE_GATE_NORMAL,
    /* last note */ 0x0b,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY,
    /* cc num    */ 0x7f,
    /* fix. vel. */ 0x64,
    
    /* ---*** Pentatonic arpeggio ***--- */
    /* notes     */ 0x32, 0x35, 0x37, 0x39, 0x3c, 0x3e, 0x41, 0x3e, 0x3c, 0x39, 0x37, 0x35, 0x32, REST, REST, REST, 
    /* data      */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
    /* groove    */ YASS_SEQUENCE_TRIOLET_DOUBLE_CROCHE,
    /* gate mode */ YASS_SEQUENCE_GATE_NORMAL,
    /* last note */ 0x0b,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY,
    /* cc num    */ 0x7f,
    /* fix. vel. */ 0x64,

    /* ---*** Augmented arpeggio ***--- */
    /* notes     */ 0x32, 0x36, 0x3a, 0x3e, 0x36, 0x3a, 0x3e, 0x42, 0x3a, 0x3e, 0x42, 0x45, 0x49, REST, REST, REST, 
    /* data      */ 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
    /* groove    */ YASS_SEQUENCE_TRIOLET_DOUBLE_CROCHE,
    /* gate mode */ YASS_SEQUENCE_GATE_NORMAL,
    /* last note */ 0x0b,
    /* data mode */ YASS_SEQUENCE_DATA_MODE_FIXED_VELOCITY,
    /* cc num    */ 0x7f,
    /* fix. vel. */ 0x64,
};

#define YASS_ROM_SEQUENCES_NB_SEQS ((sizeof(yassRomSeqs)) / YASS_SEQUENCE_DATA_SIZE)

void YASS_ROM_SEQUENCES_load(byte rom_seq_num, YASS_SEQUENCE* seq)
{
    if(rom_seq_num && (rom_seq_num <= YASS_ROM_SEQUENCES_NB_SEQS))
    {
        seq->initialize();
        const byte* rom_ptr = yassRomSeqs + (rom_seq_num - 1) * YASS_SEQUENCE_DATA_SIZE;
        
        for (byte x = 0; x < YASS_SEQUENCE_NB_NOTES; x++)
            seq->setNote(x, pgm_read_byte(rom_ptr++));
        for (byte x = 0; x < YASS_SEQUENCE_NB_NOTES; x++)
            seq->setData(x, pgm_read_byte(rom_ptr++));
        seq->setGroove(pgm_read_byte(rom_ptr++));
        seq->setGateMode(pgm_read_byte(rom_ptr++));
        seq->setLastStep(pgm_read_byte(rom_ptr++));
        seq->setDataMode(pgm_read_byte(rom_ptr++));
        seq->setControlChange(pgm_read_byte(rom_ptr++));
        seq->setFixedVelocity(pgm_read_byte(rom_ptr++));
    }
}

