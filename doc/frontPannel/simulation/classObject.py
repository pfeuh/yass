#!/usr/bin/python
# -*- coding: utf-8 -*-

OBJECT_TYPE_CIRCLE = 1
OBJECT_TYPE_RECTANGLE = 2
OBJECT_TYPE_TEXT = 3

class OBJECT():
    def __init__(self, name):
        self.__name = name
        self.__comment = ""
        
    def getName(self):
        return self.__name
        
    def format(self, values):
        return " ".join(["%.02f"%item for item in values])

    def __str__(self):
        return self.__name
        
    def setComment(self, comment=""):
        self.__comment = comment.replace("\n", " ")
        
    def getComment(self,comment=""):
        return self.__comment

class CIRCLE(OBJECT):
    cpt = 0
    def __init__(self, x, y, diameter):
        CIRCLE.cpt += 1
        OBJECT.__init__(self, "circle_%04u"%CIRCLE.cpt)
        self.__type = OBJECT_TYPE_CIRCLE
        self.__x = float(x)
        self.__y = float(y)
        self.__diameter = float(diameter)

    def __str__(self):
        return "%s %s"%(OBJECT.getName(self), OBJECT.format(self, self.getSpecs()))

    def getSpecs(self):
        return self.__x, self.__y, self.__diameter
        
    def draw(self, canvas, scale=1.0, **kwds):
        x1 = self.__x - self.__diameter / 2.0
        x2 = self.__x + self.__diameter / 2.0
        y1 = self.__y - self.__diameter / 2.0
        y2 = self.__y + self.__diameter / 2.0
        return canvas.create_oval("%fi"%(scale*x1), "%fi"%(scale*y1), "%fi"%(scale*x2), "%fi"%(scale*y2), **kwds)
        
class RECTANGLE(OBJECT):
    cpt = 0
    def __init__(self, x, y, w, h):
        RECTANGLE.cpt += 1
        OBJECT.__init__(self, "rectangle_%04u"%RECTANGLE.cpt)
        self.__type = OBJECT_TYPE_RECTANGLE
        self.__x, self.__y, self.__w, self.__h = float(x), float(y), float(w), float(h)

    def __str__(self):
        return "%s %s"%(OBJECT.getName(self), OBJECT.format(self, self.getSpecs()))

    def getSpecs(self):
        return self.__x, self.__y, self.__w, self.__h 
        
    def draw(self, canvas, scale=1.0, **kwds):
        x1 = self.__x - self.__w / 2.0
        x2 = self.__x + self.__w / 2.0
        y1 = self.__y - self.__h / 2.0
        y2 = self.__y + self.__h / 2.0
        #~ print("canvas.create_rectangle" + " %.02fi"%(scale*x1) + " %.02fi"%(scale*y1) + " %.02fi"%(scale*x2) + " %.02fi"%(scale*y2))
        return canvas.create_rectangle("%fi"%(scale*x1), "%fi"%(scale*y1), "%fi"%(scale*x2), "%fi"%(scale*y2), **kwds)
        
class TEXT(OBJECT):
    cpt = 0
    def __init__(self, x, y, text, size=12):
        TEXT.cpt += 1
        OBJECT.__init__(self, "text_%04u"%TEXT.cpt)
        self.__type = OBJECT_TYPE_TEXT
        self.__x, self.__y, self.__text, self.__size = float(x), float(y), text, float(size)

    def __str__(self):
        return "%s %s \"%s\""%(OBJECT.getName(self), OBJECT.format(self, self.getSpecs()), self.__text.replace("\n", " "))

    def getSpecs(self):
        return self.__x, self.__y, self.__size
        
    def draw(self, canvas, scale=1.0, **kwds):
        kwds["text"] = self.__text
        kwds["justify"] = "center"
        x = self.__x
        y = self.__y
        #~ print("canvas.create_text" + " %.02fi"%(scale*x) + " %.02fi"%(scale*y) + " " + self.__text.replace("\n", " "))
        return canvas.create_text("%fi"%(scale*x), "%fi"%(scale*y), **kwds)
        
if __name__ == "__main__":
    
    objects = []
    
    for x in range(20):
        objects.append(CIRCLE(float(x) * 0.2, 0.5, 0.1))
        objects[-1].setComment("circ_%u"%x)
        objects.append(RECTANGLE(x * 20, 10, 19, 10))
        objects[-1].setComment("rect_%u"%x)
        objects.append(TEXT(x * 20, 10, "text_%04u"%(x+1), 10))
        objects[-1].setComment("text_%u"%x)
        
    for object in objects:
        #~ print(object)
        print(object.getComment())

