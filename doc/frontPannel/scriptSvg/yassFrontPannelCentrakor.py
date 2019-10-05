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

#~ USE_TEXT = False
USE_TEXT = True
MAKE_BOARD_FIXATION = True
USE_TEKO_BOX = False
#~ USE_TEKO_BOX = True
#~ USE_CENTRAKOR_BOX = False
USE_CENTRAKOR_BOX = True
#~ NO_SCREW_HOLES = True
#~ MAKE_6_PANELS = False
#~ FIXATION_BIG = False
#~ ROUNDED_RECTANGLE = True

FRONT_PANNEL_WIDTH = 7.8
FRONT_PANNEL_HEIGHT = 5.2
TEKO_FRONT_PANNEL_WIDTH = 8.24
TEKO_FRONT_PANNEL_HEIGHT = 4.94 # 5.12
FRONT_PANNEL_CORNER_RADIUS = 0.395

CENTRAKOR_FRONT_PANNEL_WIDTH = 8.464
CENTRAKOR_FRONT_PANNEL_HEIGHT = 5.315
CENTRAKOR_FRONT_PANNEL_CORNER_RADIUS = 0.0

BUTTON_FONT_SIZE = 0.06
LED_FONT_SIZE = 0.04
TITLE_FONT_SIZE = 0.16
LEGEND_FONT_SIZE = 0.033

LED_RADIUS = 0.061
STEP_LED_X_ORIGIN = -2.975
STEP_LED_Y_ORIGIN = -0.025
STEP_LED_H_INTERVAL = 0.35
STEP_LED_H_TEXT_OFFSET = LED_RADIUS
STEP_LED_V_TEXT_OFFSET = LED_RADIUS * 2

BUTTON_X_ORIGIN = -2.975
BUTTON_Y_ORIGIN = -1.775
STEP_BUTTON_V_INTERVAL = 1.1
BUTTON_RADIUS = 0.2315
BUTTON_LED_V_INTERVAL = STEP_LED_H_INTERVAL
BUTTON_LED_H_INTERVAL = 0.0125
BUTTON_TEXT_UP = 0.63
BUTTON_TEXT_DOWN = -0.55

def addButton(sketch, x, y, text):
    sketch.addCircle(x, y, BUTTON_RADIUS)
    if USE_TEXT:
        if text in ("GLOB", "SEQ."):
            sketch.addText(text, x + 0.25, y - 0.1, font_size=BUTTON_FONT_SIZE, anchor="start")
        elif text in ("OK", "REST", "TIE", "DATA"):
            sketch.addText(text, x, y + 0.5, font_size=BUTTON_FONT_SIZE, anchor="middle")
        else:
            sketch.addText(text, x, y + BUTTON_TEXT_Y_OFFSET, font_size=BUTTON_FONT_SIZE, anchor="middle")

