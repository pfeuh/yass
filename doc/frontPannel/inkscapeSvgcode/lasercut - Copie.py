#!/usr/bin/python
# -*- coding: utf-8 -*-

import svgwrite
import sys


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
        kwds = self.__setCutParameters(cut, **kwds)
        if VERBOSE:
            sys.stdout.write("rectangle cut=%s %f %f %f %f %s\n"%(cut, x, y, w, h, str(kwds)))
        shape = svgwrite.shapes.Rect(insert=(x, y),size=(w, h),rx=radius, ry=radius, **kwds)
        self.__sketch.add(shape)

    def addCircle(self, x, y, r, cut=False, **kwds):
        x, y, r = self.cvrt(x), 0.0 - self.cvrt(y), self.cvrt(r)
        kwds = self.__setCutParameters(cut, **kwds)
        if VERBOSE:
            sys.stdout.write("circle    cut=%s %f %f %f %s\n"%(cut, x, y, r, str(kwds)))
        shape = svgwrite.shapes.Circle(center=(x, y), r=r, **kwds)
        self.__sketch.add(shape)

    def addText(self, text, x, y, cut=False, **kwds):
        x, y = self.cvrt(x), self.cvrt(y)
        #~ x = x - w / 2.0 
        #~ y = 0.0 - y - h / 2.0
        kwds = self.__setCutParameters(cut, **kwds)
        shape = svgwrite.text.Text(text, insert=(x, y), **kwds)
        self.__sketch.add(shape)

    def save(self):
        self.__sketch.save()

if __name__ == "__main__":

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
        
    # let's compute buttons
    w = 0.5
    h = 0.35
    r = 0.1
    x = BUTTON_X_OFFSET 
    y = -1.765
    # buttons start, stop, continue, record
    for button_num in range(4):
        sketch.addRectangle(x, y, w, h, r, True)
        x += LED_WIDTH * 2.0
    # buttons sequences 1...5
    x += LED_WIDTH
    for button_num in range(5):
        sketch.addRectangle(x, y, w, h, r, True)
        x += LED_WIDTH * 2
    # buttons sequence, global, return
    x -= 0.7
    for button_num in range(3):
        y += LED_HEIGHT
        sketch.addRectangle(x, y, w, h, r, True)
    # buttons rest, tie, last step
    x = BUTTON_X_OFFSET
    for button_num in range(3):
        sketch.addRectangle(x, y, w, h, r, True)
        x += LED_WIDTH * 2

    # let's compute 4x7 segments
    sketch.addRectangle(0.2, 1.52, 1.582, 0.5, 0, True)

    # let's compute encoder button
    sketch.addCircle(1.850, 1.5, 0.350, True)

    #~ sketch.addText("TotoTitiTata", 0, 0 ,font_size=2)



    sketch.save()

