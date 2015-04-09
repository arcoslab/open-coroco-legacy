#!/usr/bin/env python

import yarp as y
from time import sleep


#---------------------------Yarp ports connection-----------------------------------

#starting yarp network
y.Network.init()


#creating output port 1
joystick_output_port_1=y.BufferedPortBottle()
joystick_output_port_1_name="/ds3/stick1_y/out1"
joystick_output_port_1.open(joystick_output_port_1_name)
#creating output port 2
joystick_output_port_2=y.BufferedPortBottle()
joystick_output_port_2_name="/ds3/stick1_y/out2"
joystick_output_port_2.open(joystick_output_port_2_name)
#creating output port 3
joystick_output_port_3=y.BufferedPortBottle()
joystick_output_port_3_name="/ds3/stick1_y/out3"
joystick_output_port_3.open(joystick_output_port_3_name)
#creating output port 4
joystick_output_port_4=y.BufferedPortBottle()
joystick_output_port_4_name="/ds3/stick1_y/out4"
joystick_output_port_4.open(joystick_output_port_4_name)


#connecting joystick data to motors
style=y.ContactStyle()
style.persistent=1
stm32_1_input_port_speed_1_name="/stm32_1/speed/in_1"
stm32_2_input_port_speed_1_name="/stm32_2/speed/in_1"
stm32_3_input_port_speed_1_name="/stm32_3/speed/in_1"
stm32_4_input_port_speed_1_name="/stm32_4/speed/in_1"

y.Network.connect(joystick_output_port_1_name,stm32_1_input_port_speed_1_name,style)
y.Network.connect(joystick_output_port_2_name,stm32_2_input_port_speed_1_name,style)
y.Network.connect(joystick_output_port_3_name,stm32_3_input_port_speed_1_name,style)
y.Network.connect(joystick_output_port_4_name,stm32_4_input_port_speed_1_name,style)



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


        V1=y_out
        V2=V1#V1-1000.0
        V3=V1#V2-1000.0
        V4=V1#-1000.0
        
        #sending position to stm32s

        #stm32 1
        joystick_output_bottle_1=joystick_output_port_1.prepare()
        joystick_output_bottle_1.clear()
        joystick_output_bottle_1.addDouble(V1)
        joystick_output_port_1.write()

        #stm32 2
        joystick_output_bottle_2=joystick_output_port_2.prepare()
        joystick_output_bottle_2.clear()
        joystick_output_bottle_2.addDouble(V2)
        joystick_output_port_2.write()

        #stm32 3
        joystick_output_bottle_3=joystick_output_port_3.prepare()
        joystick_output_bottle_3.clear()
        joystick_output_bottle_3.addDouble(V3)
        joystick_output_port_3.write()

        #stm32 4
        joystick_output_bottle_4=joystick_output_port_4.prepare()
        joystick_output_bottle_4.clear()
        joystick_output_bottle_4.addDouble(V4)
        joystick_output_port_4.write()


        sleep(0.01)

