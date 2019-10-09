#!/usr/bin/python
# -*- coding: utf-8 -*-

# 
# file : yassFrontPannel.py
# Copyright (c) pfeuh <ze.pfeuh@gmail>
# All rights reserved.
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 

import sys
import os

sys.stdout.write(sys.version + "\n")

import scriptSvg

HOLE_RADIUS = 1.6
HOLE_OFFSET = 6.0

FRONT_PANNEL_WIDTH = 215.0
FRONT_PANNEL_HEIGHT = 135.0
FRONT_PANNEL_CORNER_RADIUS = 0.0

KEY_WIDTH = 14.1
KEYBOARD_WIDTH = 64.35
KEYBOARD_HOLE_X_INTERVAL = 76.2
KEYBOARD_HOLE_Y_INTERVAL = 51.0
KEYBOARD_FRONT_PANNEL_FOOTPRINT_WIDTH = KEYBOARD_HOLE_X_INTERVAL
KEYBOARD_FRONT_PANNEL_FOOTPRINT_HEIGHT = KEYBOARD_HOLE_X_INTERVAL
KEYBOARD_HOLE_FOOTPRINT_WIDTH = 8.0
KEYBOARD_CONNECTOR_WIDTH = 40.0
KEYBOARD_CONNECTOR_HEIGHT = 6.0

LCD_4x20_WIDTH = 97.2
LCD_4x20_HEIGHT = 40.0
LDC_4x20_FOOTPRINT_WIDTH = LCD_4x20_WIDTH
LDC_4x20_FOOTPRINT_HEIGHT = 60.0
LDC_4x20_HOLE_X_INTERVAL = 93.0
LDC_4x20_HOLE_Y_INTERVAL = 55.0
LCD_4x20_HOLE_FOOTPRINT_WIDTH = 8.0
LCD_4x20_CONNECTOR_WIDTH = 40.65
LCD_4x20_CONNECTOR_HEIGHT = 2.54 * 1.5
LCD_SCREEN_WIDTH = 76.22
LCD_SCREEN_HEIGHT = 25.41

ENCODER_RADIUS = 6.85 / 2.0
ENCODER_FOOT_PRINT_HEIGHT = 12.1
ENCODER_FOOT_PRINT_WIDTH = 20.0

DRAW_FOOTPRINT = False

def addLCD4x12(sketch, _x, _y):
    sketch.addRectangle(_x, _y, LCD_4x20_WIDTH, LCD_4x20_HEIGHT, 0, label="LCD4x12")
    sketch.addRectangle(_x, _y, LCD_SCREEN_WIDTH, LCD_SCREEN_HEIGHT, 2.0, label="LCD4x20ScreenFootprint")
    if DRAW_FOOTPRINT:
        sketch.addRectangle(_x, _y, LDC_4x20_FOOTPRINT_WIDTH, LDC_4x20_FOOTPRINT_HEIGHT, 0, label="LCD4x20Footprint")
        x = 8.5 - LDC_4x20_FOOTPRINT_WIDTH / 2.0 + LCD_4x20_CONNECTOR_WIDTH / 2.0
        y = 0.0 + LDC_4x20_FOOTPRINT_HEIGHT / 2.0 - LCD_4x20_CONNECTOR_HEIGHT / 2.0
    
    x = LDC_4x20_HOLE_X_INTERVAL / 2.0
    y = LDC_4x20_HOLE_Y_INTERVAL / 2.0
    for hole_num, (a,b) in enumerate(((-x, y), (-x, -y), (x, y), (x, -y), )):
        sketch.addCircle(_x + a,_y + b, HOLE_RADIUS, label="keyboardFixHole1")
        if DRAW_FOOTPRINT:
            sketch.addCircle(_x + a,_y + b, LCD_4x20_HOLE_FOOTPRINT_WIDTH / 2.0, label="lcd4x20FixHoleFootprint%u"%(hole_num+1))

