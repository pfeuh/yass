#!/usr/bin/python
# -*- coding: utf-8 -*-

# 
# file : scriptSvg.py
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

TEMPLATE = """<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!-- Created with Inkscape (http://www.inkscape.org/) -->

<svg
   xmlns:dc="http://purl.org/dc/elements/1.1/"
   xmlns:cc="http://creativecommons.org/ns#"
   xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
   xmlns:svg="http://www.w3.org/2000/svg"
   xmlns="http://www.w3.org/2000/svg"
   xmlns:sodipodi="http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd"
   xmlns:inkscape="http://www.inkscape.org/namespaces/inkscape"
   width="297mm"
   height="210mm"
   viewBox="0 0 297 210"
   version="1.1"
   id="svg643"
   inkscape:version="0.92.4 (5da689c313, 2019-01-14)"
   sodipodi:docname="#DOC_NAME#">
  <defs
     id="defs637" />
  <sodipodi:namedview
     id="base"
     pagecolor="#ffffff"
     bordercolor="#666666"
     borderopacity="1.0"
     inkscape:pageopacity="0.0"
     inkscape:pageshadow="2"
     inkscape:zoom="0.35"
     inkscape:cx="14.285714"
     inkscape:cy="560"
     inkscape:document-units="mm"
     inkscape:current-layer="layer1"
     showgrid="false"
     inkscape:window-width="1600"
     inkscape:window-height="837"
     inkscape:window-x="-8"
     inkscape:window-y="-8"
     inkscape:window-maximized="1" />
  <metadata
     id="metadata640">
    <rdf:RDF>
      <cc:Work
         rdf:about="">
        <dc:format>image/svg+xml</dc:format>
        <dc:type
           rdf:resource="http://purl.org/dc/dcmitype/StillImage" />
        <dc:title></dc:title>
      </cc:Work>
    </rdf:RDF>
  </metadata>
  <g
     inkscape:label="Calque 1"
     inkscape:groupmode="layer"
     id="layer1"
     transform="translate(0,-87)">
#OBJECTS#  </g>
</svg>
"""

RECTANGLE = """<rect
       style="fill:none;stroke:#ff0000;stroke-width:0.2;stroke-opacity:1"
       id="#ID#"
       width="#W#"
       height="#H#"
       x="#X#"
       y="#Y#"
       rx="#RX#"
       ry="#RY#" />
"""

TEXT = """<text
       xml:space="preserve"
       style="font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;font-size:#FONT_SIZE#mm;line-height:125%;font-family:sans-serif;-inkscape-font-specification:'sans-serif, Normal';font-variant-ligatures:normal;font-variant-caps:normal;font-variant-numeric:normal;text-align:center;letter-spacing:0px;word-spacing:0px;writing-mode:lr-tb;text-anchor:#TEXT_ANCHOR#;fill:#000000;fill-opacity:1;stroke:none;stroke-width:0.26458332px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1"
       x="#X#"
       y="#Y#"
       #TRANSFORM#
       id="#ID#"><tspan
         sodipodi:role="line"
         id="tspan771"
         x="#X#"
         y="#Y#"
         style="stroke-width:0.26458332px">#TEXT#</tspan></text>
"""

ELLIPSE = """ <ellipse
       style="fill:none;stroke:#ff0000;stroke-width:0.26115048;stroke-opacity:1"
       id="#ID#"
       cx="#X#"
       cy="#Y#"
       rx="#RX#"
       ry="#RY#" />
"""

# text--align : start middle end

VERBOSE = "-verbose" in sys.argv
SHOW_CIRCLES = "-circle" in sys.argv
SHOW_RECTANGLES = "-rectangle" in sys.argv
SHOW_TEXTS = "-text" in sys.argv

if VERBOSE:
    sys.stdout.write("VERBOSE mode active\n")

