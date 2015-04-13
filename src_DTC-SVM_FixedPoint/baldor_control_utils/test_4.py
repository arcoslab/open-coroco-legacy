#!/usr/bin/env python

#importing libraries
import yarp as y
import serial  as s
from serial import SerialException
from numpy  import pi
from time   import sleep




#--------------Yarp ports definition---------------------

#starting yarp network
y.Network.init()


#creting an output port for stm32_1_position
stm32_4_output_port_1       =y.BufferedPortBottle()
stm32_4_output_port_1_name  ="/stm32_4/position/out1"
stm32_4_output_port_1.open(stm32_4_output_port_1_name)

#creating an input port from stm32_1
stm32_4_input_port_1        =y.BufferedPortBottle()
stm32_4_input_port_1_name   ="/stm32_4/position/in1"
stm32_4_input_port_1.open(stm32_4_input_port_1_name)

#creating an input port from stm32_2
stm32_4_input_port_2        =y.BufferedPortBottle()
stm32_4_input_port_2_name   ="/stm32_4/position/in2"
stm32_4_input_port_2.open(stm32_4_input_port_2_name)

#creating an input port from stm32_3
stm32_4_input_port_3        =y.BufferedPortBottle()
stm32_4_input_port_3_name   ="/stm32_4/position/in3"
stm32_4_input_port_3.open(stm32_4_input_port_3_name)



#creating an input port from PS3 controller
stm32_4_input_port_speed_1        =y.BufferedPortBottle()
stm32_4_input_port_speed_1_name   ="/stm32_4/speed/in_1"
stm32_4_input_port_speed_1.open(stm32_4_input_port_speed_1_name)




#connecting with remote port STM32_1
#style=y.ContactStyle()
#style.persistent=1
#stm32_4_input_port_1_name="/stm32_4/position/in1"
#y.Network.connect(stm32_3_output_port_1_name,stm32_4_input_port_1_name,style)




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
stm32_1_position='0'
stm32_2_position='0'
stm32_3_position='0'
stm32_4_position='0'
serial_device_counter=0
connected=False

serial_1=0
serial_2=0
serial_3=0



