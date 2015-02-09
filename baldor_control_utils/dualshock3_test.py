#!/usr/bin/env python

import yarp as y
from time import sleep


#---------------------------Yarp ports connection-----------------------------------

#starting yarp network
y.Network.init()


#creating output port
joystick_output_port_1=y.BufferedPortBottle()
joystick_output_port_1_name="/ds3/stick1_y"
joystick_output_port_1.open(joystick_output_port_1_name)


#connecting joystick data to motors
style=y.ContactStyle()
style.persistent=1
stm32_1_input_port_speed_1_name="/stm32_1/speed/in_1"
stm32_2_input_port_speed_1_name="/stm32_2/speed/in_1"
y.Network.connect(joystick_output_port_1_name,stm32_1_input_port_speed_1_name,style)
y.Network.connect(joystick_output_port_1_name,stm32_2_input_port_speed_1_name,style)



#-------------joystick connection----------------------

import pygame as pg
pg.init()
pg.joystick.init()
js=pg.joystick.Joystick(0)
js.init()
y_out=0.




if True:
    while True:

        #getting position from joystick
        pg.event.get()
        y=js.get_axis(1)
        y_out=-y
        
        #sending position to stm32s
        joystick_output_bottle_1=joystick_output_port_1.prepare()
        joystick_output_bottle_1.clear()
        joystick_output_bottle_1.addDouble(y_out)
        joystick_output_port_1.write()
        sleep(0.01)