class SKETCH():
    RECTANGLE_ID = 1
    ELLIPSE_ID = 1
    TEXT_ID = 1
    
    def __init__(self, fname, unit="mm"):
        self.changeUnit(unit)
        self.__x_origin = 0.0
        self.__y_origin = 0.0
        self.__fname = fname
        self.__objects = {}
        self.__raw_xy = {}
        
    def getRawXy(self, objname):
        if objname in self.__raw_xy.keys():
            return self.__raw_xy[key]

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
        
    def addRectangle(self, x, y, w, h, radius=0):
        label = "rect%u"%SKETCH.RECTANGLE_ID
        self.__raw_xy[label] = float(x), float(y)
        SKETCH.RECTANGLE_ID += 1
        x, y, w, h, radius = self.cvrt(x), self.cvrt(y), self.cvrt(w), self.cvrt(h), self.cvrt(radius)
        x = x - w / 2.0 
        y = 0.0 - y - h / 2.0 
        x += self.__x_origin
        y += self.__y_origin
        self.__objects[label] = [x, y, w, h, radius]
        if SHOW_RECTANGLES:
            sys.stdout.write("%s %s\n"%(label, str(self.__objects[label])))
        return label

    def addCircle(self, x, y, r):
        label = "elli%u"%SKETCH.ELLIPSE_ID
        SKETCH.ELLIPSE_ID += 1
        x, y, r = self.cvrt(x), 0.0 - self.cvrt(y), self.cvrt(r)
        x += self.__x_origin
        y += self.__y_origin
        self.__objects[label] = [x, y, r]
        if SHOW_CIRCLES:
            sys.stdout.write("%s %s\n"%(label, str(self.__objects[label])))
        return label

    def addText(self, text, x, y, font_size=1.5, anchor="start", transform=None):
        label = "text%u"%SKETCH.TEXT_ID
        SKETCH.TEXT_ID += 1
        x, y = self.cvrt(x), self.cvrt(y)
        y = 0.0 - y
        x += self.__x_origin
        y += self.__y_origin
        self.__objects[label] = [x, y, str(text), font_size, anchor, transform]
        if SHOW_TEXTS:
            sys.stdout.write("%s %s\n"%(label, str(self.__objects[label])))
        return label

    def save(self):
        objects_code = ""
        for key in self.__objects.keys():
            object = self.__objects[key]
            if key.startswith("rect"):
                code = RECTANGLE[:]
                code = code.replace("#ID#", key)
                code = code.replace("#X#", str(object[0]))
                code = code.replace("#Y#", str(object[1]))
                code = code.replace("#W#", str(object[2]))
                code = code.replace("#H#", str(object[3]))
                code = code.replace("#RX#", str(object[4]))
                code = code.replace("#RY#", str(object[4]))
                objects_code += code
            elif key.startswith("elli"):
                code = ELLIPSE[:]
                code = code.replace("#ID#", key)
                code = code.replace("#X#", str(object[0]))
                code = code.replace("#Y#", str(object[1]))
                code = code.replace("#RX#", str(object[2]))
                code = code.replace("#RY#", str(object[2]))
                objects_code += code
            elif key.startswith("text"):
                code = TEXT[:]
                if object[5] == None:
                    code = code.replace("#TRANSFORM#", "")
                else:
                    code = code.replace("#TRANSFORM#", 'transform="rotate(%s)"'%object[5])
                    if object[5] == "-90":
                        temp = object[0]
                        object[0] = -object[1]
                        object[1] = temp
                    if object[5] == "180":
                        object[0] = -object[0]
                        object[1] = -object[1]
                        # TODO: implement 90 & 180
                code = code.replace("#ID#", key)
                code = code.replace("#X#", str(object[0]))
                code = code.replace("#Y#", str(object[1]))
                code = code.replace("#TEXT#", str(object[2]))
                code = code.replace("#FONT_SIZE#", str(object[3]))
                code = code.replace("#TEXT_ANCHOR#", str(object[4]))
                objects_code += code
        if VERBOSE:
            sys.stdout.write(objects_code)
        full_code = TEMPLATE[:]
        full_code = full_code.replace("#DOC_NAME#", self.__fname)
        full_code = full_code.replace("#OBJECTS#", objects_code)
        with open(self.__fname, "w") as fp:
            fp.write(full_code)
        sys.stdout.write("file %s saved.\n"%self.__fname)

if __name__ == "__main__":


    sketch = SKETCH('img/test.svg', "in")
    
    LED_X_OFFSET = -2.975
    LED_RADIUS = 0.06
    LED_WIDTH = 0.35
    LED_HEIGHT = 1.1
    BUTTON_X_OFFSET = -2.95
    
    # let's compute front pannel border
    sketch.addRectangle(0, 0, 7.8, 5.2, 0.15)

    # let's compute step leds
    x = LED_X_OFFSET 
    y = -0.025
    r = LED_RADIUS
    for led_num in range(16):
        sketch.addCircle(x, y, r)
        x += LED_WIDTH

    for idx in range(5):
        x = 0.0
        y = float(idx) * 0.55
        sketch.addText("text %u"%(idx), x, y, 1, "start")

    sketch.save()

