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
import matplotlib.pyplot as plt
import datetime

def bytes_to_float(string_of_bytes):
    if len(string_of_bytes)<4:  #if there are not 4 bytes then it cannot be translated into a float and therefore an error is obtained
        return 25.0
    else:
        tuplet_float= struct.unpack_from('f',string_of_bytes)   #uncompress string of 4 bytes into a floting number
        return tuplet_float[0]


class Serial_Stm32f4(object):
	

    def __init__(self):
        connecting=True;
        i=0

        self.capture_data    = False
        self.capture_counter = 0

        self.time_vector = []
        self.reference_frequency_vector = []
        self.electric_frequency_vector = []
        self.hall_frequency_vector = []
  
        self.isA_vector = []
        self.isB_vector = []
        self.isC_vector = []
        self.isD_vector = []
        self.isQ_vector = []
   
        self.VsD_vector = []
        self.VsQ_vector = []
        self.Vs_vector = []
        self.Vs_cita_vector = []
        self.Vs_relative_cita_vector = []

        self.psi_sD_vector = []
        self.psi_sQ_vector = []
        self.psi_s_vector = []
        self.psi_s_alpha_vector = []
        self.psi_s_reference_vector = []
                 
        self.te_vector = []
        self.Ud_vector = []
        self.pi_control_vector = []
        self.pi_max_vector = []
        '''
        self.time                   = 0.0
       
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
        previous_character = self.ser.read(bytes)
        single_character   = self.ser.read(bytes)

        if(previous_character+single_character == "XX"):

            while (previous_character+single_character != "mm"):#"\n"):
                previous_character = single_character
                single_character = self.ser.read(bytes)
                info +=single_character
                    
            '''
            if len(info)>=94:
                self.time                = bytes_to_float(info[0]+info[1]+info[2]+info[3])
                self.reference_frequency = bytes_to_float(info[4]+info[5]+info[6]+info[7])
                self.electric_frequency  = bytes_to_float(info[8]+info[9]+info[10]+info[11])
            '''
    
                #print   "t: %6.2f " %self.time+" ref_freq: %6.2f" %self.reference_frequency+" electric_frequency: %6.2f" %self.electric_frequency#+" hall_freq: %6.2f" %self.hall_frequency  +"isA: %6.2f" %self.isA+"isB: %6.2f" %self.isB+"isC: %6.2f" %self.isC+"isD: %6.2f" %self.isD+"isQ: %6.2f" %self.isQ+"VsD: %6.2f" %self.VsD+"VsQ: %6.2f" %self.VsQ+"Vs: %6.2f" %self.Vs+"Vs_cita: %6.2f" %self.Vs_cita+"Vs_cita_relative: %6.2f" %self.Vs_relative_cita+"psisD: %6.2f" %self.psi_sD+"psisQ: %6.2f" %self.psi_sQ+"psis: %6.2f" %self.psi_s+"psis_alpha: %6.2f" %self.psi_s_alpha+"psis_ref: %6.5f" %self.psi_s_reference+"te: %6.2f" %self.te+"Ud: %6.2f" %self.Ud+"pi_control: %6.2f" %self.pi_control+"pi_max: %6.2f" %self.pi_max
             
            #print len(info)  
            split_info = info.split()
            '''
            self.time                = bytes_to_float(split_info[0])
            self.reference_frequency = bytes_to_float(split_info[1])
            self.electric_frequency  = bytes_to_float(split_info[2])
            self.hall_frequency      = bytes_to_float(split_info[3])
                              
            self.isA                = bytes_to_float(split_info[4])
            self.isB                = bytes_to_float(split_info[5])
            self.isC                = bytes_to_float(split_info[6])
            self.isD                = bytes_to_float(split_info[7])
            self.isQ                = bytes_to_float(split_info[8])
 
            self.VsD                = bytes_to_float(split_info[9])
            self.VsQ                = bytes_to_float(split_info[10])
            self.Vs                 = bytes_to_float(split_info[11])
            self.Vs_cita            = bytes_to_float(split_info[12])
            self.Vs_relative_cita   = bytes_to_float(split_info[13])

            self.psi_sD             = bytes_to_float(split_info[14])
            self.psi_sQ             = bytes_to_float(split_info[15])
            self.psi_s              = bytes_to_float(split_info[16])
            self.psi_s_alpha        = bytes_to_float(split_info[17])
            self.psi_s_reference    = bytes_to_float(split_info[18])
                 
            self.te                 = bytes_to_float(split_info[19])
            self.Ud                 = bytes_to_float(split_info[20])
            self.pi_control         = bytes_to_float(split_info[21])
            self.pi_max             = bytes_to_float(split_info[22])
             '''
                        
            for i in range( len(split_info)):
                if i+1<len(split_info):

                    if   split_info[i] == "t"   : self.time                = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "rf"  : self.reference_frequency = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "f"   : self.electric_frequency  = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "h"   : self.hall_frequency      = bytes_to_float(split_info[i+1])
                    
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
                                        
            if self.capture_data==True:
                '''
                self.time_vector                [self.capture_counter]=self.time
                self.reference_frequency_vector [self.capture_counter]=self.reference_frequency
                self.electric_frequency_vector  [self.capture_counter]=self.electric_frequency
                self.hall_frequency_vector      [self.capture_counter]=self.hall_frequency
  
                self.isA_vector                 [self.capture_counter]=self.isA
                self.isB_vector                 [self.capture_counter]=self.isB
                self.isC_vector                 [self.capture_counter]=self.isC
                self.isD_vector                 [self.capture_counter]=self.isD
                self.isQ_vector                 [self.capture_counter]=self.isQ
   
                self.VsD_vector                 [self.capture_counter]=self.VsD
                self.VsQ_vector                 [self.capture_counter]=self.VsQ
                self.Vs_vector                  [self.capture_counter]=self.Vs
                self.Vs_cita_vector             [self.capture_counter]=self.Vs_cita_vector
                self.Vs_relative_cita_vector    [self.capture_counter]=self.Vs_relative_cita_vector

                self.psi_sD_vector              [self.capture_counter]=self.psi_sD
                self.psi_sQ_vector              [self.capture_counter]=self.psi_sQ
                self.psi_s_vector               [self.capture_counter]=self.psi_s_vector
                self.psi_s_alpha_vector         [self.capture_counter]=self.psi_s_alpha_vector
                self.psi_s_reference_vector     [self.capture_counter]=self.psi_s_reference_vector
                         
                self.te_vector                  [self.capture_counter]=self.te
                self.Ud_vector                  [self.capture_counter]=self.Ud
                self.pi_control_vector          [self.capture_counter]=self.pi_control
                self.pi_max_vector              [self.capture_counter]=self.pi_max

                self.capture_counter=self.capture_counter+1           
                '''
                self.time_vector.append(self.time)
                self.reference_frequency_vector.append(self.reference_frequency)
                self.electric_frequency_vector.append(self.electric_frequency)
                self.hall_frequency_vector.append(self.hall_frequency)
  
                self.isA_vector.append(self.isA)
                self.isB_vector.append(self.isB)
                self.isC_vector.append(self.isC)
                self.isD_vector.append(self.isD)
                self.isQ_vector.append(self.isQ)
   
                self.VsD_vector.append(self.VsD)
                self.VsQ_vector.append(self.VsQ)
                self.Vs_vector.append(self.Vs)
                self.Vs_cita_vector.append(self.Vs_cita_vector)
                self.Vs_relative_cita_vector.append(self.Vs_relative_cita_vector)

                self.psi_sD_vector.append(self.psi_sD)
                self.psi_sQ_vector.append(self.psi_sQ)
                self.psi_s_vector.append(self.psi_s_vector)
                self.psi_s_alpha_vector.append(self.psi_s_alpha_vector)
                self.psi_s_reference_vector.append(self.psi_s_reference_vector)
                         
                self.te_vector.append(self.te)
                self.Ud_vector.append(self.Ud)
                self.pi_control_vector.append(self.pi_control)
                self.pi_max_vector.append(self.pi_max)

            #for data in split_info:    
                #print "reference frequency: %6.2f " % self.freq
                #print "reference frequency: %6.2f " % self.freq_ref +"hall_freq: %6.2f " % self.hall_freq+" electric_frequency: %6.2f "%self.freq +"Ud: %6.2f " % self.Ud+"te_ref: %6.2f "%self.te_ref+"te: %6.2f "%self.te
                #sys.stdout.write("\n")
            print   "t: %6.2f " %self.time+" ref_freq: %6.2f" %self.reference_frequency+" electric_frequency: %6.2f" %self.electric_frequency+" hall_freq: %6.2f" %self.hall_frequency  +" isA: %6.2f" %self.isA+" isB: %6.2f" %self.isB+" isC: %6.2f" %self.isC+" isD: %6.2f" %self.isD+" isQ: %6.2f" %self.isQ+" VsD: %6.2f" %self.VsD+" VsQ: %6.2f" %self.VsQ+" Vs: %6.2f" %self.Vs+" Vs_cita: %6.2f" %self.Vs_cita+" Vs_cita_relative: %6.2f" %self.Vs_relative_cita+"psisD: %6.2f" %self.psi_sD+" psisQ: %6.2f" %self.psi_sQ+" psis: %6.2f" %self.psi_s+" psis_alpha: %6.2f" %self.psi_s_alpha+" psis_ref: %6.5f" %self.psi_s_reference+"te: %6.2f" %self.te+" Ud: %6.2f" %self.Ud+" pi_control: %6.2f" %self.pi_control+" pi_max: %6.2f" %self.pi_max
     

    def write(self):	
         while sys.stdin in select.select([sys.stdin], [], [], 0)[0]:	#read from standart input if there is something, otherwise not 
            line = sys.stdin.readline()     #you must press enter before typing the new command
            if line:
                line=raw_input("Enter new command: ")   #the printing of the stm32f4 data is stopped until you type a new reference
                if line:
                    self.ser.write(line)
                    self.ser.write('\n')
                    self.ser.write('\r')

                    split_command = line.split()
                    if split_command[0]=='c':
                        self.capture_data=True
                        self.capture_counter=0
                    elif split_command[0]=='f':
                        self.capture_data=False

                        plt.subplot(2, 1, 1)
                        plt.plot(self.time_vector, self.electric_frequency_vector)
                        plt.title('frequency vs time')
                        #plt.text(2, 0.65, r'$\cos(2 \pi t) \exp(-t)$', fontdict=font)
                        plt.xlabel('time (ticks)')
                        plt.ylabel('frequency (Hz)')
             
                        plt.subplot(2, 1, 2)
                        plt.plot(self.time_vector, self.isA_vector)
                        plt.title('current vs time')
                        #plt.text(2, 0.65, r'$\cos(2 \pi t) \exp(-t)$', fontdict=font)
                        plt.xlabel('time (ticks)')
                        plt.ylabel('isA (A)')

                        #plt.subplots_adjust(left=0.15)
                        #plt.tight_layout(pad=1.08, h_pad=None, w_pad=None, rect=None)
                        
                        spacing=0.9
                        '''
                        left  = 0.125  # the left side of the subplots of the figure
                        right = 0.9    # the right side of the subplots of the figure
                        bottom = 0.1   # the bottom of the subplots of the figure
                        top = 0.9      # the top of the subplots of the figure
                        wspace = 0.2   # the amount of width reserved for blank space between subplots
                        hspace = 0.2   # the amount of height reserved for white space between subplots
                        '''
                        #plt.subplots_adjust(left=0.1*spacing, bottom=0.1*spacing, right=spacing, top=spacing, wspace=spacing, hspace=spacing)
                        plt.subplots_adjust(hspace=0.3)
                        plt.show()

                        '''
                        plt.plot(self.time_vector,self.electric_frequency_vector)                  
                        #plt.show()
                        plt.savefig("/home/tumacher/local/src/repositories/arcoslab/experiment/open-coroco/src_DTC-SVM/Python/measures/"+ "electric_frequency vs time" +"." + datetime.datetime.now().ctime() +".jpg")
                        plt.plot(self.time_vector,self.isA_vector)    
                        #plt.show()                        
                        plt.savefig("/home/tumacher/local/src/repositories/arcoslab/experiment/open-coroco/src_DTC-SVM/Python/measures/"+ "isA vs time" +"." + datetime.datetime.now().ctime() +".jpg")                        
                        '''
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
	

