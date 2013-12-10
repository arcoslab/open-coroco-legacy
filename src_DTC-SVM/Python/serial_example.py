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
        tuplet_float= struct.unpack_from('f',string_of_bytes)   #uncompress string of 4 bytes into a floting number
        return tuplet_float[0]


class Serial_Stm32f4(object):
	

    def __init__(self):
        connecting=True;
        i=0

        self.time                   = 0.0
        '''
        self.reference_frequency    = 0.0
        self.electric_frequency     = 0.0
        self.hall_frequency         = 0.0
  
        self.isA                    = 0.0
        self.isB                    = 0.0
        self.isC                    = 0.0
        self.isD                    = 0.0
        self.isQ                    = 0.0
   
        self.VsD                    = 0.0
        self.VsQ                    = 0.0
        self.Vs                     = 0.0
        self.Vs_cita                = 0.0
        self.Vs_relative_cita       = 0.0

        self.psi_sD                 = 0.0
        self.psi_sQ                 = 0.0
        self.psi_s                  = 0.0
        self.psi_s_alpha            = 0.0
        self.psi_s_reference        = 0.0
                 
        self.te                     = 0.0
        self.Ud                     = 0.0
        self.pi_control             = 0.0
        self.pi_max                 = 0.0
        '''

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
                    '''
                    if   split_info[i] == "freq_ref" : self.freq_ref  = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "freq"     : self.freq      = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "hall"     : self.hall_freq = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "Ud"       : self.Ud        = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "te_ref"   : self.te_ref    = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "te"       : self.te        = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "size"     : self.size      = bytes_to_float(split_info[i+1])
                    '''  
                    if   split_info[i] == "t"   : self.time                = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "rf"  : self.reference_frequency = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "freq"   : self.electric_frequency  = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "h"   : self.hall_frequency      = bytes_to_float(split_info[i+1])
                    '''
                    elif split_info[i] == "iA"   : self.isA                = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "iB"   : self.isB                = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "iC"   : self.isC                = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "iD"   : self.isD                = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "iQ"   : self.isQ                = bytes_to_float(split_info[i+1])
 
                    elif split_info[i] == "VD"   : self.VsD                = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "VQ"   : self.VsQ                = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "Vs"   : self.Vs                 = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "Vc"   : self.Vs_cita            = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "Vr"   : self.Vs_relative_cita   = bytes_to_float(split_info[i+1])

                    elif split_info[i] == "pD"   : self.psi_sD             = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "pQ"   : self.psi_sQ             = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "ps"   : self.psi_s              = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "pa"   : self.psi_s_alpha        = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "pf"   : self.psi_s_reference    = bytes_to_float(split_info[i+1])
                 
                    elif split_info[i] == "te"   : self.te                 = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "Ud"   : self.Ud                 = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "pi"   : self.pi_control         = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "mx"   : self.pi_max             = bytes_to_float(split_info[i+1])
                    '''


            for data in split_info:    
                #print "reference frequency: %6.2f " % self.freq
                #print "reference frequency: %6.2f " % self.freq_ref +"hall_freq: %6.2f " % self.hall_freq+" electric_frequency: %6.2f "%self.freq +"Ud: %6.2f " % self.Ud+"te_ref: %6.2f "%self.te_ref+"te: %6.2f "%self.te
                #sys.stdout.write("\n")
                if (self.time>200.0):
                    print   "t: %6.2f " %self.time+" ref_freq: %6.2f" %self.reference_frequency+" electric_frequency: %6.2f" %self.electric_frequency+" hall_freq: %6.2f" %self.hall_frequency#+"isA: %6.2f" %self.isA+"isB: %6.2f" %self.isB+"isC: %6.2f" %self.isC+"isD: %6.2f" %self.isD+"isQ: %6.2f" %self.isQ+"VsD: %6.2f" %self.VsD+"VsQ: %6.2f" %self.VsQ+"Vs: %6.2f" %self.Vs+"Vs_cita: %6.2f" %self.Vs_cita+"Vs_cita_relative: %6.2f" %self.Vs_relative_cita+"psisD: %6.2f" %self.psi_sD+"psisQ: %6.2f" %self.psi_sQ+"psis: %6.2f" %self.psi_s+"psis_alpha: %6.2f" %self.psi_s_alpha+"psis_ref: %6.5f" %self.psi_s_reference+"te: %6.2f" %self.te+"Ud: %6.2f" %self.Ud+"pi_control: %6.2f" %self.pi_control+"pi_max: %6.2f" %self.pi_max
     

    def write(self):	
         while sys.stdin in select.select([sys.stdin], [], [], 0)[0]:	#read from standart input if there is something, otherwise not 
            line = sys.stdin.readline()     #you must press enter before typing the new command
            if line:
                line=raw_input("Enter new command: ")   #the printing of the stm32f4 data is stopped until you type a new reference
                if line:
                    self.ser.write(line)
                    self.ser.write('\n')
                    self.ser.write('\r')
                else: # an empty line means stdin has been closed
                    print "nothing"




def main():
    try:
        stm32f4 = Serial_Stm32f4()
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
	

