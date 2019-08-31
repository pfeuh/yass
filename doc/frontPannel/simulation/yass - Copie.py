#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import os
IMAGE_MEMORY=[]
from PIL import ImageTk, Image
import classObject


import platform
print(platform.python_version())
print(platform.architecture())

if sys.version_info[0] < 3:
    # python 2 or less
    import Tkinter as gui
    import tkMessageBox as gui_mb
    import tkSimpleDialog as gui_sd
    import tkFileDialog as gui_fd
    import tkFont as gui_font
    USING_PYTHON2 = True
else:
    # python 3 or more
    import tkinter as gui
    from tkinter import messagebox as gui_mb
    from tkinter import simpledialog as gui_sd
    from tkinter import filedialog as gui_fd
    from tkinter import font as gui_font
    USING_PYTHON2 = False

OBJECTS = []

DRAW_COLOR = "white"
DRAW_BACKCOLOR = "black"
#~ DRAW_COLOR = "black"
#~ DRAW_BACKCOLOR = "white"

DEFAULT_LED_COLOR = "red"
RED_LED_COLOR = DEFAULT_LED_COLOR
GREEN_LED_COLOR = "green"
BLUE_LED_COLOR = "blue"
YELLOW_LED_COLOR = "yellow"

PPI = 96.0
DRAW_SCALE = 1.6

WIDTH = 7.0
HEIGHT = 5.25
COLUMN_START = 0.5
COLUMN_WIDTH = 0.35
ROW_START = 1.5
ROW_HEIGHT = 0.3
LED_DIAMETER = 0.12
DISPLAY_HEIGHT = 0.5
DISPLAY_WIDTH = 1.6
ENCODER_DIAMETER = DISPLAY_HEIGHT * 2.0

KEY_WIDTH = 0.55
KEY_HEIGHT = 0.45
KEY_COLUMN = 8
DATA_INPUT_COLUMN = -1

DATA_SEQ = ("A", "B", "C", "D", "E")
DATA_GLOBAL = "REST", "TIE", "LAST\nSTEP"
DATA_RIGHT = ("ENTER", "GLOBAL\nPARAM.", "SEQ.\nPARAM.")
DATA_TRANSPORT = ("PLAY", "STOP", "CONT", "REC.")
DATA_LINE2=("MIDI\nIN", "MIDI\nOUT", "", "CLOCK", "", "CLOCK\nOUT", "", "ACTIVE\nSENSING", "", "VELOCITY\nROUTING", "", "CLICK", "", "KEY\nECHO", "", "SEND\nDUMP")
DATA_LINE_3 = ("4th", "8th", "Swing", "Triplet", "16th", "", "SHORT", "", "NORMAL","", "LEGATO", "", "LOOP", "ONE\nSHOT", "", "SEND\nDUMP")
objects = []

def drawRectangle(c, x, y, w, h):
    object = classObject.RECTANGLE(x, y, w, h)
    id = object.draw(c, DRAW_SCALE, outline=DRAW_COLOR)
    objects.append(object)
    return id

def drawCircle(c, x, y, d, color=None):
    kwds = {"outline":DRAW_COLOR }
    if color != None:
        kwds["fill"] = color 
    object = classObject.CIRCLE(x, y, d)
    id = object.draw(c, DRAW_SCALE, **kwds)
    objects.append(object)
    return id

def drawText(c, x, y, text, font=None, fill=None):
    if fill == None:
        fill = DRAW_COLOR
    object = classObject.TEXT(x, y, text)
    #~ id = object.draw(c, DRAW_SCALE, fill=fill, font=font, angle=90)
    id = object.draw(c, DRAW_SCALE, fill=fill, font=font)
    objects.append(object)
    return id

def getCellCoordinates(column, row):
    x = COLUMN_START + float(column) * COLUMN_WIDTH
    y = ROW_START + float(row) * ROW_HEIGHT
    return x, y

def drawLed(c, x, y, color=DEFAULT_LED_COLOR ):
    drawCircle(c, x, y, LED_DIAMETER, color)
    
def drawKey(c, x, y, text="???", font=None):
    drawRectangle(c, x, y, KEY_WIDTH, KEY_HEIGHT)
    kwds = {}
    if font != None:
        kwds["font"] = font
    drawText(c, x, y, text, **kwds)

def drawKeyLed(c, x, y, text="???", color=DEFAULT_LED_COLOR, font=None):
    drawKey(c, x, y, text, font)
    drawLed(c, x, y - ROW_HEIGHT * 1.2, color)
    #~ drawLed(c, x, y - ROW_HEIGHT * 1.5, color)

def drawFrame(c, column, row):
    return
    x, y = getCellCoordinates(column, row)
    drawRectangle(c, x, y, COLUMN_WIDTH, ROW_HEIGHT)

#-------------------
#- now the batches -
#-------------------

def drawStepLeds(canvas):
    for index in range(16):
        x, y = getCellCoordinates(index, 3.5)
        drawLed(canvas, x, y)
        
