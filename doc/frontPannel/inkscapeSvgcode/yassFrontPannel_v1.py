#!/usr/bin/python
# -*- coding: utf-8 -*-

# TODO: in lasercut.py class SKETCH, centered text is not really centered

import sys

print(sys.version)  # parentheses necessary in python 3.       

import lasercut

#~ NO_TEXT = False
#~ NO_SCREW_HOLES = False
#~ MAKE_6_PANELS = True
#~ MAKE_BOARD_FIXATION = True

NO_TEXT = True
NO_SCREW_HOLES = True
MAKE_6_PANELS = False
MAKE_BOARD_FIXATION = False
FIXATION_BIG = False
ROUNDED_RECTANGLE = True

def addButton(sketch, x, y, text):
    if BUTTON_STYLE == BUTTON_STYLE_ROUNDED_RECTANGLE:
        sketch.addRectangle(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_CORNER_RADIUS, True)
    elif BUTTON_STYLE == BUTTON_STYLE_CIRCLE:
        sketch.addCircle(x, y, BUTTON_RADIUS, True)
    if not NO_TEXT:
        sketch.addText(text, x, y + BUTTON_TEXT_Y_OFFSET, font_size=FONT_SIZE)

def addDin5(sketch, x, y):
    # supposed to work in inches
    sketch.addCircle(x, y, 0.3, True)
    sketch.addCircle(x - 0.43, y, 0.063, True)
    sketch.addCircle(x + 0.43, y, 0.063, True)

def addScrewHoles(sketch, w, h, nb_cols, nb_rows):
    if NO_SCREW_HOLES:
        return
    # supposed to work in inches
    w, h = float(w), float(h)
    h -= SCREW_OFFSET * 2.0
    w -= SCREW_OFFSET * 2.0
    col_width = w / (float(nb_cols) - 1.0)
    row_height = h / (float(nb_rows) - 1.0)
    for x_idx in range(nb_cols):
        for y_idx in range(nb_rows):
            x = 0.0 - w * 0.5 + float(x_idx) * col_width
            y = 0.0 - h * 0.5 + float(y_idx) * row_height
            if x_idx in (0, nb_cols - 1) or y_idx in (0, nb_rows - 1):
                sketch.addCircle(x, y, SCREW_HOLE_RADIUS, True)

