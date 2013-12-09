#!/usr/bin/python

#/*
# *This file is part of the open-coroco project.
# *
# *  Copyright (C) 2013  Sebastian Chinchilla Gutierrez <tumacher@gmail.com>
# *
# *  This program is free software: you can redistribute it and/or modify
# *  it under the terms of the GNU General Public License as published by
# *  the Free Software Foundation, either version 3 of the License, or
# *  (at your option) any later version.
# *
# *  This program is distributed in the hope that it will be useful,
# *  but WITHOUT ANY WARRANTY; without even the implied warranty of
# *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# *  GNU General Public License for more details.
# *
# *  You should have received a copy of the GNU General Public License
# *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
# */

import serial
from serial import SerialException
import struct
import select
import sys


def bytes_to_float(string_of_bytes):
    if len(string_of_bytes)<4:  #if there are not 4 bytes then it cannot be translated into a float and therefore an error is obtained
        return 0.0
    else:
        tuplet_float= struct.unpack_from('f',string_of_bytes)
        return tuplet_float[0]


class Serial_Stm32f4(object):
	
    def __init__(self):
        connecting=True;
        i=0

        while connecting==True:
            try:
                self.ser = serial.Serial("/dev/ttyACM"+str(i), 115200, timeout=1)
                connecting=False
                self.ser.flushInput()
                print "Connected to the STM32F4"
            except SerialException:
                print"Disconnected from the STM32, cua cua"
                i=i+1
                if (i>100):
                    i=0
                connecting=True
            

    def __del__(self):
        self.ser.close()
        print "---Serial port closed: disconnected from the STM32F4---"
	

        	    	    		
		
    def read(self): 
        bytes = 1
        info = ''   #info needs to be set before splitting it, otherwise pythons says it is uninitialized
        single_character = self.ser.read(bytes)

        if(single_character =="X"):

            while (single_character != "\n"):
                single_character = self.ser.read(bytes)
                info +=single_character

            split_info = info.split()
	  
            for i in range( len(split_info)):
                if i+1<len(split_info):
                    if   split_info[i] == "freq_ref" : self.freq_ref  = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "freq"     : self.freq      = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "hall"     : self.hall_freq = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "Ud"       : self.Ud        = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "te_ref"   : self.te_ref    = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "te"       : self.te        = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "size"     : self.size      = bytes_to_float(split_info[i+1])
  
            if sys.stdin in select.select([sys.stdin], [], [], 0)[0]:
                print "line"
            #else:    
                #for data in split_info:    
                    #print "reference frequency: %6.2f " % self.freq_ref +"hall_freq: %6.2f " % self.hall_freq+" electric_frequency: %6.2f "%self.freq +"Ud: %6.2f " % self.Ud+"te_ref: %6.2f "%self.te_ref+"te: %6.2f "%self.te
                #sys.stdout.write("\n")

     

    def write(self):	
        while sys.stdin in select.select([sys.stdin], [], [], 0)[0]:	#read from standart input if there is something, otherwise not 
            line = sys.stdin.readline()
            #print "print"+line
            sys.stdout.write("out"+line)
            if line:
                self.ser.write(line)
                self.ser.write('\n')
                self.ser.write('\r')
            else: # an empty line means stdin has been closed
                print "nothing"




def main():
    try:
        stm32f4 = Serial_Stm32f4()
        #line ="d 200"
        #stm32f4.ser.write(line)
        #stm32f4.ser.write('\n')
        #stm32f4.ser.write('\r')
        while True:
            stm32f4.read()
            stm32f4.write()
    except KeyboardInterrupt:
        print " " 
        print "Sorry, Ctrl-C..."
    except SerialException:
                print"STM32F4 disconnected, cua cua"
   


if __name__ == '__main__':
	main()
	



'''
connecting=True;
i=5

while connecting==True:
  try:
    stm32_f4 = serial.Serial("/dev/ttyACM"+str(i), 115200, timeout=1)
    connecting=False
  except SerialException:
    print"exception, cua cua"
    i=i-1
    connecting=True

print "end"


'''

'''
stm32_f4.write("d")
stm32_f4.write(" ")
stm32_f4.write("50")
stm32_f4.write("\n")
stm32_f4.write("\r")


while True:
    character = stm32_f4.read(1)
    sys.stdout.write(character)

    
stm32f4.close()
'''
