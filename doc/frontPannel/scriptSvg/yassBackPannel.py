#!/usr/bin/python
# -*- coding: utf-8 -*-

# 
# file : yassBackPannel.py
# Copyright (c) pfeuh <ze.pfeuh@gmail>
# All rights reserved.
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 

import sys

sys.stdout.write(sys.version + "\n")

import scriptSvg

COEFF_IN_MM = 24.5

BACK_PANNEL_WIDTH = 216
BACK_PANNEL_HEIGHT = 64
DIN5_RADIUS = 7.55
SCREW_RADIUS = 1.55
TEXT_OFFSET = 10
DRAW_CROSS = False
LEGEND_FONT_SIZE = 2.0
ON_OFF_RADIUS = 6.3
DIN_OUT_X = 2.45 * COEFF_IN_MM
DIN_IN_X = DIN_OUT_X - 1.20 * COEFF_IN_MM
CROSS_RADIUS = DIN5_RADIUS * 2.0

def addLegend(sketch, x, y, text="", label=""):
    sketch.addText(text, x, y + TEXT_OFFSET, anchor="middle", font_size=LEGEND_FONT_SIZE, label="text%s"%label)

def drawDin5(sketch, x, y, root_name=""):
    sketch.addCircle(x, y, DIN5_RADIUS, label="DIN5%s"%root_name)
    sketch.addCircle(x - 11.25, y, SCREW_RADIUS, label="DIN5%sFixHole1"%root_name)
    sketch.addCircle(x + 11.25, y, SCREW_RADIUS, label="DIN5%sFixHole2"%root_name)
    addLegend(sketch, x, y, root_name, label=root_name)

def drawBackPannel(sketch):
    # let's compute back pannel border
    sketch.addRectangle(0, 0, BACK_PANNEL_WIDTH, BACK_PANNEL_HEIGHT, 0, label="contour back border")

    y = -BACK_PANNEL_HEIGHT / 4.0
    drawDin5(sketch,DIN_OUT_X ,y , root_name=" OUT")
    drawDin5(sketch,DIN_IN_X  ,y , root_name=" IN")

    x = -1.9 * COEFF_IN_MM
    sketch.addCircle(x, y, ON_OFF_RADIUS, label="ON/OFF")
    addLegend(sketch, x, y, label="ON/OFF")

    x = -(1.9 + 0.7) * COEFF_IN_MM
    sketch.addCircle(x, y, ON_OFF_RADIUS, label="POWER")

    sketch.addText("POWER", x +10, y + TEXT_OFFSET, anchor="middle", font_size=LEGEND_FONT_SIZE, label="text POWER")


if __name__ == "__main__":

    FNAME = "backPannel"
    IMG_PATH = "img"
    filename = "%s/%s.svg"%(IMG_PATH, FNAME)
    sketch = scriptSvg.SKETCH(filename, "mm")
    
    #~ # I don't know why drawing it out of the sheet, but I can fix it
    sketch.changeOrigin(6 * 24.5, -7.5 * 24.5)

    drawBackPannel(sketch)
  
    sketch.save()
    sketch.makeCSV("img/backPannelCoordinates.csv")

    