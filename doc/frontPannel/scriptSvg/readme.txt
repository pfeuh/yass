scriptSvg is a tiny library to create inkscape files from scratch...
Without having inkscape. The target is to create files for
lasercut machine.

As it is extreme programming, I've only implemented what I need.

How does it work? I sliced a dummy svg file (which is an XML file)
with a circle, a text and a rectangle objects. I use theses sliced
texts as templates, filling coordinates and other full stuff with
script yassFrontpannel_xx.py.

The main problem for me was to create texts in 4 directions with
the 3 anchors start, middle & end. It's now fixed.

As leds and buttons of front pannel I want to lasercut are on a pcb,
I had to design this pcb in inches, that's why this library works
not so bad in inches.
