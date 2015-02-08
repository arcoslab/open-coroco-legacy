#!/usr/bin/env python

#importing libraries
import yarp as y
from time import sleep


#starting yarp network
y.Network.init()


#creting output port
stm32_1_output_port_1=y.BufferedPortBottle()
stm32_1_output_port_1_name="/stm32_1/out1"
stm32_1_output_port_1.open(stm32_1_output_port_1_name)

#creating an input port
stm32_1_input_port_1        =y.BufferedPortBottle()
stm32_1_input_port_1_name   ="/stm32_1/in1"
stm32_1_input_port_1.open(stm32_1_input_port_1_name)


#connecting with remote port
style=y.ContactStyle()
style.persistent=1
stm32_2_input_port_1_name="/stm32_2/in1"

y.Network.connect(stm32_1_output_port_1_name,stm32_2_input_port_1_name,style)


while True:
    #sending a bottle
    stm32_1_output_bottle_1=stm32_1_output_port_1.prepare()
    stm32_1_output_bottle_1.clear()
    stm32_1_output_bottle_1.addString("hola1")
    stm32_1_output_port_1.write()
    sleep(0.01)

    #receiving a bottle from input port 1
    stm32_1_input_bottle_1=stm32_1_input_port_1.read(False)
    if stm32_1_input_bottle_1:
        stm32_1_input_data_1=stm32_1_input_bottle_1.get(0)
        stm32_2_position=stm32_1_input_data_1.asString()
        print "there is a bottle from ",stm32_2_input_port_1_name
        print "bottle content: ",stm32_2_position
        
    else:
        print "there is no bottle from ",stm32_2_input_port_1_name    

