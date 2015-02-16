#!/usr/bin/env python

import yarp as y
from time import sleep

y.Network.init()
port=y.BufferedPortBottle()
portname="/ds3/stick1_y"
port.open(portname)

style=y.ContactStyle()
style.persistent=1
corocoportname1="/coroco/baldor/in1"
corocoportname2="/coroco/baldor/in2"
y.Network.connect(portname,corocoportname1,style)
y.Network.connect(portname,corocoportname2,style)


import pygame as pg
pg.init()
pg.joystick.init()
js=pg.joystick.Joystick(0)
js.init()

y_out=0.

#try:
if True:
    while True:
        pg.event.get()
        y=js.get_axis(1)
        y_out=-y
        #print y_out
        bottle=port.prepare()
        bottle.clear()
        bottle.addDouble(y_out)
        bottle.addString("hola")

        port.write()
        sleep(0.01)
#except:
#    print "Closing DS3"


