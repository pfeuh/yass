#!/usr/bin/python
# -*- coding: utf-8 -*-

import svgwrite
import sys

#~ if sys.version_info[0] < 3:
    #~ # python 2 or less
    #~ import Tkinter as gui
    #~ import tkMessageBox as gui_mb
    #~ import tkSimpleDialog as gui_sd
    #~ import tkFileDialog as gui_fd
    #~ import tkFont as gui_font
    #~ USING_PYTHON2 = True
#~ else:
    #~ # python 3 or more
    #~ import tkinter as gui
    #~ from tkinter import messagebox as gui_mb
    #~ from tkinter import simpledialog as gui_sd
    #~ from tkinter import filedialog as gui_fd
    #~ from tkinter import font as gui_font
    #~ USING_PYTHON2 = False

#~ IMAGE_MEMORY=[]
#~ win = gui.Tk()
#~ from PIL import ImageTk, Image
#~ img = gui.PhotoImage(file="img/durations.gif")
#~ IMAGE_MEMORY.append(img)

VERBOSE = "-verbose" in sys.argv
DRAFT = "-draft" in sys.argv

if VERBOSE:
    sys.stdout.write("VERBOSE mode active\n")
if DRAFT:
    sys.stdout.write("DRAFT mode active\n")

class SKETCH():
    def __init__(self, fname, unit="mm"):
        self.__sketch = svgwrite.Drawing(fname, profile='tiny')
        self.changeUnit(unit)
        self.__x_origin = 0.0
        self.__y_origin = 0.0
        self.__fname = fname
        
    def getSketch(self):
        return self.__sketch
        
    def cvrt(self, value):
        return float(value) * self.__coeff
        
    def changeUnit(self, unit):
        if unit ==  "mm":
            self.__coeff = 1.0
        elif unit == "cm":
            self.__coeff = 10.0
        elif unit == "in":
            self.__coeff = 25.4
        else:
            raise Exception("Not implemented unit \"%s\"\n"%self.__unit)
        self.__unit = unit

    def changeOrigin(self, x, y):
        self.__x_origin = self.cvrt(x)
        self.__y_origin = self.cvrt(y)
        
    def __setCutParameters(self, cut, **kwds):
        if cut and not DRAFT:
            kwds["stroke"] = "red"
            kwds["fill"] = "none"
            kwds["stroke_width"] = 0.01
        else:
            if not "stroke" in kwds.keys():
                kwds["stroke"] = "black"
            if not "fill" in kwds.keys():
                kwds["fill"] = "none"
            if not "stroke_width" in kwds.keys():
                kwds["stroke_width"] = 0.1
        return kwds
        
    def addRectangle(self, x, y, w, h, radius=0, cut=False, **kwds):
        x, y, w, h, radius = self.cvrt(x), self.cvrt(y), self.cvrt(w), self.cvrt(h), self.cvrt(radius)
        x = x - w / 2.0 
        y = 0.0 - y - h / 2.0 
        x += self.__x_origin
        y += self.__y_origin
        kwds = self.__setCutParameters(cut, **kwds)
        if VERBOSE:
            sys.stdout.write("rectangle cut=%s %f %f %f %f %s\n"%(cut, x, y, w, h, str(kwds)))
        shape = svgwrite.shapes.Rect(insert=(x, y),size=(w, h),rx=radius, ry=radius, **kwds)
        self.__sketch.add(shape)

    def addCircle(self, x, y, r, cut=False, **kwds):
        x, y, r = self.cvrt(x), 0.0 - self.cvrt(y), self.cvrt(r)
        x += self.__x_origin
        y += self.__y_origin
        kwds = self.__setCutParameters(cut, **kwds)
        if VERBOSE:
            sys.stdout.write("circle    cut=%s %f %f %f %s\n"%(cut, x, y, r, str(kwds)))
        shape = svgwrite.shapes.Circle(center=(x, y), r=r, **kwds)
        self.__sketch.add(shape)

    def addText(self, text, x, y, **kwds):
        x, y = self.cvrt(x), self.cvrt(y)
        y = 0.0 - y
        x += self.__x_origin
        y += self.__y_origin
        if "font_size" in kwds.keys():
            kwds["font_size"] =  self.cvrt(kwds["font_size"])
        else:
            kwds["font_size"] =  0.5
        kwds["text_anchor"] = "middle"
        y = y - kwds["font_size"] / 2.0
        shape = svgwrite.text.Text(text, insert=(x, y), **kwds)
        self.__sketch.add(shape)

    def save(self):
        self.__sketch.save()
        sys.stdout.write("file %s saved.\n"%self.__fname)

if __name__ == "__main__":

    def addText(text, x, y, **kwds):
        fs = float(kwds["font_size"])
        sketch.addText("text %u"%(idx), x, y)
        sketch.addRectangle(x, y,float(len(text)) * (fs / 2.0), fs, radius=0)


    sketch = SKETCH('test.svg', "in")
    
    LED_X_OFFSET = -2.975
    LED_RADIUS = 0.06
    LED_WIDTH = 0.35
    LED_HEIGHT = 1.1
    BUTTON_X_OFFSET = -2.95
    
    # let's compute front pannel border
    sketch.addRectangle(0, 0, 7.8, 5.2, 0.15, True)

    # let's compute step leds
    x = LED_X_OFFSET 
    y = -0.025
    r = LED_RADIUS
    for led_num in range(16):
        sketch.addCircle(x, y, r, True)
        x += LED_WIDTH

    for idx in range(5):
        x = 0.0
        y = float(idx) * 0.55
        addText("text %u"%(idx), x, y, font_size=0.2)

    sketch.save()

