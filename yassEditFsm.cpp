/*
 * file : yassEditFsm.cpp
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

#include "yassEditFsm.h"

/*******************/
/* Private methods */
/*******************/

/******************/
/* Public methods */
/******************/

YASS_EDIT_FSM::YASS_EDIT_FSM()
{
}

void YASS_EDIT_FSM::begin(int pin_a, int pin_b)
{
    encoder.begin(pin_a, pin_b);
}

void YASS_EDIT_FSM::setState(byte state_num, void(*change_data_hook)(bool direction))
{
    state = state_num;
    encoder.bind(change_data_hook);
}

byte YASS_EDIT_FSM::getState()
{
    return state;
}

void YASS_EDIT_FSM::sequencer()
{
    encoder.sequencer();
}

YASS_ENCODER* YASS_EDIT_FSM::getEncoder()
{
    return &encoder;
}