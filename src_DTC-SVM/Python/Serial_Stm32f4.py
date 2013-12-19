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
import datetime
import csv
import serial
from serial import SerialException
import sys
import select
import matplotlib.pyplot as plt

from byte_to_float import *

class Serial_Stm32f4(object):


    def initializing_values(self):

        #pyserial configuration
        self.path = "/home/tumacher/local/src/repositories/arcoslab/experiment/open-coroco/src_DTC-SVM/Python/measures/"
        self.dev_type="/dev/ttyACM"
        self.serial_speed=115200
        self.serial_timeout=1

        #control data for pyserial
        self.connecting      = True
        self.transmition_error =False
        self.counter               = 0
        self.capture_data    = False
        self.capture_counter = 0
        

        #data from stm32F4 (impedance control+DTC-SVM+PID+HALL)
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
        self.te_ref                 = 0.0
        self.Ud                     = 0.0
        self.pi_control             = 0.0
        self.pi_max                 = 0.0   


    def creating_data_vectors(self):
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
        self.te_ref_vector = []
        self.Ud_vector = []
        self.pi_control_vector = []
        self.pi_max_vector = []


    def connecting_to_stm32F4(self):
        while self.connecting==True:
            try:
                self.ser = serial.Serial(self.dev_type+str(self.counter),self.serial_speed , timeout=self.serial_timeout)
                self.connecting=False
                self.ser.flushInput()
                print "Connected to the STM32F4"
            except SerialException:
                print"Disconnected from the STM32, cua cua"
                self.counter=self.counter+1
                if (self.counter>100):
                    self.counter=0
                self.connecting=True                

  
    def create_log_file(self):
        self.log_file = open( self.path +"data" +"." + datetime.datetime.now().ctime() +".csv", "wb")
        self.writer   = csv.writer(self.log_file, delimiter=' ',quotechar=' ', quoting=csv.QUOTE_MINIMAL)
        header_csv    = "t t ref_freq ref_freq electric_frequency electric_frequency hall_freq hall_freq " + \
                        "isA isA isB isB isC isC isD isD isQ isQ "                                         + \
                        "VsD VsD VsQ VsQ Vs Vs Vs_cita Vs_cita Vs_cita_relative Vs_cita_relative "         + \
                        "psisD psisD psisQ psisQ psis psis psis_alpha psis_alpha psis_ref psis_ref "       + \
                        "te te Ud Ud pi_control pi_control pi_max pi_max"  
        split_header = header_csv.split()                  
        self.writer.writerow(split_header)        


    def get_value(self,string,output_variable,split_info,i,error):
        if split_info[i] == string: 
            convertion = bytes_to_float(split_info[i+1])
            if (convertion[0]==True):                       
                output_variable = 27#convertion[1]
                return True    
            else:
                return False 


    def read_data_from_stm32(self):
        bytes = 1
        info = ''   #info needs to be set before splitting it, otherwise pythons says it is uninitialized
        previous_character = self.ser.read(bytes)
        single_character   = self.ser.read(bytes)

        if(previous_character+single_character == "XX"):

            while (previous_character+single_character != "mm"):#"\n"):
                previous_character = single_character
                single_character = self.ser.read(bytes)
                info +=single_character
                    
            split_info = info.split()
            self.transmition_error=False
                
            for i in range( len(split_info)):
                if i+1<len(split_info):
                    
                    #if (error==False): error=self.get_value("t",self.time,split_info,i,error)


                    
                    if   (split_info[i] == "t" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.time = convertion[1]
                                                                        self.transmition_error=False
                                                                    else:   
                                                                        self.transmition_error=True

                    elif   (split_info[i] == "rf" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.reference_frequency = convertion[1]
                                                                        self.transmition_error=False
                                                                    else:   
                                                                        self.transmition_error=True                   


                    elif   (split_info[i] == "f" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.electric_frequency = convertion[1]
                                                                        self.transmition_error=False
                                                                    else:   
                                                                        self.transmition_error=True  

                    elif   (split_info[i] == "h" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.hall_frequency = convertion[1]
                                                                        self.transmition_error=False
                                                                    else:   
                                                                        self.transmition_error=True  


                    elif   (split_info[i] == "iA" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.isA = convertion[1]
                                                                        self.transmition_error=False
                                                                    else:   
                                                                        self.transmition_error=True  

                    elif   (split_info[i] == "iB" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.isB = convertion[1]
                                                                        self.transmition_error=False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "iC" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.isC = convertion[1]
                                                                        self.transmition_error=False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "iD" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.isD = convertion[1]
                                                                        self.transmition_error=False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "iQ" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.isQ = convertion[1]
                                                                        self.transmition_error=False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "VD" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.VsD = convertion[1]
                                                                        self.transmition_error=False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "VQ" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.VsQ = convertion[1]
                                                                        self.transmition_error=False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "Vs" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.Vs = convertion[1]
                                                                        self.transmition_error=False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "Vc" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.Vs_cita = convertion[1]
                                                                        self.transmition_error=False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "Vr" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.Vs_relative_cita = convertion[1]
                                                                        self.transmition_error = False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "pD" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.psi_sD = convertion[1]
                                                                        self.transmition_error = False
                                                                    else:   
                                                                        self.transmition_error=True 


                    elif   (split_info[i] == "pQ" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.psi_sQ = convertion[1]
                                                                        self.transmition_error = False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "ps" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.psi_s = convertion[1]
                                                                        self.transmition_error = False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "pa" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.psi_s_alpha = convertion[1]
                                                                        self.transmition_error = False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "pf" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.psi_s_reference = convertion[1]
                                                                        self.transmition_error = False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "te" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.te = convertion[1]
                                                                        self.transmition_error = False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "tr" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.te_ref = convertion[1]
                                                                        self.transmition_error = False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "Ud" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.Ud = convertion[1]
                                                                        self.transmition_error = False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "pi" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.pi_control = convertion[1]
                                                                        self.transmition_error = False
                                                                    else:   
                                                                        self.transmition_error=True 

                    elif   (split_info[i] == "mx" and self.transmition_error==False)  : 
                                                                    convertion = bytes_to_float(split_info[i+1])
                                                                    if (convertion[0]==True):                      
                                                                        self.pi_max = convertion[1]
                                                                        self.transmition_error = False
                                                                    else:   
                                                                        self.transmition_error=True 
                    '''                        
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
                    elif split_info[i] == "tr"   : self.te_ref             = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "Ud"   : self.Ud                 = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "pi"   : self.pi_control         = bytes_to_float(split_info[i+1])
                    elif split_info[i] == "mx"   : self.pi_max             = bytes_to_float(split_info[i+1])     
                    '''

    def append_new_data_to_vectors(self):
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
                self.te_ref_vector.append(self.te_ref)
                self.Ud_vector.append(self.Ud)
                self.pi_control_vector.append(self.pi_control)
                self.pi_max_vector.append(self.pi_max)        


    def save_data_to_csv_file(self):
                new_data_line_csv=  "t %6.2f "                  %self.time                  + \
                                    " ref_freq %6.2f"           %self.reference_frequency   + \
                                    " electric_frequency %6.2f" %self.electric_frequency    + \
                                    " hall_freq %6.2f"          %self.hall_frequency        + \
                                    " isA %6.2f"                %self.isA                   + \
                                    " isB %6.2f"                %self.isB                   + \
                                    " isC %6.2f"                %self.isC                   + \
                                    " isD %6.2f"                %self.isD                   + \
                                    " isQ %6.2f"                %self.isQ                   + \
                                    " VsD %6.2f"                %self.VsD                   + \
                                    " VsQ %6.2f"                %self.VsQ                   + \
                                    " Vs %6.2f"                 %self.Vs                    + \
                                    " Vs_cita %6.2f"            %self.Vs_cita               + \
                                    " Vs_cita_relative %6.2f"   %self.Vs_relative_cita      + \
                                    " psisD %6.2f"              %self.psi_sD                + \
                                    " psisQ %6.2f"              %self.psi_sQ                + \
                                    " psis %6.2f"               %self.psi_s                 + \
                                    " psis_alpha %6.2f"         %self.psi_s_alpha           + \
                                    " psis_ref %6.5f"           %self.psi_s_reference       + \
                                    " te %6.2f"                 %self.te                    + \
                                    " Ud %6.2f"                 %self.Ud                    + \
                                    " pi_control %6.2f"         %self.pi_control            + \
                                    " pi_max %6.2f"             %self.pi_max  
                split_new_data_line = new_data_line_csv.split()
                self.writer.writerow(split_new_data_line)     


    def print_to_console(self):
            new_data_line=  "t: %6.2f "                  %self.time                  + \
                            " ref_freq: %6.2f"           %self.reference_frequency   + \
                            " electric_frequency: %6.2f" %self.electric_frequency    + \
                            " hall_freq: %6.2f"          %self.hall_frequency        + \
                            " isA: %6.2f"                %self.isA                   + \
                            " isB: %6.2f"                %self.isB                   + \
                            " isC: %6.2f"                %self.isC                   + \
                            " isD: %6.2f"                %self.isD                   + \
                            " isQ: %6.2f"                %self.isQ                   + \
                            " VsD: %6.2f"                %self.VsD                   + \
                            " VsQ: %6.2f"                %self.VsQ                   + \
                            " Vs: %6.2f"                 %self.Vs                    + \
                            " Vs_cita: %6.2f"            %self.Vs_cita               + \
                            " Vs_cita_relative: %6.2f"   %self.Vs_relative_cita      + \
                            " psisD: %10.6f"             %self.psi_sD               + \
                            " psisQ: %10.6f"             %self.psi_sQ                + \
                            " psis: %10.6f"              %self.psi_s                 + \
                            " psis_alpha: %6.2f"         %self.psi_s_alpha           + \
                            " psis_ref: %10.8f"          %self.psi_s_reference       + \
                            " te: %10.6f"                %self.te                    + \
                            " Ud: %6.2f"                 %self.Ud                    + \
                            " pi_control: %10.8f"        %self.pi_control            + \
                            " pi_max %10.8f:"            %self.pi_max
            print   new_data_line        


    def plot_frequencies(self,rows,columns,subplot_index):
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector,self.electric_frequency_vector ,label='electric' )
                        plt.plot(self.time_vector,self.hall_frequency_vector     ,label='hall'     )
                        plt.plot(self.time_vector,self.reference_frequency_vector,label='reference')                     
                        plt.title('frequency vs time')
                        plt.xlabel('time (ticks)')
                        plt.ylabel('frequency (Hz)')
                        plt.legend()

    def plot_three_phase_currents(self,rows,columns,subplot_index):
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector, self.isA_vector,label='isA')
                        plt.plot(self.time_vector, self.isB_vector,label='isB')
                        plt.plot(self.time_vector, self.isC_vector,label='isC')
                        plt.title('current vs time')
                        plt.xlabel('time (ticks)')
                        plt.ylabel('three-phase currents (A)')
                        plt.legend()

    def plot_quadrature_vs_direct_currents(self,rows,columns,subplot_index):
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.isD_vector, self.isQ_vector)
                        plt.title('isQ vs isD')
                        plt.xlabel('isD (A)')
                        plt.ylabel('isQ (A)')
                        plt.legend() 

    def plot_quadrature_vs_direct_voltages(self,rows,columns,subplot_index):
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.VsD_vector, self.VsQ_vector)
                        plt.title('VsQ vs VsD')
                        plt.xlabel('VsD (A)')
                        plt.ylabel('VsQ (A)')
                        plt.legend() 

    def plot_voltage_magnitude(self,rows,columns,subplot_index):    
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector, self.Vs_vector,label='Vs')
                        plt.plot(self.time_vector, self.Ud_vector,label='Ud')
                        plt.title('voltage vs time')
                        plt.xlabel('t (ticks)')
                        plt.ylabel('Voltage (V)')
                        plt.legend()     

    def plot_flux_linkage(self,rows,columns,subplot_index):    
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.psi_sD_vector, self.psi_sQ_vector)
                        plt.title('psi_sQ vs psi_sD')
                        plt.xlabel('psi_sD (Wb)')
                        plt.ylabel('psi_sQ (Wb)')
                        plt.legend() 

    def plot_electromagnetic_torque(self,rows,columns,subplot_index):    
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector, self.te_vector,label='te')
                        plt.plot(self.time_vector, self.te_ref_vector,label='te_ref')
                        plt.title('torque vs time')
                        plt.xlabel('time (ticks)')
                        plt.ylabel('torque (Nm)')
                        plt.legend() 

    def plot_phase_advance(self,rows,columns,subplot_index):    
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector, self.pi_control_vector,label='pi_control')
                        plt.plot(self.time_vector, self.pi_max_vector,label='pi_max')
                        plt.title('pi increment vs time')
                        plt.xlabel('time (ticks)')
                        plt.ylabel('pi (degrees)')
                        plt.legend()

    def plot_all_in_one(self):
                        rows = 4
                        columns = 2 
                        subplot_index = 1
                        
                        plt.figure(num=1, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')  
                        
                        self.plot_frequencies                   (rows,columns,subplot_index)
                        subplot_index=subplot_index+1

                        self.plot_three_phase_currents          (rows,columns,subplot_index)
                        subplot_index=subplot_index+1

                        self.plot_quadrature_vs_direct_currents (rows,columns,subplot_index)
                        subplot_index=subplot_index+1    

                        self.plot_quadrature_vs_direct_voltages (rows,columns,subplot_index)
                        subplot_index=subplot_index+1

                        self.plot_voltage_magnitude             (rows,columns,subplot_index)
                        subplot_index=subplot_index+1
               
                        self.plot_flux_linkage                       (rows,columns,subplot_index)
                        subplot_index=subplot_index+1    

                        self.plot_electromagnetic_torque             (rows,columns,subplot_index)
                        subplot_index=subplot_index+1    

                        self.plot_phase_advance                      (rows,columns,subplot_index)
                        subplot_index=subplot_index+1  
                        
                        '''
                        left  = 0.125  # the left side of the subplots of the figure
                        right = 0.9    # the right side of the subplots of the figure
                        bottom = 0.1   # the bottom of the subplots of the figure
                        top = 0.9      # the top of the subplots of the figure
                        wspace = 0.2   # the amount of width reserved for blank space between subplots
                        hspace = 0.2   # the amount of height reserved for white space between subplots
                        '''
                        plt.subplots_adjust(hspace=0.4)
                        plt.subplots_adjust(wspace=0.2)
                        #plt.show()                        
                        plt.savefig(self.path+ "all_graphs" +"." + datetime.datetime.now().ctime() +".jpg")


    def plot_one_by_one(self):
                        rows = 1
                        columns = 1 
                        subplot_index = 1
                                              
                        plt.figure(num=2, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k') 
                        self.plot_frequencies                   (rows,columns,subplot_index)
                        plt.savefig(self.path+ "frequencies" +"." + datetime.datetime.now().ctime() +".jpg")

                        plt.figure(num=3, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k') 
                        self.plot_three_phase_currents          (rows,columns,subplot_index)
                        plt.savefig(self.path+ "three-phase_currents" +"." + datetime.datetime.now().ctime() +".jpg")

                        plt.figure(num=4, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k') 
                        self.plot_quadrature_vs_direct_currents (rows,columns,subplot_index)
                        plt.savefig(self.path+ "isQ_vs_isD" +"." + datetime.datetime.now().ctime() +".jpg")

                        plt.figure(num=5, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_quadrature_vs_direct_voltages (rows,columns,subplot_index)
                        plt.savefig(self.path+ "VsQ_vs_VsD" +"." + datetime.datetime.now().ctime() +".jpg")

                        plt.figure(num=6, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_voltage_magnitude             (rows,columns,subplot_index)
                        plt.savefig(self.path+ "voltage_magnitude" +"." + datetime.datetime.now().ctime() +".jpg")
               
                        plt.figure(num=7, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_flux_linkage                       (rows,columns,subplot_index)
                        plt.savefig(self.path+ "flux-linkage" +"." + datetime.datetime.now().ctime() +".jpg")

                        plt.figure(num=8, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_electromagnetic_torque             (rows,columns,subplot_index)
                        plt.savefig(self.path+ "electromagnetic_torque" +"." + datetime.datetime.now().ctime() +".jpg")

                        plt.figure(num=9, figsize=(20, 20), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_phase_advance                      (rows,columns,subplot_index)
                        plt.savefig(self.path+ "phase_advance_pi" +"." + datetime.datetime.now().ctime() +".jpg")
       


    def __init__(self):
        self.initializing_values()        
        self.creating_data_vectors()
        self.create_log_file()
        self.connecting_to_stm32F4()
               


            

    def __del__(self):
        self.log_file.close()
        self.ser.close()
        print "---Serial port closed: disconnected from the STM32F4---"
	

        	    	    		
		
    def read(self): 
            self.read_data_from_stm32()
                                        
            if self.capture_data==True:
                self.append_new_data_to_vectors()
                self.save_data_to_csv_file()

            self.print_to_console() 



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
                        self.plot_all_in_one()
                        self.plot_one_by_one()



