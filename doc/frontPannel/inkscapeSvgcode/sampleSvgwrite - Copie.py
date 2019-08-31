#!/usr/bin/python
# -*- coding: utf-8 -*-

import svgwrite
import sys

class SKETCH():
    def __init__(self, fname, unit="mm"):
        self.__sketch = svgwrite.Drawing(fname, profile='tiny')
        self.changeUnit(unit)
        
    def cvrt(self, value):
        value = float(value)
        if self.__unit ==  "mm":
            return value
        elif self.__unit == "cm":
            return value * 10.0
        elif self.__unit == "in":
            return value * 25.4
        else:
            raise Exception("Not implemented unit \"%s\"\n"%self.__unit)
        
    def changeUnit(self, unit):
        self.__unit = unit
        
    def addRectangle(self, x, y, w, h, radius=0, cut=False, **kwds):
        x, y, w, h, radius = self.cvrt(x), self.cvrt(y), self.cvrt(w), self.cvrt(h), self.cvrt(radius)
        x = x - w / 2.0 
        y = 0.0 - y - h / 2.0 
        if cut:
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
                
        sys.stdout.write("%f %f %f %f cut=%s\n%s\n"%(x, y, w, h, cut, str(kwds)))
                
                
        item = svgwrite.shapes.Rect(insert=(x, y),size=(w, h),rx=radius, ry=radius, **kwds)
        self.__sketch.add(item)

    def save(self):
        self.__sketch.save()

sketch = SKETCH('test.svg', "mm")
sketch.addRectangle(0, 0, 200, 120, 5)
sketch.addRectangle(10, 10, 200, 120, 5, stroke="green")
sketch.changeUnit("in")
sketch.addRectangle(0, 0, 7, 5.8, 0.2, stroke="blue")
sketch.save()

#~ sketch.add(dwg.line((0, 0), (10, 0), stroke=svgwrite.rgb(10, 10, 16, '%')))
#~ sketch.add(dwg.text('Test', insert=(0, 0.2), fill='none', stroke="red", stroke_width="1"))
