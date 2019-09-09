scriptSgv is a library to create inkscape files from scratch,
without having inkscape. The target is to create files for
lasercut machine.

As it is extreme programming, I've only implemented what I need.

How does it work? I sliced a dummy svg file with a circle, a text
and a rectangle. I use theses sliced texts as templates, filling
coordinates and other full stuff from file yassTrontpannel_xx.py.

The main problem for me was to create texts in 4 directions with
the 3 anchors start, middle & end. It's now fixed.