def drawFrontPannel(sketch):
    # let's compute front pannel border
    if USE_TEKO_BOX: 
        sketch.addRectangle(0, 0, TEKO_FRONT_PANNEL_WIDTH, TEKO_FRONT_PANNEL_HEIGHT, 0, label="tekoFrontPannel")
    elif USE_CENTRAKOR_BOX: 
        sketch.addRectangle(0, 0, CENTRAKOR_FRONT_PANNEL_WIDTH, CENTRAKOR_FRONT_PANNEL_HEIGHT, CENTRAKOR_FRONT_PANNEL_CORNER_RADIUS, label="frontPannel")
    else: 
        sketch.addRectangle(0, 0, FRONT_PANNEL_WIDTH, FRONT_PANNEL_HEIGHT, FRONT_PANNEL_CORNER_RADIUS, label="frontPannel")

    # let's compute step leds
    glob_text = ["In", "Out", "Prog. num.", "Arpeggiator", "Clock in", "Clock out", "Key click ", "Audio beat", "Accept sysEx", "Transposition", "Dump all", "Load all", "Save all", "Dump global", "Load global", "Save global"]
    seq_text = ["Groove", "Step duration", "Last step", "Data mode", "C. C. number", "Fixed velo. value", "", "", "", "", "Swap", "Copy", "Load fact. seq.", "Dump seq.", "Load seq.", "Save seq."]
    x = STEP_LED_X_ORIGIN 
    y = STEP_LED_Y_ORIGIN
    r = LED_RADIUS
    for led_num in range(16):
        sketch.addCircle(x, y, r, label="step led %u"%(led_num+1))
        if USE_TEXT:
            sketch.addText(str(led_num+1), x, y + STEP_LED_V_TEXT_OFFSET * 0.75, font_size = LED_FONT_SIZE, label="Led_%u_num"%(led_num+1), anchor="middle")
            sketch.addText(glob_text[led_num], x + STEP_LED_H_TEXT_OFFSET, y + STEP_LED_V_TEXT_OFFSET * 2.0, font_size = LED_FONT_SIZE, transform="-90", label="globalText%u"%(led_num+1))
            sketch.addText(seq_text[led_num], x + STEP_LED_H_TEXT_OFFSET, y - STEP_LED_V_TEXT_OFFSET, font_size = LED_FONT_SIZE, anchor="end", transform="-90", label="sequenceText%u"%(led_num+1))
        x += STEP_LED_H_INTERVAL

    # let's compute buttons
    button_text =           ["PLAY", "STOP",  "PREV", "REC.", "1",  "2",  "3",  "4",  "5",  "SEQ.", "GLOB", "NEXT", "SWITCH", "TIE", "REST"]
    button_led =            [True  , False,   False,  True,   True, True, True, True, True, True,   True,   False,  True,     True,  True]
    button_nb_x_intervals = [0,       2,       4,      6,      9,    11,   13,   15,   17,   17,     17,     17,    4,        2,     0]
    button_nb_y_intervals = [0,       0,       0,      0,      0,    0,    0,    0,    0,    1,      2,      3,     3,        3,     3]
    button_x   = []
    button_y   = []
    for button_num in range(len(button_text)):
        text = button_text[button_num]
        button_x.append(BUTTON_X_ORIGIN + STEP_LED_H_INTERVAL * button_nb_x_intervals[button_num])
        button_y.append(BUTTON_Y_ORIGIN + STEP_BUTTON_V_INTERVAL * button_nb_y_intervals[button_num])
        sketch.addCircle(button_x[button_num], button_y[button_num], BUTTON_RADIUS, label="button%s"%(text))
        if button_led[button_num]:
            sketch.addCircle(button_x[button_num] - BUTTON_LED_H_INTERVAL, button_y[button_num] + BUTTON_LED_V_INTERVAL, LED_RADIUS, label="buttonLed%s"%(text))
        anchor = "middle"
        x = button_x[button_num]
        if text != "":
            if button_num < 9:
                y = button_y[button_num] + BUTTON_TEXT_DOWN
            elif button_num < 11:
                y = button_y[button_num] - 0.1
                x = button_x[button_num] + 0.26
                anchor = "start"
            else:
                y = button_y[button_num] + BUTTON_TEXT_UP
            if USE_TEXT:
                sketch.addText(text, x, y, anchor=anchor, font_size=BUTTON_FONT_SIZE, label="buttonLed%s"%(text))

    # let's add 4 holes to fix board
    if MAKE_BOARD_FIXATION:
        sketch.addCircle(-3.05, 0.80, LED_RADIUS, label="boardFixHole1")
        sketch.addCircle(-3.05, -1.10, LED_RADIUS, label="boardFixHole2")
        sketch.addCircle(3.15, 1.0, LED_RADIUS, label="boardFixHole3")
        sketch.addCircle(3.15, -1.2, LED_RADIUS, label="boardFixHole4")

    # let's add 4 holes to fix front pannel on box
    if MAKE_BOARD_FIXATION:
        if USE_TEKO_BOX:
            x, y = 3.9375, 2.265
        else:
            x, y = 3.588, 2.238
        sketch.addCircle(-x, y, LED_RADIUS, label="boxFixHole1")
        sketch.addCircle(-x, -y, LED_RADIUS, label="boxFixHole2")
        sketch.addCircle(x, y, LED_RADIUS, label="boxFixHole3")
        sketch.addCircle(x, -y, LED_RADIUS, label="boxFixHole4")

    # let's compute 4x7 segments display
    sketch.addRectangle(0.2, 1.52, 1.587, 0.55, 0, label="display4x7segments")

    # let's compute encoder button
    x = 1.850
    sketch.addCircle(x, 1.5, 0.29, label="encoder")
    if USE_TEXT:
        sketch.addText("- DATA +", x, BUTTON_Y_ORIGIN + STEP_BUTTON_V_INTERVAL * 3 + BUTTON_TEXT_UP, font_size=BUTTON_FONT_SIZE, anchor="middle", label="encoderText")

    # some advertisement
    if USE_TEXT:
        sketch.addText("YASS", 0.2, 1.95, font_size=TITLE_FONT_SIZE, anchor="middle", label="yassText")
        if USE_TEKO_BOX:
            width = TEKO_FRONT_PANNEL_WIDTH
        elif USE_CENTRAKOR_BOX:
            width = CENTRAKOR_FRONT_PANNEL_WIDTH
        else:
            width = FRONT_PANNEL_WIDTH
        sketch.addText("Yet Another Step Sequencer - MMXIX", -width / 2.0 + 0.2, 0, font_size=LEGEND_FONT_SIZE, anchor="middle", transform="-90", label="yet...Text")

if __name__ == "__main__":

    FNAME = os.path.basename(os.path.splitext(sys.argv[0])[0])
    
    IMG_PATH = "img"
    filename = "%s/%s.svg"%(IMG_PATH, FNAME)
    sketch = scriptSvg.SKETCH(filename, "in")
    
    # I don't know why drawing it out of the sheet, but I can fix it
    sketch.changeOrigin(6, -7.5)

    drawFrontPannel(sketch)
  
    sketch.save()
    sketch.makeCSV("img/%s_coordinates.csv"%FNAME)

