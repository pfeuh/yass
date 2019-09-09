#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys

sys.stdout.write(sys.version + "\n")

import scriptSvg

#~ USE_TEXT = True
#~ NO_SCREW_HOLES = True
#~ MAKE_6_PANELS = False
#~ FIXATION_BIG = False
#~ ROUNDED_RECTANGLE = True
#~ MAKE_BOARD_FIXATION = True

USE_TEXT = True
NO_SCREW_HOLES = True
MAKE_6_PANELS = True
FIXATION_BIG = False
ROUNDED_RECTANGLE = True
MAKE_BOARD_FIXATION = True

def addButton(sketch, x, y, text):
    sketch.addCircle(x, y, BUTTON_RADIUS)
    if USE_TEXT:
        if text in ("GLOB", "SEQ."):
            sketch.addText(text, x + 0.25, y - 0.1, font_size=BUTTON_FONT_SIZE, anchor="start")
        elif text in ("OK", "REST", "TIE", "DATA"):
            sketch.addText(text, x, y + 0.5, font_size=BUTTON_FONT_SIZE, anchor="middle")
        else:
            sketch.addText(text, x, y + BUTTON_TEXT_Y_OFFSET, font_size=BUTTON_FONT_SIZE, anchor="middle")

def addDin5(sketch, x, y):
    # supposed to work in inches
    sketch.addCircle(x, y, 0.3)
    sketch.addCircle(x - 0.43, y, 0.063)
    sketch.addCircle(x + 0.43, y, 0.063)

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
                sketch.addCircle(x, y, SCREW_HOLE_RADIUS)

def doTopPannel(sketch):
    # let's compute front pannel border
    if ROUNDED_RECTANGLE:
        radius = 0.395
    else:
        radius = 0.0
    sketch.addRectangle(0, 0, TOP_WIDTH, TOP_HEIGHT, radius)
    addScrewHoles(sketch, TOP_WIDTH, TOP_HEIGHT, 4, 4)
        
    # some advertisement
    if USE_TEXT:
        sketch.addText("YASS", 0.2, 1.95, font_size=TITLE_FONT_SIZE, anchor="middle")
        sketch.addText("Yet Another Step Sequencer - c. MMXIX", 0, -2.45, font_size=LEGEND_FONT_SIZE, anchor="middle")

    # let's compute encoder's text
    sketch.addText("- DATA +", 1.86, 2.035, font_size=BUTTON_FONT_SIZE, anchor="middle")

    # let's compute step leds
    glob_text = ["In", "Out", "Prog. num.", "Arpeggiator", "Clock in", "Clock out", "Key click ", "Audio beat", "Accept sysEx", "Transposition", "Dump all", "Load all", "Save all", "Dump global", "Load global", "Save global"]
    seq_text = ["Groove", "Step duration", "Last step", "Data mode", "C. C. number", "Fixed velo. value", "", "", "", "", "Swap", "Copy", "Load fact. seq.", "Dump seq.", "Load seq.", "Save seq."]
    x = LED_X_OFFSET 
    y = -0.025
    r = LED_RADIUS
    for led_num in range(16):
        sketch.addCircle(x, y, r)
        if USE_TEXT:
            sketch.addText(glob_text[led_num], x + 0.05, y + 0.1, font_size = LED_FONT_SIZE, transform="-90")
            sketch.addText(seq_text[led_num], x + 0.05, y - 0.11, font_size = LED_FONT_SIZE, anchor="end", transform="-90")
        x += LED_WIDTH

    # let's compute individual leds
    x = LED_X_OFFSET 
    y = -1.4
    r = LED_RADIUS
    # led start
    sketch.addCircle(x, y, r)
    # led stop
    x += LED_WIDTH * 6.0
    sketch.addCircle(x, y, r)
    # leds sequences
    x += LED_WIDTH * 3.0
    for led_num in range(5):
        sketch.addCircle(x, y, r)
        x += LED_WIDTH * 2.0
    # leds sequence, global
    x -= LED_WIDTH * 2
    for led_num in range(2):
        y += LED_HEIGHT
        sketch.addCircle(x, y, r)
    # leds rest, tie, last step
    y += LED_HEIGHT
    x = LED_X_OFFSET
    for led_num in range(3):
        sketch.addCircle(x, y, r)
        x += LED_WIDTH * 2
        
    # let's compute buttons and texts
    w = 0.5
    h = 0.35
    r = 0.1
    x = BUTTON_X_OFFSET 
    y = -1.765
    # buttons start, stop, continue, record
    bt_transport_text = ["PLAY", "STOP", "PREV", "REC."]
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
    bt_transport_text = ["REST", "TIE", "DATA"]
    for button_num in range(3):
        addButton(sketch, x, y, bt_transport_text[button_num])
        x += LED_WIDTH * 2

    # let's compute 4x7 segments display
    sketch.addRectangle(0.2, 1.52, 1.582, 0.5, 0)

    # let's compute encoder button
    sketch.addCircle(1.850, 1.5, 0.29)
    
    # let's add 4 holes to fix board
    if MAKE_BOARD_FIXATION:
        if FIXATION_BIG:
            radius = 0.11
        else:
            radius = LED_RADIUS
        sketch.addCircle(-3.05, 0.80, radius)
        sketch.addCircle(-3.05, -1.10, radius)
        sketch.addCircle(3.15, 1.0, radius)
        sketch.addCircle(3.15, -1.2, radius)

    # let's add 4 holes to fix front pannel on box
    if MAKE_BOARD_FIXATION:
        radius = LED_RADIUS
        x = 3.588
        y = 2.238
        sketch.addCircle(-x, y, radius)
        sketch.addCircle(-x, -y, radius)
        sketch.addCircle(x, y, radius)
        sketch.addCircle(x, -y, radius)

