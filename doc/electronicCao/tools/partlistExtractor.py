#!/usr/bin/python
# -*- coding: utf-8 -*-

import xml.etree.ElementTree as ET
import sys

if len(sys.argv) == 1:
    raise Exception ("No filename in script's arguments!\n")
xml_root = ET.parse(sys.argv[1]).getroot()

class PARTS:
    def __init__(self):
        self.__dico = {}
        
    def addPart(self, key, value):
        key = key.upper().strip()
        value = value.strip()
        if key in self.__dico.keys():
            self.__dico[key].append(value)
        else:
            self.__dico[key] = [value]
            
    def getParts(self):
        return self.__dico

temp_parts = PARTS()

for element in xml_root:
    if element.tag == "components":
        for component in element:
            name = component.get("ref")
            found = False
            for property in component:
                if property.tag == "value":
                    found = True
                    value = property.text
                    temp_parts.addPart(value, name)
            if not found:
                temp_parts.addPart("???", name)
                
parts = temp_parts.getParts()

keys = [key for key in parts.keys()]
keys.sort()
for key in keys:
    references = parts[key]
    references.sort()
    sys.stdout.write("%s x %s\n"%(key, len(references)))
    for reference in references:
        sys.stdout.write("    %s\n"%(reference))



                    
                