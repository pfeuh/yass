#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import os
IMAGE_MEMORY=[]
from PIL import ImageTk, Image

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

def drawCircle(c, x, y, color, diameter):
    x = float(x)
    y = float(y)
    diameter = float(diameter)
    rayon = diameter / 2.0
    x1 = "%fp"%(x - rayon)
    x2 = "%fp"%(x + rayon)
    y1 = "%fp"%(y - rayon)
    y2 = "%fp"%(y + rayon)
    return c.create_oval(x1, y1, x2, y2, fill=color)

def drawImage(c, x, y, fname):
    img = gui.PhotoImage(file=fname)
    
    #image has to be be saved somewhere because the garbage collector destroys it
    IMAGE_MEMORY.append(img)
    
    return canvas.create_image("%fp"%x, "%fp"%y, image=img)

def drawEncoder(c, x, y):
    drawCircle(c, x, y, "light blue", 100)
    drawCircle(c, x, y -25, "light blue", 20)

def drawText(c, x, y, **kwds):
    if not "font" in kwds.keys():
        kwds["font"] = TEXT_FONT
    if not "fill" in kwds.keys():
        kwds["fill"] = "black"
    if not "justify" in kwds.keys():
        kwds["justify"] = "center"
    if not "text" in kwds.keys():
        kwds["text"] = ""
    x = "%fp"%(float(x))
    y = "%fp"%(float(y))
    return c.create_text(x, y, **kwds)
    
def drawPower(c, x, y):
    print("%4u %4u"%(x, y))
    drawCircle(c, x, y, "black", 60)
    drawCircle(c, x, y, "red", 50)
    drawText(c, x, y, text="POWER", font=NAME_FONT, fill="white")

def drawRectangle(c, x, y, w, h, **kwds):
    c.create_rectangle("%fp"%x, "%fp"%y, "%fp"%(x + w), "%fp"%(y + h))
    
def drawHorizontalLine(c, x1, x2, y, **kwds):
    c.create_line("%fp"%x1, "%fp"%y, "%fp"%x2, "%fp"%y, *kwds)
    
def drawButton(c, x, y, text, **kwds):
    x = float(x)
    y = float(y)
    x1 = "%fp"%(x - 40)
    x2 = "%fp"%(x + 40)
    y1 = "%fp"%(y - 25)
    y2 = "%fp"%(y + 25)
    x3 = "%fp"%(x - 40 + 2)
    x4 = "%fp"%(x + 40 - 2)
    y3 = "%fp"%(y - 25 + 2)
    y4 = "%fp"%(y + 25 - 2)
    id1 = c.create_rectangle(x1, y1, x2, y2, outline="black")
    id1 = c.create_rectangle(x3, y3, x4, y4, outline="black", fill="light grey")
    id2 = drawText(c, x, y, text=text, **kwds)
    return id1, id2

def drawMarginButton(c, x, y, color, text):
    id3 = drawCircle(c, x + 15, y, color, 20)
    id1, id2 = drawButton(c, x + 80, y, text) 
    return id1, id2, id3


def draw7Segment(c, x, y, text):
    x = float(x)
    y = float(y)
    x1 = "%fp"%(x - 25)
    x2 = "%fp"%(x + 25)
    y1 = "%fp"%(y - 40)
    y2 = "%fp"%(y + 40)
    x3 = "%fp"%(x - 25 + 2)
    x4 = "%fp"%(x + 25 - 2)
    y3 = "%fp"%(y - 40 + 2)
    y4 = "%fp"%(y + 40 - 2)
    id1 = c.create_rectangle(x1, y1, x2, y2, fill="black")
    id2 = drawText(c, x, y, text=text, font=SEG7_FONT, fill="red")
    return id1, id2

