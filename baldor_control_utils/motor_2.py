#!/usr/bin/env python

#importing libraries
import yarp as y
import serial  as s
from serial import SerialException
from numpy  import pi
from time   import sleep
from motor_tools  import *
#from motor_position import *


#--------------Yarp ports definition---------------------

#starting yarp network
y.Network.init()


#creting an output port for stm32_1_position
'''
stm32_2_output_port_1       =y.BufferedPortBottle()
stm32_2_output_port_1_name  ="/stm32_2/position/out1"
stm32_2_output_port_1.open(stm32_2_output_port_1_name)
'''
out_1       =y.BufferedPortBottle()
out_1_name  ="/stm32_2/position/out1"
out_1.open(out_1_name)


#creating an input port from stm32_2
'''
stm32_2_input_port_1        =y.BufferedPortBottle()
stm32_2_input_port_1_name   ="/stm32_2/position/in1"
stm32_2_input_port_1.open(stm32_2_input_port_1_name)
'''
in_1        =y.BufferedPortBottle()
in_1_name   ="/stm32_2/position/in1"
in_1.open(in_1_name)


#creating an input port from PS3 controller
'''
stm32_2_input_port_speed_1        =y.BufferedPortBottle()
stm32_2_input_port_speed_1_name   ="/stm32_2/speed/in_1"
stm32_2_input_port_speed_1.open(stm32_2_input_port_speed_1_name)
'''
in_speed        =y.BufferedPortBottle()
in_speed_name   ="/stm32_2/speed/in_1"
in_speed.open(in_speed_name)



#connecting with remote port STM32_1
style=y.ContactStyle()
style.persistent=1
stm32_1_input_port_1_name="/stm32_1/position/in1"
stm32_3_input_port_1_name="/stm32_3/position/in2"
stm32_4_input_port_1_name="/stm32_4/position/in2"
'''
y.Network.connect(stm32_2_output_port_1_name,stm32_1_input_port_1_name,style)
y.Network.connect(stm32_2_output_port_1_name,stm32_3_input_port_1_name,style)
y.Network.connect(stm32_2_output_port_1_name,stm32_4_input_port_1_name,style)
'''
y.Network.connect(out_1_name,stm32_1_input_port_1_name,style)
y.Network.connect(out_1_name,stm32_3_input_port_1_name,style)
y.Network.connect(out_1_name,stm32_4_input_port_1_name,style)



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
stm32_1=motor_position()
stm32_2=motor_position()
serial_device_counter=0
connected=False

serial_1=0



motor=motor_tools()


while True:

    #-------------------Confirming stm32_1_position from yarp bottles-----------

    #stm32_1=motor.checking_motor_position('1',stm32_1,stm32_2_input_port_1,stm32_2_input_port_1_name)
    stm32_1=motor.checking_motor_position('1',stm32_1,in_1,in_1_name)

    #print "motor 1=> serial: ",stm32_1.serial,"position: ",stm32_1.position
    #raw_input("raw input")
    #------------Connecting to the serial port---------------------

    #initial connection
    motor.connecting_to_stm32(2,stm32_1.serial,0,0,0)
    

    '''
    #initial connection
    while not connected:
        try:
            sleep(0.01)
            #serial_device_counter=3;
            if serial_device_counter==serial_1:#stm32_1_position:
                serial_device_counter=serial_1+1#stm32_1_position+1

            print "Connecting to /dev/ttyACM"+str(serial_device_counter)
            com=s.Serial("/dev/ttyACM"+str(serial_device_counter), baudrate=921600,timeout=1)
            
        except:
            sleep(0.01)
            if (serial_device_counter>100):
              serial_device_counter=0
            print "Connect opencoroco usb cable, trying: /dev/ttyACM"+str(serial_device_counter)  
            #print "stm32_position: " + stm32_position 
            serial_device_counter=serial_device_counter+1      
        else:
            connected=True
            print "Connected to /dev/ttyACM"+str(serial_device_counter) 
            #print "stm32_position: " + stm32_position
    '''            

    #-----------------------------
    try:
   
        while motor.connected==True:

            '''
            #waiting for data from stm32 serial port
            while com.inWaiting()>0:
                com.read(com.inWaiting())
                print "waiting"  
            
            #writing data from stm32
            com.write("f "+str(cmd_speed)+"\n\r")
            print "writing to stm32: "+ str(cmd_speed)+"original_value: "+str(value)+"test: "
            print "Connected to /dev/ttyACM"+str(serial_device_counter)   

            #reading data from stm32
            line=com.readline() 
            print "line_from_stm: ",line 
            line_split=line.split()
            stm32_2_position=line_split[0]
            print "stm32_position: " + stm32_2_position
            '''
            motor.serial_port_communication(motor,cmd_speed,value)


            #-----------Connected to STM32-------------------------------
            #if stm32_2_position=='2':
            if motor.position=='2':            
            
                #-------------------Communication though yarp bottles-----------
                sleep(0.01)

                #sending a bottle through the output_port_1
                '''
                stm32_2_output_bottle_1=stm32_2_output_port_1.prepare()
                stm32_2_output_bottle_1.clear()
                stm32_2_output_bottle_1.addString(stm32_2_position)#"hola2")
                stm32_2_output_bottle_1.addInt(serial_device_counter)#"hola2")
                stm32_2_output_port_1.write()
                '''
                motor.send_a_bottle(out_1,motor.position,motor.serial_device_counter)

                #receaving a bottle from the input port 1
                '''
                stm32_2_input_bottle_1=stm32_2_input_port_1.read(False)
                if stm32_2_input_bottle_1:

                    stm32_2_input_data_1=stm32_2_input_bottle_1.get(0)
                    stm32_1_position=stm32_2_input_data_1.asString()

                    stm32_2_input_data_1=stm32_2_input_bottle_1.get(1)
                    serial_1=stm32_2_input_data_1.asInt()

                    print "there is a bottle from ",stm32_2_input_port_1_name
                    print "bottle content: Position_1=",stm32_1_position,"serial_1: ttyACM",serial_1 
                    
                else:
                    print "there is no bottle from ",stm32_2_input_port_1_name    
                ''' 
                motor.receive_a_motor_bottle(in_1,in_1_name)
                

               #receiving a bottle from PS3 controller
                ''' 
                stm32_2_input_bottle_speed_1=stm32_2_input_port_speed_1.read(False)
                if stm32_2_input_bottle_speed_1:
                    stm32_2_input_data_speed_1=stm32_2_input_bottle_speed_1.get(0)
                    speed_1_value=stm32_2_input_data_speed_1.asDouble()
                    cmd_speed=speed_1_value
                    print "there is a bottle from ",stm32_2_input_port_speed_1_name
                    print "bottle content: ",speed_1_value
                    
                else:
                    print "there is no bottle from ",stm32_2_input_port_speed_1_name       
                '''
                cmd_speed=motor.receive_a_joystick_bottle(in_speed,in_speed_name)

            #------not connected to STM32 2----------------
            else:
                '''
                connected=False
                print "wrong stm32, detected: ",stm32_2_position
                stm32_2_position='0'
                serial_device_counter=serial_device_counter+1
                '''
                motor.retry_serial_connection()

    except SerialException:
        motor.serial_exception_handling()
        '''
        #com.close()
        #del com
        print com.inWaiting()
        print "error! error!"
        #print line
        connected=False            
        '''