def addKeyboard4x4(sketch, _x, _y):
    matter_witdh = KEY_WIDTH * 4.0
    void_width = KEYBOARD_WIDTH - matter_witdh
    step_size = KEY_WIDTH + void_width / 3.0
    x_origin = _x - KEYBOARD_WIDTH / 2.0 + 0.5 * step_size - void_width / 6.0
    y_origin = _y - KEYBOARD_WIDTH / 2.0 + 0.5 * step_size - void_width / 6.0
    
    for col in range(4):
        for row in range(4):
            x = x_origin + step_size * float(col)
            y = y_origin + step_size * float(row)
            label = "key(%u:%u)"%(col, row)
            sketch.addRectangle(x, y, KEY_WIDTH, KEY_WIDTH, 0, label=label)

    x = KEYBOARD_HOLE_X_INTERVAL / 2.0
    y = KEYBOARD_HOLE_Y_INTERVAL / 2.0
    for hole_num, (a,b) in enumerate(((-x, y), (-x, -y), (x, y), (x, -y), )):
        sketch.addCircle(_x + a,_y + b, HOLE_RADIUS, label="keyboardFixHole1")
        if DRAW_FOOTPRINT:
            sketch.addCircle(_x + a,_y + b, KEYBOARD_HOLE_FOOTPRINT_WIDTH / 2.0, label="keyboardFixHoleFootprint%u"%(hole_num+1))

    if DRAW_FOOTPRINT:
        sketch.addRectangle(_x, _y, KEYBOARD_FRONT_PANNEL_FOOTPRINT_WIDTH, KEYBOARD_FRONT_PANNEL_FOOTPRINT_HEIGHT, label="keyboardFootprint")
        sketch.addRectangle(_x, _y + KEYBOARD_FRONT_PANNEL_FOOTPRINT_HEIGHT /2.0 + KEYBOARD_CONNECTOR_HEIGHT / 2.0, 40.0, KEYBOARD_CONNECTOR_HEIGHT, label="keyboardFootprint")

def addEncoder(sketch, _x, _y):
    sketch.addCircle(_x, _y, ENCODER_RADIUS, label="EncoderFixHole")
    if DRAW_FOOTPRINT:
        sketch.addRectangle(_x, _y, ENCODER_FOOT_PRINT_WIDTH, ENCODER_FOOT_PRINT_HEIGHT, label="encoderFootprint")

def drawFrontPannel(sketch):
    # let's draw front pannel border
    sketch.addRectangle(0, 0, FRONT_PANNEL_WIDTH, FRONT_PANNEL_HEIGHT, FRONT_PANNEL_CORNER_RADIUS, label="frontPannel")

    # let's add 4 holes to fix front pannel on box
    x, y = FRONT_PANNEL_WIDTH / 2.0 - HOLE_OFFSET, FRONT_PANNEL_HEIGHT / 2.0 - HOLE_OFFSET
    for hole_num, (a,b) in enumerate(((-x, y), (-x, -y), (x, y), (x, -y), )):
        sketch.addCircle(a, b, HOLE_RADIUS, label="boxFixHole%u"%(hole_num + 1))
        if DRAW_FOOTPRINT:
            sketch.addRectangle(a,b, HOLE_OFFSET * 2.0, HOLE_OFFSET * 2.0, 0.0, label="FrontPanelFixHoleFootprint%u"%(hole_num+1))

if __name__ == "__main__":

    FNAME = os.path.basename(os.path.splitext(sys.argv[0])[0])
    
    IMG_PATH = "img"
    filename = "%s/%s.svg"%(IMG_PATH, FNAME)
    sketch = scriptSvg.SKETCH(filename, "mm")
    
    # I don't know why drawing it out of the sheet, but I can fix it
    sketch.changeOrigin(152.0, -190.0)

    drawFrontPannel(sketch)
    addKeyboard4x4(sketch, 57.5, -19.0)
    addLCD4x12(sketch, 10.0 - FRONT_PANNEL_WIDTH / 4.0, 34.0)
    addEncoder(sketch, 57.5,  34.0)
    
    sketch.save()
    sketch.makeCSV("img/%s_coordinates.csv"%FNAME)

