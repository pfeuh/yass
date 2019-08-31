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
    
def drawButton(c, x, y, text):
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
    id1 = c.create_rectangle(x3, y3, x4, y4, outline="black")
    id2 = drawText(c, x, y, text=text)
    return id1, id2

def drawMarginButton(c, x, y, text):
    x = float(x)
    y = float(y)
    id1 = drawCircle(c, x + 15, y, "red", 20)
    x = x + 80
    y = y
    x1 = "%fp"%(x - 40)
    x2 = "%fp"%(x + 40)
    y1 = "%fp"%(y - 25)
    y2 = "%fp"%(y + 25)
    x3 = "%fp"%(x - 40 + 2)
    x4 = "%fp"%(x + 40 - 2)
    y3 = "%fp"%(y - 25 + 2)
    y4 = "%fp"%(y + 25 - 2)
    id2 = c.create_rectangle(x1, y1, x2, y2, outline="black")
    id3 = c.create_rectangle(x3, y3, x4, y4, outline="black")
    id4 = drawText(c, x, y, text=text)
    return id1, id2, id3, id4

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
    ["","","","","","","","","Internal","","","","","","","","","","","","","","","Midi","","","","","","","","","Clock"],
    #~ ["Noire","","","","Croche","","","","Triolet","","","","Double","","","","Sixtolet","","","","swing\nNoire","","","","swing\nCroche","","","","swing\nDouble","","","","Step\nduration"],
    ["","","img/blanche.gif","","","img/noire.gif","","","img/noire_triolet.gif","","","img/noire_swing.gif","","","img/croche.gif","","","img/triolet.gif","","","img/croche_swing.gif","","","img/double_croche.gif","","","img/double_croche_triolet.gif","","","img/double_croche_swing.gif","","","Step\nduration"],
    ["","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","Last\nstep"],
    ["","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","Record"],
    ]
    
    for index in range(32):
        cell_width = 30
        text_height = 55
        x = x_offset + cell_width * index
        y = y_offset
        drawCircle(canvas, x , y, "red", cell_width - 10)
        drawText(canvas, x, y - cell_width, text="%u"%(index+1))
        for linenum in range(len(data)):
            y_text = y + text_height * (linenum+1)
            text = data[linenum][index]
            if not os.path.isfile(text):
                drawText(canvas, x, y_text, text=text)
            else:
                #~ img = gui.PhotoImage(file=text)
                #~ IMAGE_MEMORY.append(img)
                #~ print(text)
                #~ drawCircle(canvas, x, y_text, "green", 3)
                #~ canvas.create_image(x + cell_width / 2, y_text + 92, image=img)
                drawImage(canvas, x, y_text, text)
            if index == 31:
                drawMarginButton(canvas, x + cell_width, y_text, text=data[linenum][32])
                if linenum in (0, 1, 2, 3):
                    y_line = y + 125 + linenum * 76
                    canvas.create_line(50, y_line, 1350, y_line)

def drawTransportGroup(x_offset, y_offset):
    for index, text in enumerate(("Start", "Stop", "Continue")):
        drawButton(canvas, x_offset + 80 * index, y_offset, text)

def drawSequenceSelectorGroup(x_offset, y_offset):
    for index, text in enumerate(("Seq.A", "Seq.B", "Seq.C", "Seq.D", )):
        x = x_offset + 80 * index
        y = y_offset
        drawButton(canvas, x, y, text)
        drawCircle(canvas, x, y-40, "red", 20)

def drawCommandGroup(x_offset, y_offset):
    for index, text in enumerate(("Erase\nnote", "Link\nnote", "Valid\nnote")):
        x = x_offset + 80 * index
        y = y_offset
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
    TEMP_Y = 280
    win.title("Y A S S ! Yet another step sequencer")
    win.bind("<Leave>", quitProperly)
    canvas = gui.Canvas(win, width=1800, height=800, bg="#F9F9F9")
    canvas.grid()

    drawLedGroup(60, 60)
    drawSequenceSelectorGroup(720, 150 + TEMP_Y)
    drawTransportGroup(80, 80 + TEMP_Y)
    drawCommandGroup(80, 150 + TEMP_Y)
    draw7SegmentGroup(355, 120 + TEMP_Y)
    drawEncoder(canvas, 585, 120 + TEMP_Y)
    canvas.create_text(1435, 65, font=NAME_FONT, text="Y A S S !\nYet another\nstep sequencer", justify="center")
    x=40
    y=10
    canvas.create_rectangle(x, y, x + 1496, y + 614)
    
    win.mainloop()