while True:

    #-------------------Confirming stm32_1_position from yarp bottles-----------


    #while stm32_1_position!='1' and stm32_2_position!='2':
    while stm32_1_position!='1':
        #receaving a bottle from the input port 1
        stm32_4_input_bottle_1=stm32_4_input_port_1.read(False)
        if stm32_4_input_bottle_1:

            stm32_4_input_data_1=stm32_4_input_bottle_1.get(0)
            stm32_1_position=stm32_4_input_data_1.asString()

            stm32_4_input_data_1=stm32_4_input_bottle_1.get(1)
            serial_1=stm32_4_input_data_1.asInt()

            print "there is a bottle from ",stm32_4_input_port_1_name
            print "bottle content=> Position_1: ",stm32_1_position,"Device: ttyACM",serial_1
            #raw_input("Enter enter: ")
            
        else:
            print "there is no bottle from ",stm32_4_input_port_1_name   
            #raw_input("Enter enter: ")
            

    while stm32_2_position!='2':
        #receaving a bottle from the input port 2
        stm32_4_input_bottle_2=stm32_4_input_port_2.read(False)
        if stm32_4_input_bottle_2:

            stm32_4_input_data_2=stm32_4_input_bottle_2.get(0)
            stm32_2_position=stm32_4_input_data_2.asString()

            stm32_4_input_data_2=stm32_4_input_bottle_2.get(1)
            serial_2=stm32_4_input_data_2.asString()

            print "there is a bottle from ",stm32_4_input_port_2_name
            print "bottle content=> Position_2: ",stm32_2_position, " device_2: ", serial_2
            #raw_input("Enter enter: ")
            
            
        else:
            print "there is no bottle from ",stm32_4_input_port_2_name   
            #raw_input("Enter enter: ")
            
     
    while stm32_3_position!='3':
        #receaving a bottle from the input port 2
        stm32_4_input_bottle_3=stm32_4_input_port_3.read(False)
        if stm32_4_input_bottle_3:

            stm32_4_input_data_3=stm32_4_input_bottle_3.get(0)
            stm32_3_position=stm32_4_input_data_3.asString()

            stm32_4_input_data_3=stm32_4_input_bottle_3.get(1)
            serial_3=stm32_4_input_data_3.asString()

            print "there is a bottle from ",stm32_4_input_port_3_name
            print "bottle content=> Position_3: ",stm32_4_position, " device_3: ", serial_3
            #raw_input("Enter enter: ")
            
            
        else:
            print "there is no bottle from ",stm32_4_input_port_3_name   
            raw_input("Enter enter: ")
    #------------Connecting to the serial port---------------------

   
    #initial connection
    while not connected:
        try:
            sleep(0.01)
            #serial_device_counter=3;
            if serial_device_counter==serial_1:
                serial_device_counter=serial_1+1
            if serial_device_counter==serial_2:
                serial_device_counter=serial_2+1
            if serial_device_counter==serial_3:
                serial_device_counter=serial_2+1


            print "Connecting to /dev/ttyACM"+str(serial_device_counter)
            com=s.Serial("/dev/ttyACM"+str(serial_device_counter), baudrate=921600,timeout=1)
            
        except:
            #sleep(0.01)
            if (serial_device_counter>100):
              serial_device_counter=0
            print "Connect opencoroco usb cable, trying: /dev/ttyACM"+str(serial_device_counter)  
            #print "stm32_position: " + stm32_position 
            serial_device_counter=serial_device_counter+1 
            #raw_input("Enter enter: ")
                 
        else:
            connected=True
            print "Connected to /dev/ttyACM"+str(serial_device_counter) 
            #print "stm32_position: " + stm32_position
            #raw_input("Enter enter: ")
            
        


   #-----------------------------
    try:
   
        while connected==True:

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
            stm32_4_position=line_split[0]
            print "stm32_position: " + stm32_3_position
            #raw_input("Enter enter: ")
            


            #-----------Connected to STM32-------------------------------
            if stm32_4_position=='4':            
            
                #-------------------Communication though yarp bottles-----------
                sleep(0.01)

                #sending a bottle through the output_port_1
                stm32_4_output_bottle_1=stm32_4_output_port_1.prepare()
                stm32_4_output_bottle_1.clear()
                stm32_4_output_bottle_1.addString(stm32_4_position)#"hola2")
                stm32_4_output_bottle_1.addInt(serial_device_counter)#"hola2")
                stm32_4_output_port_1.write()
                sleep(0.01)

                #receaving a bottle from the input port 1
                stm32_4_input_bottle_1=stm32_4_input_port_1.read(False)
                if stm32_4_input_bottle_1:
                    stm32_4_input_data_1=stm32_4_input_bottle_1.get(0)
                    stm32_1_position=stm32_4_input_data_1.asString()

                    stm32_4_input_data_1=stm32_4_input_bottle_1.get(1)
                    serial_1=stm32_4_input_data_1.asInt()

                    print "there is a bottle from ",stm32_4_input_port_1_name
                    print "bottle content: Position_1=",stm32_1_position,"serial_1: ttyACM",serial_1 

                    
                else:
                    print "there is no bottle from ",stm32_4_input_port_1_name    
                 


                #receaving a bottle from the input port 2
                stm32_4_input_bottle_2=stm32_4_input_port_2.read(False)
                if stm32_4_input_bottle_2:
                    stm32_4_input_data_2=stm32_4_input_bottle_2.get(0)
                    stm32_2_position=stm32_4_input_data_2.asString()

                    stm32_4_input_data_2=stm32_4_input_bottle_2.get(1)
                    serial_2=stm32_4_input_data_2.asInt()

                    print "there is a bottle from ",stm32_4_input_port_2_name
                    print "bottle content: Position_2=",stm32_2_position,"serial_2: ttyACM",serial_2 

                else:
                    print "there is no bottle from ",stm32_4_input_port_2_name    
                 

                #receaving a bottle from the input port 3
                stm32_4_input_bottle_3=stm32_4_input_port_3.read(False)
                if stm32_4_input_bottle_3:
                    stm32_4_input_data_3=stm32_4_input_bottle_3.get(0)
                    stm32_3_position=stm32_4_input_data_3.asString()

                    stm32_4_input_data_3=stm32_4_input_bottle_3.get(1)
                    serial_3=stm32_4_input_data_3.asInt()

                    print "there is a bottle from ",stm32_4_input_port_3_name
                    print "bottle content: Position_3=",stm32_3_position,"serial_3: ttyACM",serial_3 

                    
                else:
                    print "there is no bottle from ",stm32_4_input_port_3_name    
                 


               #receiving a bottle from PS3 controller
                stm32_4_input_bottle_speed_1=stm32_4_input_port_speed_1.read(False)
                if stm32_4_input_bottle_speed_1:
                    stm32_4_input_data_speed_1=stm32_4_input_bottle_speed_1.get(0)
                    speed_1_value=stm32_4_input_data_speed_1.asDouble()
                    cmd_speed=speed_1_value
                    print "there is a bottle from ",stm32_4_input_port_speed_1_name
                    print "bottle content: ",speed_1_value
                    
                else:
                    print "there is no bottle from ",stm32_4_input_port_speed_1_name       


            #------not connected to STM32 2----------------
            else:
                connected=False
                print "wrong stm32, detected: ",stm32_3_position
                stm32_3_position='0'
                serial_device_counter=serial_device_counter+1

    except SerialException:
        #com.close()
        #del com
        print com.inWaiting()
        print "error! error!"
        #print line
        connected=False            