def doTopPannel(sketch):
    # let's compute front pannel border
    if ROUNDED_RECTANGLE:
        radius = 0.15
    else:
        radius = 0.0
    sketch.addRectangle(0, 0, TOP_WIDTH, TOP_HEIGHT, radius, True)
    addScrewHoles(sketch, TOP_WIDTH, TOP_HEIGHT, 4, 4)
        
    # some advertisement
    if not NO_TEXT:
        sketch.addText("YASS", 0.2, 1.80, font_size=0.5)
        sketch.addText("Yet Another Step Sequencer", 2.5, 2.20, font_size=FONT_SIZE * 0.75)
        sketch.addText("c. MMXIX", 2.5, 2.05, font_size=FONT_SIZE * 0.75)

    # let's compute step leds
    x = LED_X_OFFSET 
    y = -0.025
    r = LED_RADIUS
    for led_num in range(16):
        sketch.addCircle(x, y, r, True)
        x += LED_WIDTH

    # let's compute individual leds
    x = LED_X_OFFSET 
    y = -1.4
    r = LED_RADIUS
    # led start
    sketch.addCircle(x, y, r, True)
    # led stop
    x += LED_WIDTH * 6.0
    sketch.addCircle(x, y, r, True)
    # leds sequences
    x += LED_WIDTH * 3.0
    for led_num in range(5):
        sketch.addCircle(x, y, r, True)
        x += LED_WIDTH * 2.0
    # leds sequence, global
    x -= LED_WIDTH * 2
    for led_num in range(2):
        y += LED_HEIGHT
        sketch.addCircle(x, y, r, True)
    # leds rest, tie, last step
    y += LED_HEIGHT
    x = LED_X_OFFSET
    for led_num in range(3):
        sketch.addCircle(x, y, r, True)
        x += LED_WIDTH * 2
        
    # let's compute buttons and texts
    w = 0.5
    h = 0.35
    r = 0.1
    x = BUTTON_X_OFFSET 
    y = -1.765
    # buttons start, stop, continue, record
    bt_transport_text = ["PLAY", "STOP", "CONT", "REC."]
    for button_num in range(4):
        addButton(sketch, x, y, bt_transport_text[button_num])
        x += LED_WIDTH * 2.0
    # buttons sequences 1...5
    x += LED_WIDTH
    bt_transport_text = ["1", "2", "3", "4", "5"]
    for button_num in range(5):
        addButton(sketch, x, y, bt_transport_text[button_num])
        x += LED_WIDTH * 2
    # buttons sequence, global, return
    x -= 0.7
    bt_transport_text = ["SEQ.", "GLOB", "OK"]
    for button_num in range(3):
        y += LED_HEIGHT
        addButton(sketch, x, y, bt_transport_text[button_num])
    # buttons rest, tie, last step
    x = BUTTON_X_OFFSET
    bt_transport_text = ["REST", "TIE", "LAST"]
    for button_num in range(3):
        addButton(sketch, x, y, bt_transport_text[button_num])
        x += LED_WIDTH * 2

    # let's compute 4x7 segments display
    sketch.addRectangle(0.2, 1.52, 1.582, 0.5, 0, True)

    # let's compute encoder button
    sketch.addCircle(1.850, 1.5, 0.29, True)
    
    # let's add 4 holes to fix board
    if MAKE_BOARD_FIXATION:
        if FIXATION_BIG:
            radius = 0.11
        else:
            radius = LED_RADIUS
        sketch.addCircle(-3.05, 0.80, radius, True)
        sketch.addCircle(-3.05, -1.10, radius, True)
        sketch.addCircle(3.15, 1.0, radius, True)
        sketch.addCircle(3.15, -1.2, radius, True)

def doBackPannel(sketch):
    # let's compute back pannel
    sketch.changeOrigin(0, 0.0 - (TOP_HEIGHT * 0.5 + FRONT_HEIGHT * 0.5))
    sketch.addRectangle(0, 0, FRONT_WIDTH, FRONT_HEIGHT, 0, True)
    addScrewHoles(sketch, FRONT_WIDTH, FRONT_HEIGHT - BOX_THICKNESS * 2.0, 4, 3)
    y = 0.35
    addDin5(sketch, 0, 0)
    addDin5(sketch, FRONT_WIDTH * 0.25, 0)
    sketch.addCircle(0.0 - FRONT_WIDTH * 0.25, 0, 0.215, True)
    if not NO_TEXT:
        sketch.addText("MIDI IN", 0, y, font_size=FONT_SIZE)
        sketch.addText("MIDI OUT", FRONT_WIDTH * 0.25, y, font_size=FONT_SIZE)
        sketch.addText("POWER", 0.0 - FRONT_WIDTH * 0.25, y, font_size=FONT_SIZE)

def doFrontPannel(sketch):
    # let's compute front pannel
    sketch.addRectangle(0, 0, FRONT_WIDTH, FRONT_HEIGHT, 0, True)
    addScrewHoles(sketch, FRONT_WIDTH, FRONT_HEIGHT - BOX_THICKNESS * 2.0, 4, 3)

def doLeftPannel(sketch):
    # let's compute left pannel
    sketch.addRectangle(0, 0, LEFT_HEIGHT, LEFT_WIDTH, 0, True)
    addScrewHoles(sketch, LEFT_HEIGHT - BOX_THICKNESS * 2.0, LEFT_WIDTH, 3, 4)

