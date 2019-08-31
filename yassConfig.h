#ifndef yassConfig_h
#define yassConfig_h

/*
 * file : yassConfig.h
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

#define YASS_CONFIG_VERSION "1.00"

#define YASS_CONFIG_MIN_CHANNEL 1
#define YASS_CONFIG_MAX_CHANNEL 16

#define YASS_MAX_CC_NUM 127

#define YASS_DATA_MODE_VELOCITY 0
#define YASS_DATA_MODE_CC 1
#define YASS_DATA_MODE_NONE 2
#define YASS_DATA_MODE_MAX YASS_DATA_MODE_NONE

#define YASS_CONFIG_DATA_SIZE 8

class YASS_CONFIG
{
    public:
        YASS_CONFIG();
        byte getChannelIn();
        void setChannelIn(byte channel);
        byte getChannelOut();
        void setChannelOut(byte channel);
        void setOmni(bool flag);
        bool getOmni();
        void setClockIn(bool flag);
        bool getClockIn();
        void setClockOut(bool flag);
        bool getClockOut();
        void setClick(bool flag);
        bool getClick();
        void setArpeggiator(bool flag);
        bool getArpeggiator();
        void setKeyEcho(bool flag);
        bool getKeyEcho();
        void setProgNum(byte prog_num);
        byte getProgNum();
        void setUseProgNum(bool flag);
        bool getUseProgNum();
        void setCtrlChangeNumber(byte value);
        byte getCtrlChangeNumber();
        bool getUseSysEx();
        void setUseSysEx(bool flag);
        void setDataMode(byte value);
        byte getDataMode();
        byte* getDataPointer();

    private:
        //this bytes should be saved/loaded as parts of the sequence
        byte data[YASS_CONFIG_DATA_SIZE];

        void setBit(word weight, bool flag);
        bool getBit(word weight);

};

#endif

