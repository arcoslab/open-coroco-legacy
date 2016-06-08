#!/usr/bin/env python

import yarp as y
from time import sleep
import math #added by Wilber

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
style.persistent=True
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

        '''
        y=js.get_axis(1)
        y_out=-y


        V1=y_out
        V2=V1#V1-1000.0
        V3=V1#V2-1000.0
        V4=V1#-1000.0
        '''


        #------------------------------------------------------------------------------
        #------------------------------------------------------------------------------

        #getting position from left joystick
        x1=js.get_axis(1)       #Left joystick left to right values -1.0 to 0.99
        y1=js.get_axis(0)       #Left joystick up to down values -1.0 to 0.99

        y1_out=y1              #Controller gives negative values for the upbutton
        x1_out=-x1


	    #getting position from right joystick
        x2=js.get_axis(2)   #Left joystick left to right values -1.0 to 0.99
        y2=js.get_axis(3)   #Left joystick up to down values -1.0 to 0.99
        y2_out=-y2          #Controller gives negative values for the upbutton
        x2_out=x2

	    #getting speed from velocity vector
        speed= math.sqrt(x1_out**2+y1_out**2)

	    #getting angle form velocity vector
        direction=math.atan2(y1_out,x1_out) #TODO make sure in which quadrant the angle is 

	    #radius: diagonal distance between wheels split in two
        radius = 0.35/2 #carrito
        angularSpeed = math.sqrt(x2_out**2+y2_out**2) 
	
	    #tangencial speed for the wheels
        vTheta = angularSpeed*radius
	
	    #speed for the four wheels
        v1=speed*math.sin(direction+(math.pi/4))+vTheta #speed upper-left wheel
        v2=speed*math.cos(direction+(math.pi/4))-vTheta #speed upper-right wheel
        v3=speed*math.cos(direction+(math.pi/4))+vTheta #speed down-left wheel
        v4=speed*math.sin(direction+(math.pi/4))-vTheta #speed down-right wheel


        '''
        right_1 =-y_out
        left_1  = y_out#V1-1000.0
        right_2 = y_out#V2-1000.0
        left_2  = y_out#-1000.0
        '''
        right_1 =-v2
        left_1  = v1
        right_2 = -v4
        left_2  = v3

        #sending position to stm32s

        #stm32 1
        joystick_output_bottle_1=joystick_output_port_1.prepare()
        joystick_output_bottle_1.clear()
        joystick_output_bottle_1.addDouble(right_1)
        joystick_output_port_1.write()

        #stm32 2
        joystick_output_bottle_2=joystick_output_port_2.prepare()
        joystick_output_bottle_2.clear()
        joystick_output_bottle_2.addDouble(left_1)
        joystick_output_port_2.write()

        #stm32 3
        joystick_output_bottle_3=joystick_output_port_3.prepare()
        joystick_output_bottle_3.clear()
        joystick_output_bottle_3.addDouble(right_2)
        joystick_output_port_3.write()

        #stm32 4
        joystick_output_bottle_4=joystick_output_port_4.prepare()
        joystick_output_bottle_4.clear()
        joystick_output_bottle_4.addDouble(left_2)
        joystick_output_port_4.write()


        sleep(0.01)

