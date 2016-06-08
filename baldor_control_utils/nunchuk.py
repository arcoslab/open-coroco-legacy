#!/usr/bin/env python

import cwiid as w
import yarp as y
from time import sleep

y.Network.init()
port=y.BufferedPortBottle()
portname="/wiimote/nunchuk/stick_y"
corocoportname="/coroco/baldor/in"
port.open(portname)
style=y.ContactStyle()
style.persistent=True
y.Network.connect(portname,corocoportname,style)
print "Press 1 and 2 buttons at once now"
wm=w.Wiimote()
print "Wiimote connected"
wm.rpt_mode=w.RPT_NUNCHUK

while not 'nunchuk' in wm.state.keys():
    print "Waiting for nunchuk"
print "nunchuk present"

print "Don't touch joystick. Calibrating zero"
y_center=0.
y=0
while y==0:
    x,y=wm.state['nunchuk']['stick']

for i in xrange(100):
    x,y=wm.state['nunchuk']['stick']
    y_center+=y

y_center/=100.0
print "Center calibrated: ", y_center

print "When ready press any button and Move up and down. Then press any button when finished"
while wm.state['nunchuk']['buttons']==0:
    pass
sleep(0.1)
while wm.state['nunchuk']['buttons']!=0:
    pass
sleep(0.1)

y_max=0.
y_min=200.
while wm.state['nunchuk']['buttons']==0:
    x,y=wm.state['nunchuk']['stick']
    if y>y_max:
        y_max=y
        print "Y Max: ", y
    if y<y_min:
        y_min=y
        print "Y Min: ", y
    sleep(0.01)

print "Max and min values", y_max, y_min

print "When ready press any button to continue"
while wm.state['nunchuk']['buttons']==0:
    pass

y_min-=y_center
y_max-=y_center

y_out=0.



try:
    while True:
        x,y=wm.state['nunchuk']['stick']
        y_out=y-y_center
        if y_out>0.:
            y_out/=y_max
        else:
            y_out/=-y_min
        print y_out
        bottle=port.prepare()
        bottle.clear()
        bottle.addDouble(y_out)
        port.write()
        sleep(0.01)
except:
    print "Closing wiimote"
    wm.close()

