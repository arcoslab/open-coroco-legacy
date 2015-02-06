#!/usr/bin/env python

wheel_diameter=5.95 #inches

import yarp as y
import serial as s
from serial import SerialException
from numpy import pi
from time import sleep

y.Network.init()
port=y.BufferedPortBottle()
portname="/coroco/baldor/in2"
port.open(portname)
portout=y.BufferedPortBottle()
portnameout="/coroco/baldor/out2"
portout.open(portnameout)


max_speed=12*2*pi
last_raw_angle=0.
raw_angle=0.
angle=0.
rads=0.



serial_device_counter=0
connected=False
try: 
    while True:
        #connected=False
        while not connected:
            try:
                com=s.Serial("/dev/ttyACM"+str(serial_device_counter), baudrate=921600,timeout=1)#115200,timeout=1)
                #com=s.Serial("/dev/ttyACM"+str(2), baudrate=921600,timeout=1)#115200,timeout=1)
                print "Connecting to /dev/ttyACM"+str(serial_device_counter)
            except:
                serial_device_counter=serial_device_counter+1
                if (serial_device_counter>100):
                  serial_device_counter=0
                print "Connect opencoroco usb cable, trying: /dev/ttyACM"+str(serial_device_counter)         
            else:
                connected=True
                print "Connected to /dev/ttyACM"+str(serial_device_counter) 
            #sleep(0.1)

        while com.inWaiting()>0:
            com.read(com.inWaiting())

        try:
            while connected==True:
                bottle=port.read(False)
                if bottle:

                    yvalue=bottle.get(0)
                    test= bottle.get(1)
                    value=yvalue.asDouble()
                    value_test=test.asString()
                    cmd_speed=value*max_speed


                    #print "cmd_speed: ", cmd_speed
                    com.write("f "+str(cmd_speed)+"\n\r")
                    print "cmd: "+ str(cmd_speed)+"original_value: "+str(value)+"test: "+value_test+""
   


                    line=com.readline()
                    #print line+"cmddddd: "+ str(cmd_speed)
                    #line=com.read(com.inWaiting())
                    #print "Coroco line: ", line
           
                    line_split=line.split()
                    print "line_from_stm: "+line 

                    stm32_position=line_split[0]

                    if stm32_position=='2':
                        print "stm32_position: " + stm32_position
                        est_freq=float(line_split[1])
                        last_raw_angle=float(raw_angle)
                        raw_angle=int(line_split[3])
                        if last_raw_angle>(2**16-2**13) and raw_angle<(2**13):
                            last_raw_angle-=2**16
                        if raw_angle>(2**16-2**13) and last_raw_angle<(2**13):
                            last_raw_angle+=2**16
                        angle+=raw_angle-last_raw_angle
                        rads=angle*2*pi/2**16
                        odometry=rads*((wheel_diameter*0.0254)/2.0)
                        
                        bottleout=portout.prepare()
                        bottleout.clear()
                        bottleout.addDouble(odometry)
                        bottleout.addString(stm32_position)
                        portout.write(True)
                        print "joystick: "+str(value)+"raw angle: "+str(raw_angle)+" rads: "+str(rads)+" odometry: "+str(odometry)+" est_freq: "+str(est_freq)
                    else:
                        connected=False
                        print "wrong stm32, detected: ",stm32_position
                        serial_device_counter=serial_device_counter+1
                        #raw_input("Enter to continue: ") 
                    #print "freq: ",cmd_speed,"joystick: ",value, "raw angle: ", raw_angle, " rads: ", rads, " odometry: ", odometry , " est_freq: ", est_freq
                    
                #while com.inWaiting()>0:
                #    print "Reading stuff", com.inWaiting()
                #    com.read(com.inWaiting())
                #print
                #sleep(1)
        except SerialException:
            #com.close()
            #del com
            print com.inWaiting()
            print "error! error!"
            #print line
            connected=False

except KeyboardInterrupt:
    print " " 
    print "Sorry, Ctrl-C..."
    #raw_input("Enter to continue: ") 
    port.close()
    y.Network.fini()

port.close()
y.Network.fini()
