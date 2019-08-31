#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import os
import svgwrite

def addLed(sketch, x, y, r):
    sys.stdout.write( "led %f %f %f\n"%(x, y, r))
    shape = svgwrite.shapes.Circle(center=("%1.3fin"%x, "%1.3fin"%y),
        stroke_width = "0.1mm",
        r="%1.3fin"%r, fill="none", stroke="red")
    sketch.add(shape)

def addRoundedRectangle(sketch, x, y, w, h, corner_radius):
    x -= x / 2.0
    y -= 0.0 - w / 2.0
    sys.stdout.write( "rect %f %f %f %f %s\n"%(x, y, w, h, corner_radius))
    shape = svgwrite.shapes.Rect(insert=("%1.3fin"%x, "%1.3fin"%y),
        size=("%1.3fin"%w, "%1.3fin"%h), stroke="red", fill="none",
        stroke_width = "0.1mm",
        rx="%1.3fin"%corner_radius, ry="%1.3fin"%corner_radius)
    sketch.add(shape)

sketch = svgwrite.Drawing(filename="front_pannel.svg", debug=True)
#~ unit = svgwrite.Unit(unit="inch")
#~ unit="inch"
pannelWidth = 7.8
pannelHeight = 5.2
xCenter = pannelWidth * 0.5
yCenter = pannelHeight * 0.5
ledRadius = 0.12

sketch.add(svgwrite.text.Text("(0,0)", x="0", y="0"))
sketch.add(svgwrite.text.Text("(7,5)", x="7", y="5"))


# let's compute borders
pannelWidth = 7.8
pannelHeight = 5.2
xCenter = pannelWidth * 0.5
yCenter = pannelHeight * 0.5
addRoundedRectangle(sketch, xCenter, yCenter, pannelWidth, pannelHeight, 0.2)

# let's compute step leds
x = xCenter - 2.975 
y = yCenter - 0.025
r = ledRadius
for led_num in range(16):
    addLed(sketch, x, y, r)
    x += 0.35

# let's compute individual leds
x = xCenter - 2.975 
y = yCenter - 1.4
r = ledRadius
# led start
addLed(sketch, x, y, r)
# led stop
x += 0.70 * 3.0
addLed(sketch, x, y, r)
# leds sequences
x += 0.70 * 1.5
for led_num in range(5):
    addLed(sketch, x, y, r)
    x += 0.70






sketch.save()
