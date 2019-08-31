#!/usr/bin/python
# -*- coding: utf-8 -*-

# TODO: in lasercut.py class SKETCH, centered text is not really centered

import lasercut
import sys

sketch = lasercut.SKETCH('test.svg', "in")

LED_X_OFFSET = -2.975
LED_RADIUS = 0.06
LED_WIDTH = 0.35
LED_HEIGHT = 1.1
BUTTON_X_OFFSET = -2.95
BUTTON_TEXT_Y_OFFSET = -0.50
FONT_SIZE = 0.16
PANNEL_WIDTH = 7.8
PANNEL_HEIGHT = 5.2

# let's compute front pannel border
sketch.addRectangle(0, 0, PANNEL_WIDTH, PANNEL_HEIGHT, 0.15, True)

# some advertisement
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
    
# let's compute buttons ans texts
w = 0.5
h = 0.35
r = 0.1
x = BUTTON_X_OFFSET 
y = -1.765
# buttons start, stop, continue, record
bt_transport_text = ["PLAY", "STOP", "CONT", "REC."]
for button_num in range(4):
    sketch.addRectangle(x, y, w, h, r, True)
    sketch.addText(bt_transport_text[button_num], x, y+BUTTON_TEXT_Y_OFFSET, font_size=FONT_SIZE)
    x += LED_WIDTH * 2.0
# buttons sequences 1...5
x += LED_WIDTH
bt_transport_text = ["1", "2", "3", "4", "5"]
for button_num in range(5):
    sketch.addRectangle(x, y, w, h, r, True)
    sketch.addText(bt_transport_text[button_num], x, y+BUTTON_TEXT_Y_OFFSET, font_size=FONT_SIZE)
    x += LED_WIDTH * 2
# buttons sequence, global, return
x -= 0.7
bt_transport_text = ["SEQ.", "GLOB", "OK"]
for button_num in range(3):
    y += LED_HEIGHT
    sketch.addRectangle(x, y, w, h, r, True)
    sketch.addText(bt_transport_text[button_num], x, y+BUTTON_TEXT_Y_OFFSET, font_size=FONT_SIZE)
# buttons rest, tie, last step
x = BUTTON_X_OFFSET
bt_transport_text = ["REST", "TIE", "LAST"]
for button_num in range(3):
    sketch.addRectangle(x, y, w, h, r, True)
    sketch.addText(bt_transport_text[button_num], x, y+BUTTON_TEXT_Y_OFFSET, font_size=FONT_SIZE)
    x += LED_WIDTH * 2

# let's compute 4x7 segments
sketch.addRectangle(0.2, 1.52, 1.582, 0.5, 0, True)

# let's compute encoder button
sketch.addCircle(1.850, 1.5, 0.350, True)

sketch.save()