def drawTransportKeys(canvas):
    for index, text in enumerate(DATA_TRANSPORT):
        x, y = getCellCoordinates(index*2, KEY_COLUMN)
        if index in (0,3):
            if index == 0:
                drawKeyLed(canvas, x, y, text, GREEN_LED_COLOR, TRANSPORT_FONT)
            else:
                drawKeyLed(canvas, x, y, text, RED_LED_COLOR, TRANSPORT_FONT)
        else:
            drawKey(canvas, x, y, text, TRANSPORT_FONT)
        
def drawSequenceKeys(canvas):
    for index, text in enumerate(DATA_SEQ):
        x, y = getCellCoordinates(index*2 + 9, KEY_COLUMN)
        drawKeyLed(canvas, x, y, text, BLUE_LED_COLOR, SEQ_FONT)

def drawGlobalKeys(canvas):
    for index, text in enumerate((DATA_GLOBAL)):
        x, y = getCellCoordinates(index*2, -1)
        drawKeyLed(canvas, x, y, text, YELLOW_LED_COLOR, CONFIG_FONT)

def drawRightKeys(canvas):
    for index, text in enumerate(DATA_RIGHT):
        x, y = getCellCoordinates(17, -1 + index * 3)
        if index:
            drawKeyLed(canvas, x, y, text, YELLOW_LED_COLOR, CONFIG_FONT)
        else:
            drawKey(canvas, x, y, text, CONFIG_FONT)

def drawDisplay(canvas):
    x, y = getCellCoordinates( 9, DATA_INPUT_COLUMN)
    drawRectangle(canvas, x, y, DISPLAY_WIDTH, DISPLAY_HEIGHT)
    drawText(canvas, x, y, "DISPLAY 4 DIGITS - 7 SEGMENTS")
    drawText(canvas, x - DISPLAY_WIDTH / 4.0 , y - DISPLAY_HEIGHT / 2.0 - 0.05, "VELO.", LEGEND_FONT)
    drawText(canvas, x + DISPLAY_WIDTH / 4.0 , y - DISPLAY_HEIGHT / 2.0 - 0.05, "NOTE", LEGEND_FONT)
    
def drawEncoder(canvas):
    x, y = getCellCoordinates(14, DATA_INPUT_COLUMN)
    drawCircle(canvas, x, y, ENCODER_DIAMETER)
    drawCircle(canvas, x, y, ENCODER_DIAMETER + 0.03)
    drawText(canvas, x, y, "DATA\nENCODER")
    y -= ENCODER_DIAMETER * 0.3
    drawCircle(canvas, x, y, ENCODER_DIAMETER * 0.3)

def drawLine2(canvas):
    for index, text in enumerate(DATA_LINE2):
        x, y = getCellCoordinates(index, 2)
        drawText(canvas, x, y, text, LEGEND_FONT)
        if text != "":
            drawFrame(canvas, index, 2)

def drawLine3(canvas):
    for index, text in enumerate(DATA_LINE_3):
        x, y = getCellCoordinates(index, 5)
        #~ font = LEGEND_FONT
        #~ if index < 5:
        #~ font = RESOLUTION_FONT
        drawText(canvas, x, y, text, LEGEND_FONT)
        if text != "":
            drawFrame(canvas, index, 5)

def drawBigYass(canvas):
    for index, text in enumerate(("Y", "A", "S", "S")):
        x, y = getCellCoordinates(index*2, KEY_COLUMN + 2)
        color = (RED_LED_COLOR, GREEN_LED_COLOR, BLUE_LED_COLOR, YELLOW_LED_COLOR)[index]
        drawText(canvas, x, y, text, SEQ_FONT, color)

def drawSmallYass(canvas):
        x, y = getCellCoordinates(13, KEY_COLUMN + 2)
        drawText(canvas, x, y, "Yet Another Step Sequencer!", YASS_SMALL_FONT)

def quitProperly(event):
    win.quit()

if __name__ == "__main__":

    win = gui.Tk()
    print(gui.TclVersion)
    win.geometry("+0+0")
    SEQ_FONT = gui_font.Font(size=32, weight='bold')
    TRANSPORT_FONT = gui_font.Font(size=20, weight='bold')
    CONFIG_FONT = gui_font.Font(size=14, weight='bold')
    LEGEND_FONT = gui_font.Font(size=12, slant='italic')
    YASS_SMALL_FONT = gui_font.Font(size=22, slant='italic')
    win.title("Y A S S !   Yet Another Step Sequencer")
    win.bind("<Escape>", quitProperly)
    
    canvas = gui.Canvas(win, width=WIDTH*PPI*DRAW_SCALE, height=HEIGHT*PPI*DRAW_SCALE, bg=DRAW_BACKCOLOR)
    canvas.grid()

    drawRectangle(canvas, WIDTH/2.0, HEIGHT/2.0, WIDTH, HEIGHT)
    drawStepLeds(canvas)
    drawTransportKeys(canvas)
    drawSequenceKeys(canvas)
    drawGlobalKeys(canvas)
    drawRightKeys(canvas)
    drawDisplay(canvas)
    drawEncoder(canvas)
    drawLine2(canvas)
    drawLine3(canvas)
    drawBigYass(canvas)    
    drawSmallYass(canvas)    

    win.mainloop()