def doBackPannel(sketch):
    # let's compute back pannel
    sketch.changeOrigin(0, 0.0 - (TOP_HEIGHT * 0.5 + FRONT_HEIGHT * 0.5))
    sketch.addRectangle(0, 0, FRONT_WIDTH, FRONT_HEIGHT, 0)
    addScrewHoles(sketch, FRONT_WIDTH, FRONT_HEIGHT - BOX_THICKNESS * 2.0, 4, 3)
    y = 0.35
    addDin5(sketch, 0, 0)
    addDin5(sketch, FRONT_WIDTH * 0.25, 0)
    sketch.addCircle(0.0 - FRONT_WIDTH * 0.25, 0, 0.215)
    if USE_TEXT:
        sketch.addText("MIDI IN", 0, y, font_size=BUTTON_FONT_SIZE, anchor="middle", transform="180")
        sketch.addText("MIDI OUT", FRONT_WIDTH * 0.25, y, font_size=BUTTON_FONT_SIZE, anchor="middle", transform="180")
        sketch.addText("POWER", 7*-LED_WIDTH, y, font_size=BUTTON_FONT_SIZE, anchor="middle", transform="180")

def doFrontPannel(sketch):
    # let's compute front pannel
    sketch.addRectangle(0, 0, FRONT_WIDTH, FRONT_HEIGHT, 0)
    addScrewHoles(sketch, FRONT_WIDTH, FRONT_HEIGHT - BOX_THICKNESS * 2.0, 4, 3)

def doLeftPannel(sketch):
    # let's compute left pannel
    sketch.addRectangle(0, 0, LEFT_HEIGHT, LEFT_WIDTH, 0)
    addScrewHoles(sketch, LEFT_HEIGHT - BOX_THICKNESS * 2.0, LEFT_WIDTH, 3, 4)

def doRightPannel(sketch):
    # let's compute right pannel
    sketch.addRectangle(0, 0, LEFT_HEIGHT, LEFT_WIDTH, 0)
    addScrewHoles(sketch, LEFT_HEIGHT - BOX_THICKNESS * 2.0, LEFT_WIDTH, 3, 4)

def doBottomPannel(sketch):
    # let's compute bottom pannel
    sketch.addRectangle(0, 0, TOP_HEIGHT, TOP_WIDTH, 0)
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
    
    BUTTON_FONT_SIZE = 1.5
    LED_FONT_SIZE = 1.0
    TITLE_FONT_SIZE = 4.0
    LEGEND_FONT_SIZE = 0.8
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
    
    FNAME = "YassFrontPannel"
    IMG_PATH = "img"
    draftText = ""
    filename = "%s/%s%s.svg"%(IMG_PATH, draftText, FNAME)
    
    coordinates = {}
    
    sketch = scriptSvg.SKETCH(filename, "in")
    
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