def drawLedGroup(x_offset, y_offset):
    data = [
    ["1","","2","","3","","4","","5","","6","","7","","8","","9","","10","","11","","12","","13","","14","","15","","16","omni","Midi in\nchannel"],
    ["1","","2","","3","","4","","5","","6","","7","","8","","9","","10","","11","","12","","13","","14","","15","","16","","Midi out\nchannel"],
    ["","","","","Internal\nclock","","","","","","","","Midi\nclock","","","","","","","","Clock\nsending","","","","","","","","No clock\nsending","","","","Clock"],
    ["img/blanche.gif","","img/noire.gif","","img/noire_triolet.gif","","img/noire_swing.gif","","img/croche.gif","","img/triolet.gif","","img/croche_swing.gif","","img/double_croche.gif","","img/double_croche_triolet.gif","","img/double_croche_swing.gif","","","","Staccato","","","","Detached","","","","Legato","","Step\nduration"],
    ["","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","Last\nstep"],
    ["","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","Record"],
    ]

    for index in range(32):
        cell_width = 30
        text_height = 55
        x = x_offset + cell_width * index
        y = y_offset
        if not index:
            drawCircle(canvas, x , y, "red", cell_width - 10)
        else:
            drawCircle(canvas, x , y, "dark red", cell_width - 10)
        drawText(canvas, x, y - cell_width, text="%u"%(index+1))
        for linenum in range(len(data)):
            y_text = y + text_height * (linenum+1)
            text = data[linenum][index]
            if not os.path.isfile(text):
                drawText(canvas, x, y_text, text=text)
            else:
                drawImage(canvas, x, y_text, text)
            if index == 31:
                color = "dark orange"
                if linenum == 5:
                    color = "dark red"
                if linenum in (3, 4):
                    color = "#C0C000" # dark yellow
                drawMarginButton(canvas, x + cell_width, y_text, color, text=data[linenum][32])

def drawTransportGroup(x_offset, y_offset):
    for index, text in enumerate(("Start", "Stop", "Continue")):
        drawButton(canvas, x_offset + 80 * index, y_offset, text)

def drawSequenceSelectorGroup(x_offset, y_offset):
    for index, text in enumerate(("A", "B", "C", "D", )):
        x = x_offset + 80 * index
        y = y_offset
        drawButton(canvas, x, y, text, font=SEQNAME_FONT)
        if not index:
            drawCircle(canvas, x, y-40, "green", 20)
        else:
            drawCircle(canvas, x, y-40, "dark green", 20)

def drawCommandGroup(x_offset, y_offset):
    for index, text in enumerate(("Erase", "Link", "Valid")):
        x = x_offset + 80 * index
        y = y_offset
        drawButton(canvas, x, y, text)
        drawButton(canvas, x, y, text)

def draw7SegmentGroup(x_offset, y_offset):
    for index, text in enumerate(("1", "2", "0")):
        draw7Segment(canvas, x_offset + 52 * index, y_offset, text)
    
def quitProperly(event):
    win.quit()

if __name__ == "__main__":

    win = gui.Tk()
    #~ for family in gui_font.families():
        #~ print(str(family))
    TEXT_FONT = gui_font.Font(family='helvetica', size=14, weight='bold')
    SEG7_FONT = gui_font.Font(family='courier', size=78)
    NAME_FONT = gui_font.Font(family='courier', size=12, slant='italic', weight="bold")
    SEQNAME_FONT = gui_font.Font(family='helvetica', size=32, weight="bold")
    TEMP_Y = 280
    win.title("Y A S S ! Yet another step sequencer")
    #~ win.bind("<Leave>", quitProperly)
    canvas = gui.Canvas(win, width=1800, height=800, bg="#F9F9F9")
    canvas.grid()

    drawLedGroup(60, 60)
    drawSequenceSelectorGroup(720, 150 + TEMP_Y)
    drawTransportGroup(80, 80 + TEMP_Y)
    drawCommandGroup(80, 150 + TEMP_Y)
    draw7SegmentGroup(355, 120 + TEMP_Y)
    drawEncoder(canvas, 585, 120 + TEMP_Y)
    
    drawPower(canvas, 1100, 50)
    drawText(canvas, 22, 170, font=NAME_FONT, text="G\nL\nO\nB\nA\nL", justify="center")
    drawText(canvas, 840, 336, font=NAME_FONT, text="Y A S S ! Yet another step sequencer", justify="center")
    drawRectangle(canvas, 10, 10, 1160, 465) # front pannel area
    drawRectangle(canvas, 35, 90, 980, 160)  # global settings
    drawHorizontalLine(canvas, 35, 1016, 140)
    drawHorizontalLine(canvas, 35, 1016, 196)
    
    win.mainloop()
