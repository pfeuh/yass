#!/usr/bin/python
# -*- coding: utf-8 -*-

import lasercut
import sys

sketch = lasercut.SKETCH('test.svg', "in")

LED_X_OFFSET = -2.975
LED_RADIUS = 0.06
LED_WIDTH = 0.35
LED_HEIGHT = 1.1
BUTTON_X_OFFSET = -2.95

# let's compute front_pannel border
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

sketch.save()

