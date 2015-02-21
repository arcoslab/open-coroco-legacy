#!/usr/bin/env python

#importing libraries
import yarp     as y
import serial   as s
from serial import SerialException
from numpy  import pi
from time   import sleep
from motor_tools  import *

#-----------------yarp connection----------------------------------

#starting yarp network
y.Network.init()


#creting output port
out_1       =y.BufferedPortBottle()
out_1_name  ="/stm32_1/position/out1"
out_1.open(out_1_name)

#creating an input port
in_1        =y.BufferedPortBottle()
in_1_name   ="/stm32_1/position/in1"
in_1.open(in_1_name)

#creating an input port from PS3 controller
in_speed        =y.BufferedPortBottle()
in_speed_name   ="/stm32_1/speed/in_1"
in_speed.open(in_speed_name)

#connecting with remote ports
style=y.ContactStyle()
style.persistent=1
stm32_2_in_1_name="/stm32_2/position/in1"
stm32_3_in_1_name="/stm32_3/position/in1"
stm32_4_in_1_name="/stm32_4/position/in1"
y.Network.connect(out_1_name,stm32_2_in_1_name,style)
y.Network.connect(out_1_name,stm32_3_in_1_name,style)
y.Network.connect(out_1_name,stm32_4_in_1_name,style)


#------------------Initial values -----------------

#for speeds and postition calculations
max_speed=12*2*pi
last_raw_angle=0.
raw_angle=0.
angle=0.
rads=0.

value=33
cmd_speed=0.0

#for serial port
stm32_position='0'
serial_device_counter=0
connected=False



motor=motor_tools()


while True:
    
    #------------Connecting to the serial port---------------------
    motor.connecting_to_stm32()

    #-----------------------------
    try:
   
        while motor.connected==True:

            #waiting for data from stm32 serial port
            while motor.com.inWaiting()>0:
                motor.com.read(motor.com.inWaiting())
                print "waiting"  
            
            #writing data from stm32
            motor.com.write("f "+str(cmd_speed)+"\n\r")
            print "writing to stm32: "+ str(cmd_speed)+"original_value: "+str(value)+"test: "
            print "Connected to /dev/ttyACM"+str(motor.serial_device_counter)   

            #reading data from stm32
            line=motor.com.readline() 
            print "line_from_stm: ",line 
            line_split=line.split()
            stm32_1_position=line_split[0]
            print "stm32_position: " + stm32_1_position
            

            
            #-----------connected to STM32 1----------
            if stm32_1_position=='1':

                #-------------------Communication though yarp bottles-----------
                sleep(0.01)


                motor.send_a_bottle(out_1,stm32_1_position,motor.serial_device_counter)

                stm32_2_position=motor.receive_a_motor_bottle(in_1,stm32_2_in_1_name)

                cmd_speed=motor.receive_a_joystick_bottle(in_speed,in_speed_name)



            #---------not connected to STM32 1----------------
            else:
                motor.connected=False
                print "wrong stm32, detected: ",stm32_1_position
                stm32_1_position='0'
                motor.serial_device_counter=motor.serial_device_counter+1


    except SerialException:
        #com.close()
        #del com
        print motor.com.inWaiting()
        print "error! error!"
        #print line
        motor.connected=False  