def doRightPannel(sketch):
    # let's compute right pannel
    sketch.addRectangle(0, 0, LEFT_HEIGHT, LEFT_WIDTH, 0, True)
    addScrewHoles(sketch, LEFT_HEIGHT - BOX_THICKNESS * 2.0, LEFT_WIDTH, 3, 4)

def doBottomPannel(sketch):
    # let's compute bottom pannel
    sketch.addRectangle(0, 0, TOP_HEIGHT, TOP_WIDTH, 0, True)
    addScrewHoles(sketch, TOP_HEIGHT, TOP_WIDTH, 4, 4)
    
if __name__ == "__main__":

    SCREW_OFFSET = 0.2
    SCREW_HOLE_RADIUS = 0.061
    LED_X_OFFSET = -2.975
    LED_RADIUS = 0.06
    LED_WIDTH = 0.35
    LED_HEIGHT = 1.1

    BUTTON_STYLE_ROUNDED_RECTANGLE = 1
    BUTTON_STYLE_CIRCLE = 2
    BUTTON_STYLE = BUTTON_STYLE_CIRCLE
    BUTTON_WIDTH = 0.463
    BUTTON_HEIGHT = 0.35
    BUTTON_CORNER_RADIUS = 0.1
    BUTTON_RADIUS = BUTTON_WIDTH * 0.5
    BUTTON_X_OFFSET = -2.95
    BUTTON_TEXT_Y_OFFSET = -0.50
    
    FONT_SIZE = 0.16
    TOP_WIDTH = 7.8
    TOP_HEIGHT = 5.2
    FRONT_WIDTH = TOP_WIDTH
    BOARD_THICKNESS = 0.063
    BOX_THICKNESS = 0.2
    UPPER_VOID_THICKNESS = 0.4
    LOWER_VOID_THICKNESS = 0.8
    SUPPORT_THICKNESS = 0.4
    FRONT_HEIGHT = BOARD_THICKNESS + UPPER_VOID_THICKNESS + LOWER_VOID_THICKNESS + SUPPORT_THICKNESS + BOX_THICKNESS * 2.0 
    LEFT_WIDTH = TOP_HEIGHT + 2.0 * BOX_THICKNESS
    LEFT_HEIGHT = FRONT_HEIGHT
    LEFT_WIDTH = TOP_HEIGHT + 2.0 * BOX_THICKNESS
    LEFT_HEIGHT = FRONT_HEIGHT
    
    FNAME = "Yass_v5"

    NO_TEXT = True
    NO_SCREW_HOLES = True
    MAKE_6_PANELS = False
    MAKE_BOARD_FIXATION = True

    IMG_PATH = "img"

    for lasercut.DRAFT in (True, False):
        draftText = ""
        if lasercut.DRAFT:
            draftText = "_draft"

        filename = "%s/%s%s.svg"%(IMG_PATH, draftText, FNAME)
        
        sketch = lasercut.SKETCH(filename, "in")
        
        doTopPannel(sketch)
      
        if MAKE_6_PANELS:
            sketch.changeOrigin(0, 0.0 - (TOP_HEIGHT * 0.5 + FRONT_HEIGHT * 0.5))
            doBackPannel(sketch)

            sketch.changeOrigin(0, TOP_HEIGHT * 0.5 + FRONT_HEIGHT * 0.5)
            doFrontPannel(sketch)
            
            sketch.changeOrigin(0.0 - (TOP_WIDTH * 0.5 + LEFT_HEIGHT * 0.5), 0)
            doLeftPannel(sketch)

            sketch.changeOrigin(TOP_WIDTH * 0.5 + LEFT_HEIGHT * 0.5, 0)
            doRightPannel(sketch)
                
            sketch.changeOrigin(0.0 - (TOP_WIDTH * 0.5 + LEFT_HEIGHT + TOP_HEIGHT * 0.5), 0)
            doBottomPannel(sketch)
        
        sketch.save()

