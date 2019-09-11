#ifndef yassEditFsm_h
#define yassEditFsm_h

/*
 * file : yassEditFsm.h
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
#include "yassEncoder.h"

#define YASS_EDIT_FSM_STATE_VERSION "1.00"

class YASS_EDIT_FSM
{
    public:
        YASS_EDIT_FSM();
        void begin(int pin_a, int pin_b);
        void setState(byte state_num, void(*change_data_hook)(bool direction));
        byte getState();
        void sequencer();
        YASS_ENCODER* getEncoder();

    private:
        byte state;
        YASS_ENCODER encoder = YASS_ENCODER();

};

#endif

