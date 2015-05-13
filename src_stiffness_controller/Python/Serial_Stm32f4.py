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
import time 
import datetime
import csv
import serial
from serial import SerialException
import sys
import select
import matplotlib.pyplot as plt
import math

from byte_to_float import *
import os

class Serial_Stm32f4(object):


    def initializing_values(self):

        self.time_starting_c_capture=datetime.datetime.now()
        self.force_append =False
        self.empty_vectors=False

        #pyserial configuration
        self.root_path           = "./measures/"
        self.dev_type       ="/dev/ttyACM"
        self.serial_speed   =115200
        self.serial_timeout =1

        #control data for pyserial
        self.connecting         = True
        self.transmition_error  = False
        self.counter            = 0
        self.capture_data       = False
        self.capture_counter    = 0
        self.print_selection    = 15
        self.new_data_line      = ''
        self.read_capture_state = 'not_collecting'
        self.tag_comment        = ''
        #self.aditional_comment=', STATOR_RESISTANCE_TEST 240degrees,MULTI_ROTOR OPEN LOOP,wrong motor parameters,one psi,wr filt,te filt,k 0.2, actual i,open 0.00005f,Ud 40%,psi_ref=0.0016,38kpwm'
        self.aditional_comment=', hall frequency controller, P =0.001'
        self.driving_counter    = 0
        self.various_counter     = 0
        self.type_of_test       = 0        
        self.exception          = 'N'
        self.read_strings       =False

        #debugging finite statemachine
        self.start_debugging=False
        self.debugging_state='initial'

        #plotting
        self.plotting_character     = ''
        self.plotting_character_0   = 'o'
        self.plotting_character_1   = '.'
        self.title_extra            = ''

        #test routine
        self.max_test_time      = 70000#50000#298#100000#100000#50000#100000
        self.min_test_time      = 300
        self.test_routine_state = 'initial'
        self.driving_test_state = 'initial'
        self.various_test_state = 'initial'
        self.start_test         = False
        self.start_driving_test = False
        self.start_various_test = False
        self.test_frequency     = '0'

        #torque test
        self.torque_start_test          =   False
        self.torque_test_routine_state  ='initial'

        self.torque_start_driving_test = False
        self.torque_driving_test_state = 'initial'
        self.torque_driving_counter    = 0

        #P_test
        '''
        #sacred parameters
        self.P              =100.0#0.000000999999997475#0.000001
        self.final_P        =0.000001
        self.P_increment    =2000 
        self.P_test_counter =0
        self.max_P_tests    =10
        self.P_divisor      =1000000000
        '''
        self.start_P_test       = False
        self.P              =600.0#0.000000999999997475#0.000001
        self.final_P        =0.000001
        self.P_increment    =10 
        self.P_test_counter =0
        self.max_P_tests    =5
        self.P_divisor      =10000000
        self.torque_P_speed_state='initial'

        #I tests
        self.start_I_test       = False
        self.I              =1.0#0.000000999999997475#0.000001
        self.final_I        =0.000001
        self.I_increment    =10 
        self.I_test_counter =0
        self.max_I_tests    =11
        self.I_divisor      =10000000000000000000

         #torque P_test
        self.torque_start_P_test   = False
        self.torque_P              =1.0#0.000000999999997475#0.000001
        self.torque_final_P        =0.000001
        self.torque_P_increment    =10 
        self.torque_P_test_counter =0
        self.torque_max_P_tests    =5
        self.torque_P_divisor      =1000000000

        self.torque_P_speed = 0.0
        self.torque_I_speed = 0.0
                

        #I tests
        self.start_I_test   = False
        self.I              =1.0#0.000000999999997475#0.000001
        self.final_I        =0.000001
        self.I_increment    =10 
        self.I_test_counter =0
        self.max_I_tests    =11
        self.I_divisor      =10000000000000000000


        #PI controller finite state machine(stm32)
        self.P_speed=0.0
        self.I_speed=0.0
        self.P_speed_state='initial'
        self.I_speed_state='initial'

        self.capture_c_button = False
        self.capture_c_button_state='initial'

        #data from stm32F4 (impedance control+DTC-SVM+PID+HALL)
        self.time                   = 0.0
       
        self.reference_frequency    = 0.0
        self.electric_frequency     = 0.0
        self.hall_frequency         = 0.0

        self.Ia_peak__short_pulse = 0.0;
        self.Ib_peak__short_pulse = 0.0;
        self.Ic_peak__short_pulse = 0.0;
        self.initial_rotor_angle  = 0.0;
        self.initial_rotor_zone   = 0.0;
        self.absolute_initial_rotor_angle  = 0.0;


  
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

        self.required_VsD                    = 0.0
        self.required_VsQ                    = 0.0
        self.required_Vs                     = 0.0
        self.required_Vs_cita                = 0.0


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
        self.load_angle             = 0.0   

        self.strain_gauge           = 0.0


        self.checksum_stm32         = 0.0
        self.checksum_python        = 0.0

        self.data_1                 = 0.0
        self.data_2                 = 0.0
        self.data_3                 = 0.0
        self.data_4                 = 0.0
        self.data_5                 = 0.0
        self.data_6                 = 0.0

        #admittance controller
        self.stiffness = 0.0
        self.damping   = 0.0
        self.reference_electric_angle = 0.0
        self.reference_mechanic_angle = 0.0
        self.reference_gear_angle     = 0.0
        self.electric_angle           = 0.0
        self.mechanic_angle           = 0.0
        self.gear_angle               = 0.0
        self.electric_frequency       = 0.0
        self.mechanic_frequency       = 0.0
        self.gear_frequency           = 0.0

        self.pole_pairs               = 5.0
        self.gear_ratio               = 200.0

        #hall sensors
        self.halla= 0.0
        self.hallb= 0.0
        self.prev_halla= 0.0
        self.prev_hallb= 0.0

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
   
        self.VsD_vector              = []
        self.VsQ_vector              = []
        self.Vs_vector               = []
        self.Vs_cita_vector          = []
        self.Vs_relative_cita_vector = []

        self.required_VsD_vector              = []
        self.required_VsQ_vector              = []
        self.required_Vs_vector               = []
        self.required_Vs_cita_vector          = []

        self.psi_sD_vector          = []
        self.psi_sQ_vector          = []
        self.psi_s_vector           = []
        self.psi_s_alpha_vector     = []
        self.psi_s_reference_vector = []
                 
        self.te_vector          = []
        self.te_ref_vector      = []
        self.Ud_vector          = []
        self.pi_control_vector  = []
        self.pi_max_vector      = []
        self.load_angle_vector  = []

        self.strain_gauge_vector= []

        self.data_1_vector          = []
        self.data_2_vector          = []
        self.data_3_vector          = []
        self.data_4_vector          = []
        self.data_5_vector          = []

        self.stiffness_vector                = []
        self.damping_vector                  = []
        self.reference_electric_angle_vector = []
        self.reference_mechanic_angle_vector = []
        self.reference_gear_angle_vector     = []
        self.electric_angle_vector           = []
        self.mechanic_angle_vector           = []
        self.gear_angle_vector               = []
        self.electric_frequency_vector       = []
        self.mechanic_frequency_vector       = []
        self.gear_frequency_vector           = []



    def connecting_to_stm32F4(self):
        while self.connecting==True:
            try:
                self.ser = serial.Serial(self.dev_type+str(self.counter),self.serial_speed , timeout=self.serial_timeout)
                self.connecting=False
                self.ser.flushInput()
                print "Connected to the STM32F4"
                
                self.ser.write('p')
                self.ser.write(' ')
                self.ser.write(str(self.print_selection))
                self.ser.write('\n')
                self.ser.write('\r')
                
                
            except SerialException:
                print"Disconnected from the STM32, cua cua"
                self.counter=self.counter+1
                if (self.counter>100):
                    self.counter=0
                self.connecting=True                

  
    def create_log_file(self):

        #self.path=self.root_path+self.tag_comment+'_'+"data" +"." + datetime.datetime.now().ctime() +"."+self.tag_comment+'/'
        #self.path              =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'
        if not os.path.exists(self.path):
            os.makedirs(self.path)#+"data" +"." + datetime.datetime.now().ctime() +"."+self.tag_comment)

        if   self.print_selection==0 : tag='frequencies'
        elif self.print_selection==1 : tag='three-phase_currents'
        elif self.print_selection==2 : tag='isQ_vs_isD'
        elif self.print_selection==3 : tag='VsQ_vs_VsD'
        elif self.print_selection==4 : tag='voltage_magnitude'
        elif self.print_selection==5 : tag='flux-linkage'
        elif self.print_selection==6 : tag='electromagnetic_torque'
        elif self.print_selection==7 : tag='phase_advance_pi'
        elif self.print_selection==8 : tag=''
        elif self.print_selection==9 : tag=''
        elif self.print_selection==13: tag='strain_gauge'
        else                        : tag=''


        self.log_file = open( self.path+"["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"] data " + tag +".csv", "wb")
        self.writer   = csv.writer(self.log_file, delimiter=' ',quotechar=' ', quoting=csv.QUOTE_MINIMAL)
 
        if self.print_selection==9:   header_csv = "t t ref_freq ref_freq electric_frequency electric_frequency hall_freq hall_freq "+\
                                                   "isA isA isB isB isC isC isD isD isQ isQ "                                        +\
                                                   "VsD VsD VsQ VsQ Vs Vs Vs_cita Vs_cita Vs_cita_relative Vs_cita_relative "        +\
                                                   "psisD psisD psisQ psisQ psis psis psis_alpha psis_alpha psis_ref psis_ref "      +\
                                                   "te te Ud Ud pi_control pi_control pi_max pi_max"  
        elif self.print_selection== 0: header_csv ="t t ref_freq ref_freq electric_frequency electric_frequency hall_freq hall_freq " 
        elif self.print_selection== 1: header_csv ="t t isA isA isB isB isC isC " 
        elif self.print_selection== 2: header_csv ="t t isD isD isQ isQ " 
        elif self.print_selection== 3: header_csv ="t t VsD VsD VsQ VsQ "
        elif self.print_selection== 4: header_csv ="t t VsD Vs Vs Ud Ud "
        elif self.print_selection== 5: header_csv ="t t psisD psisD psisQ psisQ "
        elif self.print_selection== 6: header_csv ="t t te te Ud Ud pi_control pi_control pi_max pi_max" 
        elif self.print_selection== 7: header_csv ="t t pi_control pi_control pi_max pi_max" 
        elif self.print_selection==11: header_csv ="t t psis psis alpha alpha ref ref"
        else :                         header_csv =""

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


    def get_data_and_checksum(self):
                    bytes=1
                    info=''
                    i=0
                    while i<4:
                        single_character = self.ser.read(bytes)
                        #print "single_character_byte_to_float: "+single_character + " ord: " +str(ord(single_character))
                        info +=single_character
                        i=i+1;
                    convertion = bytes_to_float(info)
                    if (convertion[0]==True):                      
                        data = convertion[1]
                        self.transmition_error=False
                        #print "found, whaat!"+str(data)
                    else:   
                        self.transmition_error=True
                        data=0
                        #print " four bytes not found" 
                    
                    info_counter=0
                    for ch in info:
                        if info_counter<len(info)-1:
                            self.checksum_python=self.checksum_python+ord(ch)
                            if self.checksum_python>=256:
                                self.checksum_python=self.checksum_python-256
                        #print ord(ch)
                    return data




    def read_data_from_stm32_strings(self):
        bytes = 1
        string=''
        #info = ''   #info needs to be set before splitting it, otherwise pythons says it is uninitialized
        single_character   = self.ser.read(bytes)
        self.checksum_python=0
        self.checksum_stm32=0
        if(single_character == "X"):

            while (single_character != "m"):
                single_character = self.ser.read(bytes)
                string+=single_character


            split_string = string.split()
            split_counter=0
            


            while split_counter<len(split_string) and len(split_string)>1: 
                #print "split_string[split_counter]: " + split_string[split_counter] + " split_string[split_counter+1 ]: "+split_string[split_counter+1]
                print split_counter
                print split_counter+1
                if   (split_string[split_counter]=='t'):   self.time                =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='r'):   self.reference_frequency =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='h'):   self.hall_frequency      =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='e'):   
                    self.electric_frequency  =float(split_string[split_counter+1])
                    self.mechanic_frequency  =self.electric_frequency/self.pole_pairs
                    self.gear_frequency      =self.mechanic_frequency/self.gear_ratio

 
                elif (split_string[split_counter]=='A'):   self.isA =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='B'):   self.isB =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='C'):   self.isC =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='D'):   self.isD =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='Q'):   self.isQ =float(split_string[split_counter+1])

                elif (split_string[split_counter]=='d'):   self.VsD                =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='q'):   self.VsQ                =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='s'):   self.Vs                 =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='c'):   self.Vs_cita            =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='R'):   self.Vs_relative_cita   =float(split_string[split_counter+1])

                elif (split_string[split_counter]=='p'):   self.psi_sD                =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='P'):   self.psi_sQ                =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='L'):   self.psi_s                 =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='O'):   self.psi_s_alpha           =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='v'):   self.psi_s_reference       =float(split_string[split_counter+1])  
                                          
                elif (split_string[split_counter]=='W'):   self.te                =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='y'):   self.te_ref            =float(split_string[split_counter+1])

                elif (split_string[split_counter]=='U'):   self.Ud                =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='l'):   self.pi_control        =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='x'):   self.pi_max            =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='<'):   self.load_angle        =float(split_string[split_counter+1])

                elif (split_string[split_counter]=='G'):   self.strain_gauge      =float(split_string[split_counter+1])

                elif (split_string[split_counter]=='f'):   self.stiffness               =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='M'):   self.damping                 =float(split_string[split_counter+1])
 
                elif (split_string[split_counter]=='E'):   
                    self.reference_electric_angle   =float(split_string[split_counter+1])
                    self.reference_mechanic_angle   =self.reference_electric_angle/self.pole_pairs
                    self.reference_gear_angle       =self.reference_mechanic_angle/self.gear_ratio

                elif (split_string[split_counter]=='J'):   
                    self.reference_mechanic_angle   =float(split_string[split_counter+1])
                    self.reference_gear_angle       =self.reference_mechanical_angle/self.gear_ratio
                    self.reference_electric_angle   =self.reference_mechanical_angle*self.pole_pairs

                elif (split_string[split_counter]=='o'):   
                    self.reference_gear_angle       =float(split_string[split_counter+1])
                    self.reference_mechanic_angle   =self.reference_gear_angle*self.gear_ratio
                    self.reference_electric_angle   =self.reference_mechanic_angle*self.pole_pairs

                elif (split_string[split_counter]=='g'):   
                    self.electric_angle             =float(split_string[split_counter+1])   
                    self.mechanic_angle             =self.electric_angle/self.pole_pairs
                    self.gear_angle                 =self.mechanic_angle/self.gear_ratio
      
                elif (split_string[split_counter]=='b'):   
                    self.mechanic_angle             =float(split_string[split_counter+1])
                    self.electric_angle             =self.mechanic_angle*self.pole_pairs
                    self.gear_angle                 =self.mechanic_angle/self.gear_ratio

                elif (split_string[split_counter]=='a'):   
                    self.gear_angle                 =float(split_string[split_counter+1]) 
                    self.mechanic_angle             =self.gear_angle*self.gear_ratio
                    self.electric_angle             =self.mechanic_angle*self.pole_pairs

                elif (split_string[split_counter]=='F'):   
                    self.mechanic_frequency         =float(split_string[split_counter+1]) 
                    self.gear_frequency             =self.mechanic_frequency/self.gear_ratio
                    self.electric_frequency         =self.mechanic_frequency/self.pole_pairs            

                elif (split_string[split_counter]=='H'):   
                    self.gear_frequency             =float(split_string[split_counter+1]) 
                    self.mechanic_frequency         =self.gear_frequency*self.gear_ratio
                    self.electric_frequency         =self.mechanic_frequency*self_pole_pairs



                '''
                elif (split_string[split_counter]=='1'):   self.data_1            =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='2'):   self.data_2            =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='3'):   self.data_3            =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='4'):   self.data_4            =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='5'):   self.data_5            =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='6'):   self.data_6            =float(split_string[split_counter+1])

                elif (split_string[split_counter]=='K'):   self.P_speed            =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='I'):   self.I_speed            =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='j'):   self.torque_P_speed     =float(split_string[split_counter+1])
                elif (split_string[split_counter]=='i'):   self.torque_I_speed     =float(split_string[split_counter+1])  
                '''
                split_counter+=1


    def read_data_from_stm32(self):
        bytes = 1
        #info = ''   #info needs to be set before splitting it, otherwise pythons says it is uninitialized
        single_character   = self.ser.read(bytes)
        self.checksum_python=0
        self.checksum_stm32=0
        if(single_character == "X"):

            while (single_character != "m"):
                single_character = self.ser.read(bytes)

                if   (single_character=='t'):   self.time                =self.get_data_and_checksum()
                elif (single_character=='r'):   self.reference_frequency =self.get_data_and_checksum()
                elif (single_character=='h'):   self.hall_frequency      =self.get_data_and_checksum()
                elif (single_character=='e'):   
                    self.electric_frequency  =self.get_data_and_checksum()
                    self.mechanic_frequency  =self.electric_frequency/self.pole_pairs
                    self.gear_frequency      =self.mechanic_frequency/self.gear_ratio
 
                elif (single_character=='A'):   self.isA =self.get_data_and_checksum()
                elif (single_character=='B'):   self.isB =self.get_data_and_checksum()
                elif (single_character=='C'):   self.isC =self.get_data_and_checksum()
                elif (single_character=='D'):   self.isD =self.get_data_and_checksum()
                elif (single_character=='Q'):   self.isQ =self.get_data_and_checksum()

                elif (single_character=='d'):   self.VsD                =self.get_data_and_checksum()
                elif (single_character=='q'):   self.VsQ                =self.get_data_and_checksum()
                elif (single_character=='s'):   self.Vs                 =self.get_data_and_checksum()
                elif (single_character=='c'):   self.Vs_cita            =self.get_data_and_checksum()
                elif (single_character=='R'):   self.Vs_relative_cita   =self.get_data_and_checksum()

                elif (single_character=='p'):   self.psi_sD                =self.get_data_and_checksum()
                elif (single_character=='P'):   self.psi_sQ                =self.get_data_and_checksum()
                elif (single_character=='L'):   self.psi_s                 =self.get_data_and_checksum()
                elif (single_character=='O'):   self.psi_s_alpha           =self.get_data_and_checksum()
                elif (single_character=='v'):   self.psi_s_reference       =self.get_data_and_checksum()  
                                                #print "entering v ord: " + str(ord('v')) 
                elif (single_character=='W'):   self.te                =self.get_data_and_checksum()
                elif (single_character=='y'):   self.te_ref            =self.get_data_and_checksum()

                elif (single_character=='U'):   self.Ud                =self.get_data_and_checksum()
                elif (single_character=='l'):   self.pi_control        =self.get_data_and_checksum()
                elif (single_character=='x'):   self.pi_max            =self.get_data_and_checksum()

                elif (single_character=='1'):   self.required_VsD            =self.get_data_and_checksum()
                elif (single_character=='2'):   self.required_VsQ            =self.get_data_and_checksum()
                elif (single_character=='3'):   self.required_Vs             =self.get_data_and_checksum()
                #elif (single_character=='4'):   self.data_4            =self.get_data_and_checksum()
                #elif (single_character=='5'):   self.data_5            =self.get_data_and_checksum()
                #elif (single_character=='6'):   self.data_6            =self.get_data_and_checksum()

                elif (single_character=='K'):   self.P_speed            =self.get_data_and_checksum()
                elif (single_character=='I'):   self.I_speed            =self.get_data_and_checksum()
                elif (single_character=='j'):   self.torque_P_speed     =self.get_data_and_checksum()
                elif (single_character=='i'):   self.torque_I_speed     =self.get_data_and_checksum()    

                elif (single_character=='G'):   self.strain_gauge       =self.get_data_and_checksum() 

                elif (single_character=='7'):   self.Ia_peak__short_pulse =self.get_data_and_checksum()
                elif (single_character=='8'):   self.Ib_peak__short_pulse =self.get_data_and_checksum()
                elif (single_character=='9'):   self.Ic_peak__short_pulse =self.get_data_and_checksum()
                elif (single_character=='0'):   self.initial_rotor_angle  =self.get_data_and_checksum()         
                elif (single_character=='_'):   self.absolute_initial_rotor_angle  =self.get_data_and_checksum()
                elif (single_character=='-'):   self.initial_rotor_zone   =self.get_data_and_checksum() 

                #admitance control
                elif (single_character=='f'):   self.stiffness                  =self.get_data_and_checksum()
                elif (single_character=='M'):   self.damping                    =self.get_data_and_checksum()

                elif (single_character=='E'):   
                    self.reference_electric_angle   =self.get_data_and_checksum()
                    self.reference_mechanic_angle   =self.reference_electric_angle/self.pole_pairs
                    self.reference_gear_angle       =self.reference_mechanic_angle/self.gear_ratio

                elif (single_character=='J'):   
                    self.reference_mechanic_angle   =self.get_data_and_checksum()
                    self.reference_gear_angle       =self.reference_mechanic_angle/self.gear_ratio
                    self.reference_electric_angle   =self.reference_mechanic_angle*self.pole_pairs

                elif (single_character=='o'):   
                    self.reference_gear_angle       =self.get_data_and_checksum()
                    self.reference_mechanic_angle   =self.reference_gear_angle*self.gear_ratio
                    self.reference_electric_angle   =self.reference_mechanic_angle*self.pole_pairs

                elif (single_character=='g'):   
                    self.electric_angle             =self.get_data_and_checksum()   
                    self.mechanic_angle             =self.electric_angle/self.pole_pairs
                    self.gear_angle                 =self.mechanic_angle/self.gear_ratio
      
                elif (single_character=='b'):   
                    self.mechanic_angle             =self.get_data_and_checksum()
                    self.electric_angle             =self.mechanic_angle*self.pole_pairs
                    self.gear_angle                 =self.mechanic_angle/self.gear_ratio

                elif (single_character=='a'):   
                    self.gear_angle                 =self.get_data_and_checksum() 
                    self.mechanic_angle             =self.gear_angle*self.gear_ratio
                    self.electric_angle             =self.mechanic_angle*self.pole_pairs

                elif (single_character=='F'):   
                    self.mechanic_frequency         =self.get_data_and_checksum() 
                    self.gear_frequency             =self.mechanic_frequency/self.gear_ratio
                    self.electric_frequency         =self.mechanic_frequency/self.pole_pairs            

                elif (single_character=='H'):   
                    self.gear_frequency             =self.get_data_and_checksum() 
                    self.mechanic_frequency         =self.gear_frequency*self.gear_ratio
                    self.electric_frequency         =self.mechanic_frequency*self.pole_pairs
                    '''
                    w
                    Y
                    z
                    Z
                    '''
                elif (single_character=='S'):  self.halla= self.get_data_and_checksum() 
                elif (single_character=='T'):  self.hallb= self.get_data_and_checksum() 
                elif (single_character=='u'):  self.prev_halla= self.get_data_and_checksum() 
                elif (single_character=='V'):  self.prev_hallb= self.get_data_and_checksum() 


                elif (single_character=='N'):   
                    self.exception  = self.ser.read(bytes)
                    self.exception += self.ser.read(bytes)
                    self.exception += self.ser.read(bytes)
                    self.exception += self.ser.read(bytes)
                    self.exception += self.ser.read(bytes)
                    self.exception += self.ser.read(bytes)
                    self.exception += self.ser.read(bytes)

                    self.exception += self.ser.read(bytes)
                    self.exception += self.ser.read(bytes)
                    self.exception += self.ser.read(bytes)
                    self.exception += self.ser.read(bytes)
                    self.exception += self.ser.read(bytes)
                    self.exception += self.ser.read(bytes)
                    self.exception += self.ser.read(bytes)
                    self.exception += self.ser.read(bytes)

                elif (single_character=='k'):
                    self.checksum_stm32 = ord(self.ser.read(bytes))
                #print "single_character: " + single_character
        
        if (self.checksum_python!=self.checksum_stm32):
            self.transmition_error=True            
 

     
    def append_new_data_to_vectors(self):

        if self.transmition_error==False: 

                #------------calculating values from gotten data----------------------------------------
                #---(they are inside the stm32, but we do not want to waste time sending them)----------        
                #self.psi_s       = math.sqrt(self.psi_sD*self.psi_sD+self.psi_sQ*self. psi_sQ)
                self.psi_s_alpha = self.fast_vector_angle(self.psi_sQ,self.psi_sD)

                if self.print_selection==1:
                    self.isD=self.isA;
                    self.isQ=(self.isA+2.0*self.isB)/math.sqrt(3.0)
                #----------------------------------------------------------------------------------------


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
                self.Vs_relative_cita_vector.append(self.Vs_relative_cita)

                self.required_VsD_vector.append(self.required_VsD)
                self.required_VsQ_vector.append(self.required_VsQ)
                self.required_Vs_vector.append(self.required_Vs)
                self.required_Vs_cita_vector.append(self.required_Vs_cita)

                self.psi_sD_vector.append(self.psi_sD)
                self.psi_sQ_vector.append(self.psi_sQ)
                self.psi_s_vector.append(self.psi_s)
                self.psi_s_alpha_vector.append(self.psi_s_alpha)
                self.psi_s_reference_vector.append(self.psi_s_reference)
                         
                self.te_vector.append(self.te)
                self.te_ref_vector.append(self.te_ref)
                self.Ud_vector.append(self.Ud)
                self.pi_control_vector.append(self.pi_control)
                self.pi_max_vector.append(self.pi_max)
                        
                self.load_angle_vector.append(self.load_angle)

                self.strain_gauge_vector.append(self.strain_gauge)

                self.stiffness_vector.append(self.stiffness)
                self.damping_vector.append(self.damping)
                self.reference_electric_angle_vector.append(self.reference_electric_angle)
                self.reference_mechanic_angle_vector.append(self.reference_mechanic_angle)
                self.reference_gear_angle_vector.append(self.reference_gear_angle)
                self.electric_angle_vector.append(self.electric_angle)
                self.mechanic_angle_vector.append(self.mechanic_angle)
                self.gear_angle_vector.append(self.gear_angle)
                #self.electric_frequency_vector.append(self.electric_frequency)
                self.mechanic_frequency_vector.append(self.mechanic_frequency)
                self.gear_frequency_vector.append(  self.gear_frequency)    
                

    def fast_vector_angle(self,y,x):
        if   (x==0.0 and y==0.0           )  : angle = 0.0                     #division by zero
        elif (x>=0.0 and y>=0.0 and  x>= y)  : angle =       (180.0/math.pi)*math.atan( y/ x)  #1st quadrant
        elif (x>=0.0 and y>=0.0 and  y>  x)  : angle =  90.0-(180.0/math.pi)*math.atan( x/ y)  #2nd quadrant
        elif (x< 0.0 and y>=0.0 and  y>=-x)  : angle =  90.0+(180.0/math.pi)*math.atan(-x/ y)  #3rd quadrant
        elif (x< 0.0 and y>=0.0 and -x>  y)  : angle = 180.0-(180.0/math.pi)*math.atan( y/-x)  #4th quadrant
        elif (x< 0.0 and y< 0.0 and -x>=-y)  : angle = 180.0+(180.0/math.pi)*math.atan(-y/-x)  #5th quadrant
        elif (x< 0.0 and y< 0.0 and -y> -x)  : angle = 270.0-(180.0/math.pi)*math.atan(-x/-y)  #6th quadrant
        elif (x>=0.0 and y< 0.0 and -y>= x)  : angle = 270.0+(180.0/math.pi)*math.atan( x/-y)  #7th quadrant 
        elif (x>=0.0 and y< 0.0 and  x> -y)  : angle = 360.0-(180.0/math.pi)*math.atan(-y/ x)  #8th quadrant 
        else                                 : angle =   0.0                 
          
        if   angle>=360.0: angle=angle-360.0
        elif angle<   0.0: angle=angle+360.0
            
        return angle


    def vector_angle(self,quadrature_component,direct_component):
    	angle=0.0
	    #angle=180.0f/PI*atanf(quadrature_component/direct_component);

        if (direct_component!=0.0):
            angle=57.295779513082320*math.atan(quadrature_component/direct_component)
        else:
            angle=0.0

        if (angle>=360.0):
		    angle=angle-360.0
        elif (angle<0.0):
		    angle=angle+360.0

        if ( (quadrature_component<=0.0) and (direct_component<0.0) ):
		    angle=angle+180.0

        elif ( (quadrature_component>0.0)and(direct_component<0.0)):
		    angle=angle-180.0

        return angle;




    def full_print_string (self):
           #print "inside full_print_string"
           extra_information=  " "+self.test_routine_state + " "+self.driving_test_state+" "+str(self.driving_counter) + \
                            " N: "+self.exception
           self.new_data_line=  "t %6.2f "                  %self.time                      + \
                                    " ref_freq %6.2f"           %self.reference_frequency   + \
                                    " electric_frequency %6.2f" %self.electric_frequency    + \
                                    " hall_freq %6.2f"          %self.hall_frequency        + \
                                    " isA %6.2f"                %self.isA                   + \
                                    " isB %6.2f"                %self.isB                   + \
                                    " isC %6.2f"                %self.isC                   + \
                                    " isD %6.2f"                %self.isD                   + \
                                    " isQ %6.2f"                %self.isQ                   + \
                                    " is  %6.2f"                %math.sqrt(self.isQ*self.isQ+self.isD*self.isD) + \
                                    " is_cita %6.2f"            %self.vector_angle(self.isQ,self.isD)           + \
                                    " VsD %6.2f"                %self.VsD                   + \
                                    " VsQ %6.2f"                %self.VsQ                   + \
                                    " Vs %6.2f"                 %self.Vs                    + \
                                    " Vs_cita %6.2f"            %self.Vs_cita               + \
                                    " Vs_cita_relative %6.2f"   %self.Vs_relative_cita      + \
                                    " psisD %10.8f"             %self.psi_sD                + \
                                    " psisQ %10.8f"             %self.psi_sQ                + \
                                    " psis %6.2f"               %self.psi_s                 + \
                                    " psis_alpha %6.2f"         %self.psi_s_alpha           + \
                                    " psis_ref %10.8f"          %self.psi_s_reference       + \
                                    " te %6.2f"                 %self.te                    + \
                                    " Ud %6.2f"                 %self.Ud                    + \
                                    " pi_control %12.9f"        %self.pi_control            + \
                                    " pi_max %10.6f"            %self.pi_max                + \
                                    " load_angle %12.9f"        %self.load_angle            + \
                                    " strain_gauge %12.9f"      %self.strain_gauge          + \
                                    " psi_rotating_angle %12.9f"%self.data_1                + \
                                    " R_s %12.9f"               %self.data_2                + \
                                    " tick_period %12.9f"       %self.data_3                + \
                                    " data4 %12.9f"             %self.data_4                + \
                                    " data5 %12.9f"             %self.data_5                + \
                                    " data6 %12.9f"             %self.data_6                + extra_information

    def print_selection_print_string(self):

        #extra_information=  " "+self.test_routine_state + " "+self.driving_test_state+" "+str(self.driving_counter) + \
        #                    " "+self.P_speed_state+" "+self.I_speed_state+" "+self.various_test_state+" "+self.torque_test_routine_state+" "+self.torque_P_speed_state#+" N: "+self.exception
        extra_information=  " "+self.test_routine_state + " "+self.driving_test_state+" "+str(self.driving_counter)# + " "+self.P_speed_state+" "+self.I_speed_state+" "+self.various_test_state+"       


        if   self.print_selection==0:
            #self.reference_electric_angle   =float(split_string[split_counter+1])
            m  =self.electric_frequency/self.pole_pairs
            f  =self.electric_frequency/(self.pole_pairs*self.gear_ratio)
            self.new_data_line= "t: %6.2f "                   %self.time                    + \
                                " ref_freq: %6.2f"            %self.reference_frequency     + \
                                " electric_frequency: %10.2f" %self.electric_frequency      + \
                                " hall_freq: %6.2f"           %self.hall_frequency          + \
                                " mechanical_freq: %6.2f"     %m    + \
                                " gear_freq: %10.6f"           %f + extra_information
                                

        elif self.print_selection==1:
            self.isD=self.isA;
            self.isQ=(self.isA+2.0*self.isB)/math.sqrt(3.0)
            self.new_data_line= "t: %6.2f "                  %self.time                 + \
                                " isA: %6.2f"                %self.isA                  + \
                                " isB: %6.2f"                %self.isB                  + \
                                " isC: %6.2f"                %self.isC                  + \
                                " isD: %6.2f"                %(self.isD)                + \
                                " isQ: %6.2f"                %(self.isQ)                + \
                                " is:  %6.2f"                %math.sqrt(self.isQ*self.isQ+self.isD*self.isD) + \
                                " is_cita: %6.2f"            %self.vector_angle(self.isQ,self.isD)     + extra_information  

        elif self.print_selection==2:
            self.new_data_line= "t: %6.2f "                   %self.time                + \
                                " isD: %6.2f"                %self.isD                  + \
                                " isQ: %6.2f"                %self.isQ                  + \
                                " is:  %6.2f"                %math.sqrt(self.isQ*self.isQ+self.isD*self.isD) + \
                                " is_cita: %6.2f"            %self.vector_angle(self.isQ,self.isD)     + extra_information       

        elif self.print_selection==3:
            self.new_data_line= "t: %6.2f "                  %self.time                + \
                                " VsD: %6.2f"                %self.VsD                 + \
                                " VsQ: %6.2f"                %self.VsQ                 + \
                                " Vs:  %6.2f"                %math.sqrt(self.VsQ*self.VsQ+self.VsD*self.VsD) + \
                                " Vs_cita: %6.2f"            %self.vector_angle(self.VsQ,self.VsD)     + extra_information

        elif self.print_selection==4:
            self.new_data_line= "t: %6.2f "                  %self.time                + \
                                " req_Vs: %6.2f"             %self.required_Vs         + \
                                " Vs: %6.2f"                 %self.Vs                  + \
                                " Ud: %6.2f"                 %self.Ud                  + extra_information
                                #" Vs_cita: %6.2f"            %self.Vs_cita             + \
                                #" Vs_cita_relative: %6.2f"   %self.Vs_relative_cita      

        elif self.print_selection==5:
            self.new_data_line= "t: %6.2f "                  %self.time                + \
                                " psisD: %10.6f"             %self.psi_sD              + \
                                " psisQ: %10.6f"             %self.psi_sQ              + \
                                " psis:  %10.6f"             %math.sqrt(self.psi_sD*self.psi_sD+self.psi_sQ*self.psi_sQ) + \
                                " psis_angle %10.6f"         %self.fast_vector_angle(self.psi_sQ,self.psi_sD) + extra_information




        elif self.print_selection==6:
            self.new_data_line= "t: %6.2f "                  %self.time                + \
                                " te: %10.6f"                %self.te                  + \
                                " te_ref: %10.6f"            %self.te_ref              + extra_information  

                                
        elif self.print_selection==7:
            self.new_data_line= "t: %6.2f "                  %self.time                + \
                                " angle acceleration (pi_control): %12.8f"        %self.pi_control          + \
                                " total angle advance (2pi*f+pi_control): %12.8f"            %self.pi_max              + \
                                " hall_freq: %6.2f"          %self.hall_frequency      + extra_information

        elif self.print_selection==8:
            self.new_data_line= " P: %12.8f:"               %self.P_speed              + \
                                " I: %12.8f:"               %self.I_speed              + extra_information

        elif self.print_selection==10:
            self.new_data_line= " P: %12.8f:"               %self.torque_P_speed              + \
                                " I: %12.8f:"               %self.torque_I_speed              + extra_information
        
        elif self.print_selection==13:
            self.new_data_line= " t: %12.8f:"               %self.time              + \
                                " strain_gauge: %12.8f:"    %self.strain_gauge                + extra_information
        
        elif self.print_selection==14:
            self.new_data_line= "t: %6.2f "   %self.time                                                                          + \
                                " VsD: %6.2f" %self.required_VsD                                                                  + \
                                " VsQ: %6.2f" %self.required_VsQ                                                                  + \
                                " Vs:  %6.2f" %math.sqrt(self.required_VsQ*self.required_VsQ+self.required_VsD*self.required_VsD) + \
                                " Vs_cita: %6.2f"            %self.vector_angle(self.required_VsQ,self.required_VsD)     + extra_information


        elif self.print_selection==15:
            m=self.electric_frequency/self.pole_pairs
            g=self.electric_frequency/(self.pole_pairs*self.gear_ratio)


            self.new_data_line= "t: %6.2f "       %self.time                                                + \
                                " K: %10.6f"       %self.stiffness                                          + \
                                " D: %10.6f"       %self.damping                                            + \
                                " rg: %6.2f"     %self.reference_gear_angle                                 + \
                                " g: %12.7f"       %self.gear_angle                                         + \
                                " rf: %9.4f"%self.reference_frequency                                       + \
                                " ef: %6.2f"     %self.electric_frequency                                   + \
                                " hf: %9.4f"           %self.hall_frequency                                 + \
                                " sg: %12.8f:"    %self.strain_gauge                                        + \
                                " mf: %6.2f"     %m                   + \
                                " gf: %10.6f"     %g + extra_information
                                

            '''
            self.new_data_line= "t: %6.2f "       %self.time                        + \
                                " K: %6.2f"       %self.stiffness                   + \
                                " D: %6.2f"       %self.damping                     + \
                                " r_e: %12.2f"    %self.reference_electric_angle    + \
                                " r_m: %12.2f"    %self.reference_mechanic_angle    + \
                                " r_g: %6.2f"     %self.reference_gear_angle        + \
                                " e: %6.2f"       %self.electric_angle              + \
                                " m: %6.2f"       %self.mechanic_angle              + \
                                " g: %6.2f"       %self.gear_angle                  + \
                                " e_f: %6.2f"     %self.electric_frequency          + \
                                " m_f: %6.2f"     %self.mechanic_frequency          + \
                                " g_f: %10.6f"     %self.gear_frequency              + extra_information
            '''



        elif self.print_selection==16:
            
            if self.halla>0     : self.halla=1.0
            else                : self.halla=0.0
            if self.hallb>0     : self.hallb=1.0
            else                : self.hallb=0.0
            if self.prev_halla>0: self.prev_halla=1.0
            else                : self.prev_halla=0.0
            if self.prev_hallb>0: self.prev_hallb=1.0
            else                : self.prev_hallb=0.0
            
            self.new_data_line= "t: %6.2f "          %self.time            + \
                                " hall_f: %6.2f"     %self.hall_frequency  + \
                                " prev_HA: %6.2f"    %self.prev_halla      + \
                                " HA: %6.2f"         %self.halla           + \
                                " prev_HB: %6.2f"    %self.prev_hallb      + \
                                " HB: %6.2f"         %self.hallb           + extra_information





        elif self.print_selection==11:
                self.new_data_line= "t %6.2f "                  %self.time                  + \
                                    " ref_freq %6.2f"           %self.reference_frequency   + \
                                    " electric_frequency %6.2f" %self.electric_frequency    + \
                                    " hall_freq %10.8f"         %self.hall_frequency        + \
                                    " isA %6.2f"                %self.isA                   + \
                                    " isB %6.2f"                %self.isB                   + \
                                    " isC %6.2f"                %self.isC                   + \
                                    " isD %6.2f"                %self.isD                   + \
                                    " isQ %6.2f"                %self.isQ                   + \
                                    " is  %6.2f"                %math.sqrt(self.isQ*self.isQ+self.isD*self.isD) + \
                                    " is_cita: %6.2f"           %self.vector_angle(self.isQ,self.isD)           + \
                                    " VsD %6.2f"                %self.VsD                   + \
                                    " VsQ %6.2f"                %self.VsQ                   + \
                                    " Vs %6.2f"                 %self.Vs                    + \
                                    " Vs_cita %6.2f"            %self.Vs_cita               + \
                                    " Vs_cita_relative %6.2f"   %self.Vs_relative_cita      + \
                                    " psisD %10.8f"             %self.psi_sD                + \
                                    " psisQ %10.8f"             %self.psi_sQ                + \
                                    " psis %10.8f"              %self.psi_s                 + \
                                    " psi_s_alpha %6.2f"        %self.psi_s_alpha           + \
                                    " psis_ref %10.8f"          %self.psi_s_reference       + \
                                    " te %6.2f"                 %self.te                    + \
                                    " Ud %6.2f"                 %self.Ud                    + \
                                    " pi_control %12.9f"        %self.pi_control            + \
                                    " pi_max %10.6f"            %self.pi_max                + \
                                    " load_angle %12.9f"        %self.load_angle            
                                    #"t: %6.2f "                  %self.time                + \
                                    #"frequency %6.2f"                 %self.electric_frequency
                                    #" psis: %10.6f"              %self.psi_s               + \
                                    #" psis_alpha: %6.2f"         %self.psi_s_alpha         + \
                                    #" psis_ref: %10.8f"          %self.psi_s_reference
        


        '''
        elif self.print_selection==11:
                self.new_data_line= "t %6.2f "                  %self.time                      + \
                                    " ref_freq %6.2f"           %self.reference_frequency   + \
                                    " electric_frequency %6.2f" %self.electric_frequency    + \
                                    " hall_freq %6.2f"          %self.hall_frequency        + \
                                    " isA %6.2f"                %self.isA                   + \
                                    " isB %6.2f"                %self.isB                   + \
                                    " isC %6.2f"                %self.isC                   + \
                                    " isD %6.2f"                %self.isD                   + \
                                    " isQ %6.2f"                %self.isQ                   + \
                                    " is:  %6.2f"               %math.sqrt(self.isQ*self.isQ+self.isD*self.isD) + \
                                    " is_cita: %6.2f"           %self.vector_angle(self.isQ,self.isD)           + \
                                    " VsD %6.2f"                %self.VsD                   + \
                                    " VsQ %6.2f"                %self.VsQ                   + \
                                    " Vs %6.2f"                 %self.Vs                    + \
                                    " Vs_cita %6.2f"            %self.Vs_cita               + \
                                    " Vs_cita_relative %6.2f"   %self.Vs_relative_cita      + \
                                    " psisD %10.8f"             %self.psi_sD               + \
                                    " psisQ %10.8f"             %self.psi_sQ               + \
                                    " psis %6.2f"               %self.psi_s                 + \
                                    " psis_alpha %6.2f"         %self.psi_s_alpha           + \
                                    " psis_ref %10.8f"          %self.psi_s_reference      + \
                                    " te %6.2f"                 %self.te                    + \
                                    " Ud %6.2f"                 %self.Ud                    + \
                                    " pi_control %12.9f"        %self.pi_control           + \
                                    " pi_max %10.6f"            %self.pi_max               + \
                                    " load_angle %12.9f"        %self.load_angle            
                                    #"t: %6.2f "                  %self.time                + \
                                    #"frequency %6.2f"                 %self.electric_frequency
                                    #" psis: %10.6f"              %self.psi_s               + \
                                    #" psis_alpha: %6.2f"         %self.psi_s_alpha         + \
                                    #" psis_ref: %10.8f"          %self.psi_s_reference
        
        elif self.print_selection==12:
            self.new_data_line= " Ias: %10.4f:"       %self.Ia_peak__short_pulse  + \
                                " Ibs: %10.4f:"       %self.Ib_peak__short_pulse  + \
                                " Ics: %10.4f:"       %self.Ic_peak__short_pulse  + \
                                " zone: %10.4f:"  %self.initial_rotor_zone  + \
                                " angle: %10.4f:" %self.initial_rotor_angle  + \
                                " ab_angle: %10.4f:" %self.absolute_initial_rotor_angle   + extra_information
        '''       
        

    def save_data_to_csv_file(self):
        '''
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
                            " psisD %10.6f"              %self.psi_sD               + \
                            " psisQ %10.6f"              %self.psi_sQ               + \
                            " psis %6.2f"               %self.psi_s                 + \
                            " psis_alpha %6.2f"         %self.psi_s_alpha           + \
                            " psis_ref %10.6f"           %self.psi_s_reference      + \
                            " te %10.6f"                 %self.te                   + \
                            " Ud %6.2f"                 %self.Ud                    + \
                            " pi_control %6.2f"         %self.pi_control            + \
                            " pi_max %6.2f"             %self.pi_max  
        '''

        if self.print_selection!=9  :  
            self.print_selection_print_string()
            #print "just appending some data"
        elif self.print_selection==9:                       
            #print "apending everything to a file"
            self.full_print_string ()

        #if self.transmition_error==False:      
        #    print   self.new_data_line


        if self.transmition_error==False: 
            split_new_data_line = self.new_data_line.split()
            self.writer.writerow(split_new_data_line)     


    


    def print_to_console(self):
        #print "print_selection: " + str(self.print_selection)
        if self.print_selection!=9  :  self.print_selection_print_string()
        elif self.print_selection==9:  self.full_print_string ()
        #print "check_sum python: "+str(self.checksum_python)+" stm32: "+str(self.checksum_stm32)

        if self.transmition_error==False:      
          print   self.new_data_line
        '''
        else :
            print   self.new_data_line + " transmition_error"
            #print "warning: transmition_error"
        '''




    #adding a '*' or 'o'after the two vectors allows to plot points instead of a continuos line 





    def plot_electric_angles(self,rows,columns,subplot_index):

        plt.subplot(rows,columns,subplot_index)
        plt.plot(self.time_vector,self.electric_angle_vector ,self.plotting_character,label='electric angle')
        plt.plot(self.time_vector,self.reference_electric_angle_vector,self.plotting_character,label='reference angle')                     
        plt.title('Electric angle vs time'+self.title_extra)
        plt.xlabel('time (ticks)')
        plt.ylabel('electric angle (degrees)')
        plt.legend()

    def plot_mechanical_angles(self,rows,columns,subplot_index):

        plt.subplot(rows,columns,subplot_index)
        plt.plot(self.time_vector,self.mechanic_angle_vector ,self.plotting_character,label='mechanic angle')
        plt.plot(self.time_vector,self.reference_mechanic_angle_vector,self.plotting_character,label='reference angle')                     
        plt.title('Mechanic angle vs time'+self.title_extra)
        plt.xlabel('time (ticks)')
        plt.ylabel('mechanic angle (degrees)')
        plt.legend()

    def plot_gear_angles(self,rows,columns,subplot_index):

        plt.subplot(rows,columns,subplot_index)
        plt.plot(self.time_vector,self.gear_angle_vector ,self.plotting_character,label='gear angle')
        plt.plot(self.time_vector,self.reference_gear_angle_vector,self.plotting_character,label='reference angle')                     
        plt.title('Gear angle vs time'+self.title_extra)
        plt.xlabel('time (ticks)')
        plt.ylabel('gear angle (degrees)')
        plt.legend()



    def plot_frequencies(self,rows,columns,subplot_index):

        plt.subplot(rows,columns,subplot_index)
        plt.plot(self.time_vector,self.electric_frequency_vector ,self.plotting_character,label='electric')
        plt.plot(self.time_vector,self.hall_frequency_vector     ,self.plotting_character,label='hall'     )
        plt.plot(self.time_vector,self.reference_frequency_vector,self.plotting_character,label='reference')                     
        plt.title('frequency vs time'+self.title_extra)
        plt.xlabel('time (ticks)')
        plt.ylabel('frequency (Hz)')
        plt.legend()

    def plot_gear_angle_and_frequency(self,rows,columns,subplot_index): 

        plt.subplot(rows,columns,subplot_index)
        plt.plot(self.time_vector, self.gear_angle_vector,self.plotting_character,label='gear angle',color='m')
        plt.plot(self.time_vector, self.reference_gear_angle_vector,self.plotting_character,label='reference angle',color='y')
        plt.ylabel('gear angle (degrees)')
        plt.twinx() # to activate a second axis
        plt.plot(self.time_vector,self.electric_frequency_vector ,self.plotting_character,label='electric frequency',color='r')
        plt.plot(self.time_vector,self.hall_frequency_vector     ,self.plotting_character,label='hall' ,color='g'    )
        plt.plot(self.time_vector,self.reference_frequency_vector,self.plotting_character,label='reference',color='b')  
        plt.title('frequency and gear angle vs time'+self.title_extra)
        plt.xlabel('time (ticks)')
        plt.ylabel('frequency(Hz)')
        plt.legend()



    def plot_strain_gauge_and_gear_angle(self,rows,columns,subplot_index): 

        plt.subplot(rows,columns,subplot_index)
        plt.plot(self.time_vector, self.gear_angle_vector,self.plotting_character,label='gear angle',color='m')
        plt.plot(self.time_vector, self.reference_gear_angle_vector,self.plotting_character,label='reference angle',color='y')
        plt.ylabel('gear angle (degrees)')
        plt.twinx() # to activate a second axis
        plt.plot(self.time_vector, self.strain_gauge_vector,self.plotting_character,label='strain_gauge')
        plt.plot(self.time_vector, self.te_ref_vector,self.plotting_character,label='te_ref')
        plt.title('strain gauge and gear angle vs time'+self.title_extra)
        plt.xlabel('time (ticks)')
        plt.ylabel('strain gauge(Nm)')
        plt.legend()

    def plot_strain_gauge_and_electric_frequency(self,rows,columns,subplot_index): 

        plt.subplot(rows,columns,subplot_index)
        plt.plot(self.time_vector, self.strain_gauge_vector,self.plotting_character,label='strain_gauge',color='m')
        plt.ylabel('strain gauge (Nm)')
        plt.twinx() # to activate a second axis
        plt.plot(self.time_vector,self.electric_frequency_vector ,self.plotting_character,label='electric frequency',color='r')
        plt.plot(self.time_vector,self.hall_frequency_vector     ,self.plotting_character,label='hall' ,color='g'    )
        plt.plot(self.time_vector,self.reference_frequency_vector,self.plotting_character,label='reference',color='b')  
        plt.title('electric frequency and strain gauge vs time'+self.title_extra)
        plt.xlabel('time (ticks)')
        plt.ylabel('frequency(Hz)')
        plt.legend()


    def plot_three_phase_currents(self,rows,columns,subplot_index):

                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector, self.isA_vector,self.plotting_character,label='isA')
                        plt.plot(self.time_vector, self.isB_vector,self.plotting_character,label='isB')
                        plt.plot(self.time_vector, self.isC_vector,self.plotting_character,label='isC')
                        plt.title('current vs time'+self.title_extra)
                        plt.xlabel('time (ticks)')
                        plt.ylabel('three-phase currents (A)')
                        plt.legend()

    def plot_three_phase_currents_A(self,rows,columns,subplot_index):

                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector, self.isA_vector,self.plotting_character,label='isA')
                        #plt.plot(self.time_vector, self.isB_vector,self.plotting_character,label='isB')
                        #plt.plot(self.time_vector, self.isC_vector,self.plotting_character,label='isC')
                        plt.title('current vs time'+self.title_extra)
                        plt.xlabel('time (ticks)')
                        plt.ylabel('Phase A current  (A)')
                        plt.legend()

    def plot_three_phase_currents_B(self,rows,columns,subplot_index):

                        plt.subplot(rows,columns,subplot_index)
                        #plt.plot(self.time_vector, self.isA_vector,self.plotting_character,label='isA')
                        plt.plot(self.time_vector, self.isB_vector,self.plotting_character,label='isB')
                        #plt.plot(self.time_vector, self.isC_vector,self.plotting_character,label='isC')
                        plt.title('current vs time'+self.title_extra)
                        plt.xlabel('time (ticks)')
                        plt.ylabel('Phase B current (A)')
                        plt.legend()

    def plot_three_phase_currents_C(self,rows,columns,subplot_index):

                        plt.subplot(rows,columns,subplot_index)
                        #plt.plot(self.time_vector, self.isA_vector,self.plotting_character,label='isA')
                        #plt.plot(self.time_vector, self.isB_vector,self.plotting_character,label='isB')
                        plt.plot(self.time_vector, self.isC_vector,self.plotting_character,label='isC')
                        plt.title('current vs time'+self.title_extra)
                        plt.xlabel('time (ticks)')
                        plt.ylabel('Phase C current (A)')
                        plt.legend()



    def plot_quadrature_vs_direct_currents(self,rows,columns,subplot_index):

                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.isD_vector, self.isQ_vector,self.plotting_character_0)
                        plt.title ('isQ vs isD'+self.title_extra)
                        plt.xlabel('isD (A)')
                        plt.ylabel('isQ (A)')
                        plt.legend() 

    def plot_quadrature_vs_direct_voltages(self,rows,columns,subplot_index):

                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.VsD_vector, self.VsQ_vector,self.plotting_character_0)
                        plt.title('VsQ vs VsD'+self.title_extra)
                        plt.xlabel('VsD (V)')
                        plt.ylabel('VsQ (V)')
                        plt.legend() 

    def plot_required_quadrature_vs_direct_voltages(self,rows,columns,subplot_index):

                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.required_VsD_vector,self.required_VsQ_vector,self.plotting_character_0)
                        plt.title('required VsQ vs requried VsD'+self.title_extra)
                        plt.xlabel('required_VsD (V)')
                        plt.ylabel('required_VsQ (V)')
                        plt.legend() 

    def plot_voltage_magnitude(self,rows,columns,subplot_index):  
  
                        plt.subplot(rows,columns,subplot_index)
                        #plt.plot(self.time_vector, self.required_Vs_vector,self.plotting_character,label='required_Vs')
                        plt.plot(self.time_vector, self.Vs_vector,self.plotting_character,label='Vs')
                        plt.plot(self.time_vector, self.Ud_vector,self.plotting_character,label='Ud')
                        plt.title('voltage vs time'+self.title_extra)
                        plt.xlabel('t (ticks)')
                        plt.ylabel('Voltage (V)')
                        plt.legend()     

    def plot_flux_linkage(self,rows,columns,subplot_index): 

                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.psi_sD_vector, self.psi_sQ_vector,self.plotting_character_0)
                        plt.title('psi_sQ vs psi_sD'+self.title_extra)
                        plt.xlabel('psi_sD (Wb)')
                        plt.ylabel('psi_sQ (Wb)')
                        plt.legend() 


    def plot_flux_linkage_angle(self,rows,columns,subplot_index):  

                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector, self.psi_s_alpha_vector,self.plotting_character_0)
                        plt.title('psi_s_alpha vs time'+self.title_extra)
                        plt.xlabel('time (degrees)')
                        plt.ylabel('psi_s_alpha (Wb)')
                        plt.legend() 



    def plot_electromagnetic_torque(self,rows,columns,subplot_index): 

                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector, self.te_vector,self.plotting_character,label='te')
                        #plt.plot(self.time_vector, self.te_vector,'o',label='te')
                        plt.plot(self.time_vector, self.te_ref_vector,self.plotting_character,label='te_ref')
                        plt.title('torque vs time'+self.title_extra)
                        plt.xlabel('time (ticks)')
                        plt.ylabel('torque (Nm)')
                        plt.legend() 

    def plot_strain_gauge_torque(self,rows,columns,subplot_index): 

                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector, self.strain_gauge_vector,self.plotting_character,label='sg')
                        #plt.plot(self.time_vector, self.te_vector,'o',label='te')
                        plt.plot(self.time_vector, self.te_ref_vector,self.plotting_character,label='te_ref')
                        plt.title('torque vs time (strain gauge)'+self.title_extra)
                        plt.xlabel('time (ticks)')
                        plt.ylabel('torque (Nm)')
                        plt.legend() 

    def plot_phase_advance(self,rows,columns,subplot_index): 
   
                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector, self.pi_control_vector,self.plotting_character,label='pi_control')
                        #plt.plot(self.time_vector, self.pi_max_vector,self.plotting_character,label='pi_max')
                        plt.title('pi increment vs time'+self.title_extra)
                        plt.xlabel('time (ticks)')
                        plt.ylabel('pi (degrees)')
                        plt.legend()


    def plot_load_angle_and_frequency(self,rows,columns,subplot_index): 

                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.time_vector, self.load_angle_vector,self.plotting_character,label='load angle')
                        plt.ylabel('load angle (degrees)')
                        plt.twinx() # to activate a second axis
                        plt.plot(self.time_vector,self.electric_frequency_vector ,self.plotting_character,label='electric frequency',color='r')
                        plt.title('frequency vs load angle'+self.title_extra)
                        plt.xlabel('time (ticks)')
                        plt.ylabel('frequency(Hz)')
                        plt.legend()

    def plot_load_angle_vs_frequency(self,rows,columns,subplot_index):    

                        plt.subplot(rows,columns,subplot_index)
                        plt.plot(self.load_angle_vector,self.electric_frequency_vector ,self.plotting_character,label='')
                        plt.title('frequency vs load angle'+self.title_extra)
                        plt.xlabel('load angle (degrees)')
                        plt.ylabel('frequency (Hz)')
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
                        plt.savefig(self.path+"[" +datetime.datetime.now().ctime() +"] ""all_graphs"+".jpg")
                        plt.close()

    def plot_one_by_one(self):
                        rows = 1
                        columns = 1 
                        subplot_index = 1
                        plot_figsize=(10, 10)
                        plot_dpi=300
                        plot_face_color='w'
                        plot_edge_color='k'
                        plot_name=self.path
                      
                        plt.figure(num=2, figsize=(10, 10), dpi=300, facecolor='w', edgecolor='k') 
                        self.plot_frequencies                   (rows,columns,subplot_index)
                        plt.savefig(self.path+datetime.datetime.now().ctime()+"."+self.tag_comment+"._frequencies" +".jpg")
                        plt.close()

                        plt.figure(num=3, figsize=(10, 10), dpi=300, facecolor='w', edgecolor='k') 
                        self.plot_three_phase_currents          (rows,columns,subplot_index)
                        plt.savefig(self.path+ "three-phase_currents" +"." + datetime.datetime.now().ctime() +self.tag_comment+".jpg")
                        plt.close()

                        plt.figure(num=4, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color) 
                        self.plot_three_phase_currents_A          (rows,columns,subplot_index)
                        plt.savefig(plot_name+"three-phase_currents_A"+".jpg")
                        plt.close()

                        plt.figure(num=5, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color) 
                        self.plot_three_phase_currents_B          (rows,columns,subplot_index)
                        plt.savefig(plot_name+"three-phase_currents_B"+".jpg")
                        plt.close()

                        plt.figure(num=6, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color) 
                        self.plot_three_phase_currents_C          (rows,columns,subplot_index)
                        plt.savefig(plot_name+"three-phase_currents_C"+".jpg")
                        plt.close()


                        plt.figure(num=7, figsize=(10, 10), dpi=300, facecolor='w', edgecolor='k') 
                        self.plot_quadrature_vs_direct_currents (rows,columns,subplot_index)
                        plt.savefig(self.path+ "isQ_vs_isD" +"." + datetime.datetime.now().ctime() +self.tag_comment+".jpg")
                        plt.close()

                        plt.figure(num=8, figsize=(10, 10), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_quadrature_vs_direct_voltages (rows,columns,subplot_index)
                        plt.savefig(self.path+ "VsQ_vs_VsD" +"." + datetime.datetime.now().ctime() +self.tag_comment+".jpg")
                        plt.close()

                        plt.figure(num=9, figsize=(10, 10), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_voltage_magnitude             (rows,columns,subplot_index)
                        plt.savefig(self.path+ "voltage_magnitude" +"." + datetime.datetime.now().ctime() +self.tag_comment+".jpg")
                        plt.close()
               
                        plt.figure(num=10, figsize=(10, 10), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_flux_linkage                       (rows,columns,subplot_index)
                        plt.savefig(self.path+ "flux-linkage" +"." + datetime.datetime.now().ctime() +self.tag_comment+".jpg")
                        plt.close()

                        plt.figure(num=11, figsize=(10, 10), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_electromagnetic_torque             (rows,columns,subplot_index)
                        plt.savefig(self.path+ "electromagnetic_torque" +"." + datetime.datetime.now().ctime() +self.tag_comment+".jpg")
                        plt.close()


                        plt.figure(num=12, figsize=(10, 10), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_phase_advance                      (rows,columns,subplot_index)
                        plt.savefig(self.path+ "phase_advance_pi" +"." + datetime.datetime.now().ctime() +self.tag_comment+".jpg")
                        plt.close()

                        plt.figure(num=13, figsize=(10, 10), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_load_angle_vs_frequency            (rows,columns,subplot_index)
                        plt.savefig(self.path+ "frequency vs load angle" +"." + datetime.datetime.now().ctime() +self.tag_comment+".jpg")
                        plt.close()

                        plt.figure(num=14, figsize=(10, 10), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_load_angle_and_frequency           (rows,columns,subplot_index)
                        plt.savefig(self.path+ "frequency and load angle vs time" +"." + datetime.datetime.now().ctime() +self.tag_comment+".jpg")
                        plt.close()


                        plt.figure(num=15, figsize=(10, 10), dpi=300, facecolor='w', edgecolor='k')
                        self.plot_strain_gauge_torque             (rows,columns,subplot_index)
                        plt.savefig(self.path+ "electromagnetic_torque_strain_gauge" +"." + datetime.datetime.now().ctime() +self.tag_comment+".jpg")
                        plt.close()

    
    def plot_selection(self):
        rows = 1
        columns = 1 
        subplot_index = 1
                     
        plot_name = self.path+ "[" +datetime.datetime.now().ctime() +"] ["+self.tag_comment+"] "
        plot_figsize=(10,10)
        plot_dpi=300
        plot_face_color='w'
        plot_edge_color='k'  
        

        if   self.print_selection==0:
           if len(self.time_vector)==0 or len(self.electric_frequency_vector)==0 or len(self.hall_frequency_vector)==0: #or len(self.reference_frequency_vector)==0:
                self.empty_vectors=True;
            
           else:
                plt.figure(num=2, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color) 
                self.plot_frequencies                   (rows,columns,subplot_index)
                plt.savefig( plot_name+"frequencies" +self.title_extra+".jpg")
                plt.close()

        elif self.print_selection==1:
            if len(self.time_vector)==0 or len(self.isA_vector)==0 or len(self.isB_vector)==0 or len(self.isC_vector)==0:
                self.empty_vectors=True

            else:
                plt.figure(num=3, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color) 
                self.plot_three_phase_currents          (rows,columns,subplot_index)
                plt.savefig(plot_name+"three-phase_currents"+".jpg")
                plt.close()

                plt.figure(num=4, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color) 
                self.plot_three_phase_currents_A          (rows,columns,subplot_index)
                plt.savefig(plot_name+"three-phase_currents_A"+".jpg")
                plt.close()

                plt.figure(num=5, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color) 
                self.plot_three_phase_currents_B          (rows,columns,subplot_index)
                plt.savefig(plot_name+"three-phase_currents_B"+".jpg")
                plt.close()

                plt.figure(num=6, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color) 
                self.plot_three_phase_currents_C          (rows,columns,subplot_index)
                plt.savefig(plot_name+"three-phase_currents_C"+".jpg")
                plt.close()


                plt.figure(num=7, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color) 
                self.plot_quadrature_vs_direct_currents (rows,columns,subplot_index)
                plt.savefig(plot_name+"isQ_vs_isD"+".jpg")
                plt.close()

        elif self.print_selection==2:
            if len(self.isD_vector)==0 or len(self.isQ_vector)==0:
                self.empty_vectors=True

            else:
                plt.figure(num=8, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color) 
                self.plot_quadrature_vs_direct_currents (rows,columns,subplot_index)
                plt.savefig(plot_name+"isQ_vs_isD"+".jpg")
                plt.close()

        elif self.print_selection==3:
            if  len(self.VsD_vector)==0 or len(self.VsQ_vector)==0 :
                self.empty_vectors=True

            else:
                plt.figure(num=9, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color)
                self.plot_quadrature_vs_direct_voltages (rows,columns,subplot_index)
                plt.savefig(plot_name+"VsQ_vs_VsD"+".jpg")
                plt.close()

        elif self.print_selection==4:
            if len(self.time_vector)==0 or len(self.Vs_vector)==0 or len(self.Ud_vector)==0 :
                self.empty_vectors=True

            else:

                plt.figure(num=10, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color)
                self.plot_voltage_magnitude             (rows,columns,subplot_index)
                plt.savefig(plot_name+"voltage_magnitude"+".jpg")
                plt.close()

        elif self.print_selection==5:
            if len(self.psi_sD_vector)==0 or len(self.psi_sQ_vector)==0: #or len(self.time_vector)==0 or len(self.psi_s_alpha_vector)==0:
                self.empty_vectors=True

            else: 
                plt.figure(num=11, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color)
                self.plot_flux_linkage                       (rows,columns,subplot_index)
                plt.savefig(plot_name+"flux-linkage"+".jpg")
                plt.close()

                plt.figure(num=12, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color)
                self.plot_flux_linkage_angle                 (rows,columns,subplot_index)
                plt.savefig(plot_name+"flux-linkage angle vs time"+".jpg")
                plt.close()

        elif self.print_selection==6:
            if len(self.time_vector)==0 or len(self.te_vector)==0: #or  len(self.te_ref_vector)==0:
                self.empty_vectors=True

            else: 
                plt.figure(num=13, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color)
                self.plot_electromagnetic_torque             (rows,columns,subplot_index)
                plt.savefig(plot_name+"electromagnetic_torque" +".jpg")
                plt.close()

        elif self.print_selection==7:
            if len(self.time_vector)==0 or len(self.pi_control_vector)==0 or  len(self.pi_max_vector)==0:
                self.empty_vectors=True

            else: 
                plt.figure(num=14, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color)
                self.plot_phase_advance                      (rows,columns,subplot_index)
                plt.savefig(plot_name+"phase_advance_pi"+".jpg")
                plt.close()  

        elif self.print_selection==11:
            self.plot_all_in_one()
            self.plot_one_by_one()  

        elif self.print_selection==13:
            if len(self.time_vector)==0 or len(self.strain_gauge_vector)==0: #or  len(self.te_ref_vector)==0:
                self.empty_vectors=True

            else: 
                plt.figure(num=13, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color)
                self.plot_strain_gauge_torque             (rows,columns,subplot_index)
                plt.savefig(plot_name+"electromagnetic_torque" +".jpg")
                plt.close()

        elif self.print_selection==14:
            if  len(self.required_VsD_vector)==0 or len(self.required_VsQ_vector)==0 :
                self.empty_vectors=True

            else:
                plt.figure(num=9, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color)
                self.plot_required_quadrature_vs_direct_voltages (rows,columns,subplot_index)
                plt.savefig(plot_name+"required_VsQ_vs_required_VsD"+".jpg")
                plt.close()

        if   self.print_selection==15:
           if len(self.time_vector)==0 or len(self.electric_frequency_vector)==0 or len(self.hall_frequency_vector)==0: #or len(self.reference_frequency_vector)==0:
                self.empty_vectors=True;
            
           else:
                plt.figure(num=16, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color) 
                self.plot_frequencies                   (rows,columns,subplot_index)
                plt.savefig( plot_name+"frequencies" +self.title_extra+".jpg")
                plt.close()

                plt.figure(num=17, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color) 
                self.plot_gear_angle_and_frequency(rows,columns,subplot_index)
                plt.savefig( plot_name+"gear angle and frequencies vs time" +self.title_extra+".jpg")
                plt.close()

                plt.figure(num=18, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color) 
                self.plot_gear_angles(rows,columns,subplot_index)
                plt.savefig( plot_name+"gear angle vs time" +self.title_extra+".jpg")
                plt.close()

                plt.figure(num=19, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color)
                self.plot_strain_gauge_torque             (rows,columns,subplot_index)
                plt.savefig(plot_name+"strain_gauge_torque" +".jpg")
                plt.close()

                plt.figure(num=20, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color)
                self.plot_strain_gauge_and_electric_frequency             (rows,columns,subplot_index)
                plt.savefig(plot_name+"strain_gauge_and_frequency" +".jpg")
                plt.close()

                plt.figure(num=21, figsize=plot_figsize, dpi=plot_dpi, facecolor=plot_face_color, edgecolor=plot_edge_color)
                self.plot_strain_gauge_and_gear_angle             (rows,columns,subplot_index)
                plt.savefig(plot_name+"strain_gauge_and_gear_angle" +".jpg")
                plt.close()






    def __init__(self):
        self.initializing_values()        
        self.creating_data_vectors()
        #self.create_log_file()
        self.connecting_to_stm32F4()
               


            

    def __del__(self):
        self.log_file.close()
        self.ser.close()
        print "---Serial port closed: disconnected from the STM32F4---"
	

        	    	    		
		
    def read(self): 
            if self.read_strings==True:
                self.read_data_from_stm32_strings()
            else:                
                self.read_data_from_stm32()
                                        
            if self.capture_data==True and self.transmition_error==False:
                
                if   self.read_capture_state == 'not_collecting' and (self.time <= self.min_test_time or self.force_append ==True):#  30 cycles for the regularbyte sending
                                                                                      # and 300 for the whole data 
                    self.read_capture_state = 'collecting'
                    #print "not appending, timer: " + str(self.time)

                elif self.read_capture_state == 'collecting':
                    self.append_new_data_to_vectors()
                    self.save_data_to_csv_file()
                    #print "apending data to vectors"

            self.print_to_console() 
            '''
            if self.transmition_error==True:
                print "transmition error"
            else:
                print "transmition ok"
            '''

    def write_a_line(self,line):
        #self.ser.write('\n')
        #self.ser.write('\r') 
        self.ser.write(line)
        self.ser.write('\n')
        self.ser.write('\r')      

    def break_the_motor(self):
        line = 'd 0' 
        self.write_a_line(line)

    def break_the_motor_torque(self):
        line = 'Q 0' 
        self.write_a_line(line)

    def change_frequency (self, frequency):
        #line='d '+ frequency
        line=self.test_command+' '+frequency
        #print "change_frequency :::: new frequency to test: " + frequency
        #print line
        self.write_a_line(line)

    def change_torque (self, torque):
        line='Q '+ torque
        self.write_a_line(line)



    def testing_routine(self):
        #test_frequency='200'
        frequency_tolerance=0.05
        #print self.test_routine_state

        if   (self.test_routine_state=='initial' and 
              self.start_test==True                      ): 
 
            self.break_the_motor()
            self.test_routine_state='breaking'
            #line=raw_input("Enter to continue: ") 

        #in case there was a transmition error and the referente did not change to 0Hz
        elif   (self.test_routine_state=='breaking' and 
               self.electric_frequency>2.0               ): 
            self.break_the_motor()
            self.test_routine_state='breaking'
            #line=raw_input("breaking again, Enter to continue: ")        
        


        elif (  self.test_routine_state=='breaking' and 
                self.transmition_error ==False      and 
                self.electric_frequency<4.0            ):


           self.change_frequency(self.test_frequency)
           #self.change_frequency('777')
           self.capturing_data()
           self.test_routine_state='changing_frequency_reference'
           #print "self.test_frequency" + self.test_frequency
           #line=raw_input("Enter to continue aaaaahhhhhhhh!!!: ") 
        
           #in case there was a transmition error and the reference frequency did no change in to the test_frequency
           
        elif (  self.test_routine_state=='changing_frequency_reference' and 
                self.transmition_error ==False                and 
                self.time>self.min_test_time):#self.reference_frequency!=float(self.test_frequency) ):


           self.change_frequency(self.test_frequency)
           #self.change_frequency('777')
           self.capturing_data()
           self.test_routine_state='changing_frequency_reference'
           #print "self.test_frequency" + self.test_frequency
           #line=raw_input("Enter to continue changing_frequencies aaaaahhhhhhhh!!!: ") 
        
        elif (  self.test_routine_state=='changing_frequency_reference' and 
                self.transmition_error ==False                and 
                self.time<self.min_test_time):#self.reference_frequency!=float(self.test_frequency) ):


           self.change_frequency(self.test_frequency)
           #self.change_frequency('777')
           self.capturing_data()
           self.test_routine_state='changing_frequency'
           #print "self.test_frequency" + self.test_frequency
           #line=raw_input("Enter to continue changing_frequencies aaaaahhhhhhhh!!!: ") 
        
                
       



 
        elif (  self.test_routine_state=='changing_frequency'                   and 
                self.transmition_error ==              False                    and 
                self.time>=self.max_test_time                                       ):
            #self.electric_frequency<(1.0+frequency_tolerance)*float(self.test_frequency)  and
            #self.electric_frequency>(1.0-frequency_tolerance)*float(self.test_frequency)      
            self.break_the_motor();
            self.end_capturing_data()
            self.test_routine_state='stopping_the_motor'
            #line=raw_input("Enter to continue: ") 
 

        elif (  self.test_routine_state=='stopping_the_motor'                   and
                self.transmition_error==False                                   and
                self.electric_frequency<0.01                                        ):

            self.test_routine_state='motor_stopped'
            self.start_test=False
            #line=raw_input("Enter to continue: ") 


        elif (  self.test_routine_state=='motor_stopped' and
                self.start_test==True                            ):

            #self.break_the_motor()
            self.test_routine_state='breaking'
            #line=raw_input("Enter to continue: ") 
            


    def torque_testing_routine(self):

        frequency_tolerance=0.05

        if   (self.torque_test_routine_state=='initial' and 
              self.torque_start_test==True                      ): 
 
            self.break_the_motor_torque()
            self.torque_test_routine_state='changing_frequency_reference'#'breaking'


        elif   (self.torque_test_routine_state=='breaking' and 
               self.electric_frequency>2.0               ): 
            self.break_the_motor_torque()
            self.break_the_motor_torque()
            self.break_the_motor_torque()
            self.break_the_motor_torque()
            self.torque_test_routine_state='breaking'

 
        elif (  self.torque_test_routine_state=='breaking' and 
                self.transmition_error ==False      and 
                self.electric_frequency<4.0            ):

           self.change_torque(self.test_torque)
           self.change_torque(self.test_torque)
           self.change_torque(self.test_torque)
           self.change_torque(self.test_torque)
           self.capturing_data()
           self.torque_test_routine_state='changing_frequency_reference'
           
        elif (  self.torque_test_routine_state=='changing_frequency_reference' and 
                self.transmition_error ==False                and 
                self.time>self.min_test_time):#self.reference_frequency!=float(self.test_frequency) ):
           self.change_torque(self.test_torque)   #######################################********************
           self.change_torque(self.test_torque)
           self.change_torque(self.test_torque)
           self.change_torque(self.test_torque) 
           self.capturing_data()
           self.torque_test_routine_state='changing_frequency_reference'
           
        
        elif (  self.torque_test_routine_state=='changing_frequency_reference' and 
                self.transmition_error ==False                and 
                self.time<self.min_test_time):#self.reference_frequency!=float(self.test_frequency) ):

           self.change_torque(self.test_torque)
           self.change_torque(self.test_torque)
           self.change_torque(self.test_torque)
           self.change_torque(self.test_torque) #################################********************
           self.capturing_data()
           self.torque_test_routine_state='changing_frequency'
 
        elif (  self.torque_test_routine_state=='changing_frequency'                   and 
                self.transmition_error ==              False                    and 
                self.time>=self.max_test_time                                       ):
            self.break_the_motor_torque()
            self.break_the_motor_torque()
            self.break_the_motor_torque()
            self.break_the_motor_torque()
            self.end_capturing_data()
            self.torque_test_routine_state='stopping_the_motor'

        elif (  self.torque_test_routine_state=='stopping_the_motor'                   and
                self.transmition_error==False                                   and
                self.electric_frequency<0.1                                        ):
            self.torque_test_routine_state='motor_stopped'
            self.torque_start_test=False

        elif (  self.torque_test_routine_state=='motor_stopped' and
                self.torque_start_test==True                            ):
            self.torque_test_routine_state='breaking'


        if (  self.torque_test_routine_state=='changing_frequency_reference'):
            print "time: "+str(self.time)+" min: "+str(self.min_test_time)+" transmition_error: " +str(self.transmition_error)


    def driving_tests_for_every_set_of_data(self):
    
        if   (self.start_driving_test==True and 
            self.driving_test_state=='initial'):
            self.print_selection_setup(0)
            self.start_test=True 
            self.driving_test_state='printing_0'
            self.driving_counter=0

        
        elif (self.start_test==False and 
            self.driving_test_state=='printing_'+str(self.driving_counter) and 
            self.driving_counter<7):
            self.print_selection_setup(self.driving_counter+1)
            self.start_test         =True 
            self.driving_test_state ='printing_'+str(self.driving_counter+1)
            self.driving_counter    =self.driving_counter+1

        elif (self.start_test==False and 
            self.driving_test_state=='printing_7'):
            self.print_selection_setup(7)
            self.start_test=False 
            self.driving_test_state='initial'
            self.start_driving_test=False

        '''
        elif (self.start_test==False and 
            self.driving_test_state=='printing_0'):
            self.print_selection_setup(1)
            self.start_test=True 
            self.driving_test_state='printing_1'

                           
        elif (self.start_test==False and 
            self.driving_test_state=='printing_1'):
            self.print_selection_setup(0)
            self.start_test=False 
            self.driving_test_state='initial'
            self.start_driving_test=False
        '''            
        

    def torque_driving_tests_for_every_set_of_data(self):
    
        if   (self.torque_start_driving_test==True and 
              self.torque_driving_test_state=='initial'):
            self.print_selection_setup(0)
            self.torque_start_test=True 
            self.torque_driving_test_state='printing_0'
            self.torque_driving_counter=0
        
        elif (self.torque_start_test==False and 
            self.torque_driving_test_state=='printing_'+str(self.torque_driving_counter) and 
            self.torque_driving_counter<7):
            self.print_selection_setup(self.torque_driving_counter+1)
            self.torque_start_test         =True 
            self.torque_driving_test_state ='printing_'+str(self.torque_driving_counter+1)
            self.torque_driving_counter    =self.torque_driving_counter+1

        elif (self.torque_start_test==False and 
            self.torque_driving_test_state=='printing_7'):
            self.print_selection_setup(7)
            self.torque_start_test          =False 
            self.torque_driving_test_state  ='initial'
            self.torque_start_driving_test  =False




    def capturing_data(self):
        self.capture_data       = True
        self.capture_counter    = 0
        self.create_log_file()
        self.read_capture_state = 'not_collecting'       

    def end_capturing_data(self):
        self.capture_data=False
        if   self.print_selection==9: 
            self.plot_all_in_one()
            #self.plot_one_by_one()
        else                        :
            self.plot_selection() 
        self.log_file.close()
        self.read_capture_state == 'not_collecting'
        self.creating_data_vectors() #it empties the vectors        
 
    def print_selection_setup(self,selection):
        self.capture_data=False
        self.print_selection=selection#int(split_command[1])     
        self.write_a_line( 'p '+str(selection) )              



    def P_test(self):                                          

        '''
        if self.P!=100:
            self.max_test_time=50000
        else :
            self.max_test_time=1500000
        '''                 
        

        if self.P_speed_state=='initial' and self.start_P_test==True:
            line='P '+str(self.P)
            print line
            self.write_a_line(line)
            self.P_speed_state='waiting for P update_0'
            #self.print_selection_setup(8)

        elif self.P_speed_state=='waiting for P update_'+str(self.P_test_counter) and self.P_speed==self.P and self.P_test_counter<self.max_P_tests:
            self.start_test=True;
            self.title_extra=' (P='+str(self.P/self.P_divisor)+')'
            self.print_selection_setup(0)#********************************0)#self.P_test_counter)
            #self.path              =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'   
            self.P_speed_state='testing_'+str(self.P_test_counter)

        elif self.P_speed_state=='waiting for P update_'+str(self.P_test_counter) and self.P_speed!=self.P  and self.P_test_counter<self.max_P_tests:
            linex='P python: '+str(self.P)+' P stm32: '+str(self.P_speed)
            print linex
            line='P '+str(self.P)
            self.write_a_line(line)
            self.P_speed_state='waiting for P update_'+str(self.P_test_counter)


            '''
            elif self.P_speed_state=='waiting for P update' and self.P_speed==self.P :
                self.start_test=True;
                self.print_selection_setup(0)
                #self.path              =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'   
                self.P_speed_state='testing'

            elif self.P_speed_state=='waiting for P update' and self.P_speed!=self.P :
                linex='P python: '+str(self.P)+' P stm32: '+str(self.P_speed)
                print linex
                line='P '+str(self.P)
                self.write_a_line(line)
                self.P_speed_state='waiting for P update'
            '''


        elif self.P_speed_state=='testing_'+str(self.P_test_counter) and self.start_test==False and self.P_test_counter<self.max_P_tests:

            self.P_test_counter=self.P_test_counter+1

            if self.P_test_counter<self.max_P_tests:    
                            self.P_speed_state='waiting for P update_'+str(self.P_test_counter)

                            #Pincrement                            
                            self.P=self.P*self.P_increment

                            
                            line='P '+str(self.P)
                            print line
                            self.write_a_line(line)
          
            else                                   :    
                            self.start_P_test=False
                            self.P_speed_state='initial'        
                            self.P=0
                            self.P_test_counter=0
 
            #self.print_selection_setup(8)



        
        elif self.P_speed_state=='testing_'+str(self.max_P_tests) and self.start_test==False:
            self.start_P_test=False
            self.P_speed_state='initial'        
            self.P=0
            self.P_test_counter=0

    def torque_P_test(self):                                          

        '''
        if self.P!=100:
            self.max_test_time=50000
        else :
            self.max_test_time=1500000
        '''                 
        
        if self.torque_P_speed_state=='initial' and self.torque_start_P_test==True:
            line='# '+str(self.torque_P)
            print line
            self.write_a_line(line)
            self.torque_P_speed_state='waiting for P update_0'
        
        elif self.torque_P_speed_state=='waiting for P update_'+str(self.torque_P_test_counter) and self.torque_P_speed==self.torque_P and self.torque_P_test_counter<self.torque_max_P_tests:
            self.torque_start_test=True;
            self.title_extra=' (P='+str(self.torque_P/self.torque_P_divisor)+')'
            self.print_selection_setup(6)#********************************0)#self.P_test_counter)
            self.torque_P_speed_state='testing_'+str(self.torque_P_test_counter)

        elif self.torque_P_speed_state=='waiting for P update_'+str(self.torque_P_test_counter) and self.torque_P_speed!=self.torque_P  and self.torque_P_test_counter<self.torque_max_P_tests:
            linex='P python: '+str(self.torque_P)+' P stm32: '+str(self.torque_P_speed)
            print linex
            line='# '+str(self.torque_P)
            self.write_a_line(line)
            self.torque_P_speed_state='waiting for P update_'+str(self.torque_P_test_counter)


        elif self.torque_P_speed_state=='testing_'+str(self.torque_P_test_counter) and self.torque_start_test==False and self.torque_P_test_counter<self.torque_max_P_tests:

            self.torque_P_test_counter=self.torque_P_test_counter+1

            if self.torque_P_test_counter<self.torque_max_P_tests:    
                            self.torque_P_speed_state='waiting for P update_'+str(self.torque_P_test_counter)

                            #Pincrement                            
                            self.torque_P=self.P*self.torque_P_increment

                            
                            line='# '+str(self.torque_P)
                            print line
                            self.write_a_line(line)
          
            else                                   :    
                            self.torque_start_P_test=False
                            self.torque_P_speed_state='initial'        
                            self.torque_P=0
                            self.torque_P_test_counter=0

     
        elif self.torque_P_speed_state=='testing_'+str(self.torque_max_P_tests) and self.torque_start_test==False:
            self.torque_start_P_test=False
            self.torque_P_speed_state='initial'        
            self.torque_P=0
            self.torque_P_test_counter=0


    def I_test(self):                                          

        '''
        if self.I!=100:
            self.max_test_time=50000
        else :
            self.max_test_time=1500000
        '''                 
        

        if self.I_speed_state=='initial' and self.start_I_test==True:
            line='I '+str(self.I)
            print line
            self.write_a_line(line)
            self.I_speed_state='waiting for I update_0'
            #self.print_selection_setup(8)

        elif self.I_speed_state=='waiting for I update_'+str(self.I_test_counter) and self.I_speed==self.I and self.I_test_counter<self.max_I_tests:
            self.start_test=True;
            self.title_extra=' (I='+str(self.I/self.I_divisor)+')'
            self.print_selection_setup(0)#********************************0)#self.I_test_counter)
            #self.path              =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'   
            self.I_speed_state='testing_'+str(self.I_test_counter)

        elif self.I_speed_state=='waiting for I update_'+str(self.I_test_counter) and self.I_speed!=self.I  and self.I_test_counter<self.max_I_tests:
            linex='I python: '+str(self.I)+' I stm32: '+str(self.I_speed)
            print linex
            line='I '+str(self.I)
            self.write_a_line(line)
            self.I_speed_state='waiting for I update_'+str(self.I_test_counter)


        elif self.I_speed_state=='testing_'+str(self.I_test_counter) and self.start_test==False and self.I_test_counter<self.max_I_tests:

            self.I_test_counter=self.I_test_counter+1

            if self.I_test_counter<self.max_I_tests:    
                            self.I_speed_state='waiting for I update_'+str(self.I_test_counter)

                            #Iincrement                            
                            self.I=self.I*self.I_increment

                            
                            line='I '+str(self.I)
                            print line
                            self.write_a_line(line)
          
            else                                   :    
                            self.start_I_test=False
                            self.I_speed_state='initial'        
                            self.I=0
                            self.I_test_counter=0
 
            #self.print_selection_setup(8)

        
        elif self.I_speed_state=='testing_'+str(self.max_I_tests) and self.start_test==False:
            self.start_I_test=False
            self.I_speed_state='initial'        
            self.I=0
            self.I_test_counter=0



    def testing_for_various_frequencies(self):
        self.test_frequency_array =('10','50','100','200','300','400','500','600','640')
    
        if   (self.start_various_test==True and 
            self.various_test_state=='initial'):
            self.print_selection_setup(self.type_of_test)
            self.start_test= True 
            self.various_test_state='various_0'
            self.various_counter=0
            self.test_frequency= self.test_frequency_array [0]
            self.title_extra=' (f='+self.test_frequency_array [0]+'Hz)'

        
        elif (self.start_test==False and 
            self.various_test_state=='various_'+str(self.various_counter) and 
            self.various_counter<len(self.test_frequency_array)-1):
            self.print_selection_setup(self.type_of_test)
            self.start_test         =True 
            self.various_test_state ='various_'+str(self.various_counter+1)
            self.various_counter    =self.various_counter+1
            self.test_frequency= self.test_frequency_array [self.various_counter]
            self.title_extra=' (f='+self.test_frequency_array [self.various_counter]+'Hz)'
               

        elif (self.start_test==False and 
            self.various_test_state=='various_'+str(len(self.test_frequency_array)-1)):
            self.print_selection_setup(self.type_of_test)
            self.start_various_test=False 
            self.various_test_state='initial'
            self.start_various_test=False

       
    '''          
    def capture_c_button_end(self):

        if self.capture_c_button_state=='initial' and  self.capture_c_button == True and self.time<2:
            self.capture_c_button_state='capturing'

        elif self.capture_c_button_state=='capturing' and self.capture_c_button == True and self.time>=self.max_test_time:
            self.end_capturing_data()
            self.capture_c_button=False
            self.capture_c_button_state='initial'
    '''    
          
    def capture_c_button_end(self):

        if self.capture_c_button_state=='initial' and  self.capture_c_button == True and self.time<2:
            self.capture_c_button_state='capturing'
            self.capturing_data()

        elif self.capture_c_button_state=='capturing' and self.capture_c_button == True and self.time>=self.max_test_time and self.empty_vectors==True:
            self.end_capturing_data()
            self.empty_vectors=False
            self.write_a_line('c ')
            #self.capturing_data()
            #self.tag_comment       =line+self.aditional_comment+self.print_selection_tags()#raw_input("Enter comment: ") 
            self.capture_c_button = True
            self.capture_c_button_state='initial'
                  

        elif self.capture_c_button_state=='capturing' and self.capture_c_button == True and self.time>=self.max_test_time:
            self.end_capturing_data()
            
            if self.empty_vectors==True:
                self.capture_c_button=True
                self.empty_vectors=False
                self.write_a_line('c ')
                self.write_a_line('c ')
                self.write_a_line('c ')
            else: 
                self.capture_c_button=False

            self.capture_c_button_state='initial'


    '''
    def capture_c_button_end(self):
         
        self.time_window=0.1

        self.time_now_c_capture=datetime.datetime.now()
        self.time_difference_c_capture=self.time_now_c_capture-self.time_starting_c_capture
        self.seconds=self.time_difference_c_capture.seconds+self.time_difference_c_capture.microseconds/1000000.0


        if self.capture_c_button_state=='initial' and  self.capture_c_button == True:
            self.capture_c_button_state='initial frequencies'
            self.print_selection_setup(0)
            #self.print_selection_setup(0)
            #self.write_a_line('c ')
            #self.write_a_line('c ')
            print "waiting like idiots 1"
  
 
        elif self.capture_c_button_state=='initial frequencies' and  self.capture_c_button == True:
            self.capture_c_button_state='capturing frequencies'
            time.sleep(1)
            self.capturing_data()
            self.force_append =True
            self.time_starting_c_capture=datetime.datetime.now()
            

        elif self.capture_c_button_state=='capturing frequencies' and self.capture_c_button == True and self.seconds>=self.time_window:
            self.end_capturing_data()
            self.force_append =False
            self.capture_c_button_state='initial three-phase currents'
            self.print_selection_setup(1)
            #self.print_selection_setup(1)
            #self.write_a_line('c ')
            self.write_a_line('c ')
            print "waiting like idiots 2"
  

        elif self.capture_c_button_state=='initial three-phase currents' and  self.capture_c_button == True: 
            self.capture_c_button_state='capturing three-phase currents'
            time.sleep(1)
            self.capturing_data()  
            self.force_append =True  
            self.time_starting_c_capture=datetime.datetime.now() 
            print "waiting like idiots 3"
            self.seconds=0.0
            #time.sleep(1)  

        elif self.capture_c_button_state=='capturing three-phase currents' and self.capture_c_button == True and self.seconds<self.time_window:
            #self.end_capturing_data()
            #self.capture_c_button_state='initial'
            print "waiting like idiots 4"
            #self.print_selection_setup(1)
            #self.print_selection_setup(1)
            #self.write_a_line('c ')
            #self.write_a_line('c ')
            #time.sleep(1) 


        elif self.capture_c_button_state=='capturing three-phase currents' and self.capture_c_button == True and self.seconds>=self.time_window:
            self.end_capturing_data()
            self.capture_c_button_state='initial'
            print "waiting like idiots 5"
            self.force_append =False
            #self.print_selection_setup(1)
            #self.print_selection_setup(1)
            #self.write_a_line('c ')
            #self.write_a_line('c ')
            time.sleep(5)            

            self.capture_c_button=False
    '''
    
    def print_selection_tags(self):
        if   self.print_selection==0: tag='frequencies'
        elif self.print_selection==1: tag='three-phase_currents'
        elif self.print_selection==2: tag='isQ_vs_isD'
        elif self.print_selection==3: tag='VsQ_vs_VsD'
        elif self.print_selection==4: tag='voltage_magnitude'
        elif self.print_selection==5: tag='flux-linkage'
        elif self.print_selection==6: tag='electromagnetic_torque'
        elif self.print_selection==7: tag='phase_advance_pi'
        elif self.print_selection==8: tag=''
        elif self.print_selection==9: tag=''
        else                        : tag=''

        return tag

    def write(self):	

         #finite state machines
         self.debug_code()
         self.P_test()
         self.I_test()
         self.testing_for_various_frequencies()
         self.driving_tests_for_every_set_of_data()
         self.testing_routine()

         self.torque_driving_tests_for_every_set_of_data()
         self.torque_testing_routine()
         self.torque_P_test()
         #if self.transmition_error: print "transmition_error"    

         self.capture_c_button_end()

              


         while sys.stdin in select.select([sys.stdin], [], [], 0)[0]:	#read from standart input if there is something, otherwise not 
            line = sys.stdin.readline()     #you must press enter before typing the new command
            if line:
                line=raw_input("Enter new command: ")   #the printing of the stm32f4 data is stopped until you type a new reference
                if line:
                    #print "line captured in the first while: " + line
                    #raw_input("Enter enter if you want to live: ")
                    split_command = line.split()

                    #updating reference frequency
                    if     split_command[0]=='d':
                        self.write_a_line(line)

                    elif   split_command[0]=='D':
                        #self.damping=float (split_command[1])
                        self.write_a_line(line)

                    elif   split_command[0]=='K':
                        #self.stiffness=float (split_command[1])
                        self.write_a_line(line)

                    elif   split_command[0]=='E':
                        #self.reference_electric_angle=float (split_command[1])
                        self.write_a_line(line)

                    elif   split_command[0]=='M':
                        #self.reference_mechanic_angle=float (split_command[1])
                        self.write_a_line(line)

                    elif   split_command[0]=='G':
                        #self.reference_gear_angle=float (split_command[1])
                        self.write_a_line(line)



                    #load angle
                    elif     split_command[0]=='L':
                        self.write_a_line(line)
                    #psi_ref


                    #updating reference torque
                    elif     split_command[0]=='Q':
                        self.write_a_line(line)
                    
                    #capturing data into csv
                    elif   split_command[0]=='c':
                        self.tag_comment       =line+self.aditional_comment+self.print_selection_tags()#raw_input("Enter comment: ") 
                        self.path              =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'  
                        self.title_extra=''
                        self.write_a_line(line)
                        self.write_a_line('c ')
                        self.write_a_line('c ')

                        #self.capturing_data()
                        #self.tag_comment       =line+self.aditional_comment+self.print_selection_tags()#raw_input("Enter comment: ") 
                        self.capture_c_button = True

                    elif   split_command[0]=='C':
                        #self.tag_comment       =line+self.aditional_comment+self.print_selection_tags()#raw_input("Enter comment: ") 
                        #self.path              =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'  
                        #self.title_extra=''
                        self.write_a_line('c ')
                        self.write_a_line('c ')
                        self.write_a_line('c ')

                        #self.capturing_data()
                        #self.tag_comment       =line+self.aditional_comment+self.print_selection_tags()#raw_input("Enter comment: ") 
                        self.capture_c_button = True




                    elif split_command[0]=='f':
                        self.end_capturing_data()

                    #selecting what to print
                    elif split_command[0]=='p':
                        #self.capture_data=False
                        #self.print_selection=int(split_command[1])     
                        #self.write_a_line(line)
                        self.print_selection_setup(int(split_command[1]))
                        
                    elif split_command[0]=='t':
                        
                        self.start_driving_test=True;
                        self.test_frequency    =split_command[1]
                        self.tag_comment       =line+self.aditional_comment#raw_input("Enter comment: ") 
                        self.path              =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'  
                        self.title_extra=''

                    elif split_command[0]=='ttorque':
                        
                        self.torque_start_driving_test=True;
                        self.test_torque              =split_command[1]
                        self.tag_comment              =line+self.aditional_comment 
                        self.path                     =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'  
                        self.title_extra=''

                    elif split_command[0]=='va':
                        
                        self.start_various_test=True;
                        #self.test_frequency    =split_command[1]
                        self.type_of_test      =int(split_command[1])
                        self.tag_comment       =line+self.aditional_comment#raw_input("Enter comment: ") 
                        self.path              =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'  

                    elif split_command[0]=='all':
                        
                        self.start_test=True;
                        self.print_selection_setup(9)
                        self.test_frequency    =split_command[1]
                        self.tag_comment       =line+self.aditional_comment#raw_input("Enter comment: ") 
                        self.path              =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'  


                    elif split_command[0]=='one':
                        self.test_command='d'#'G';
                        self.start_test=True;
                        self.print_selection_setup(int(split_command[2]))
                        self.test_frequency    =split_command[1]
                        self.tag_comment       =line+self.aditional_comment#raw_input("Enter comment: ") 
                        self.path              =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'  


                    elif split_command[0]=='ONE':
                        
                        self.start_test=True;
                        self.print_selection_setup(int(split_command[2]))
                        self.test_frequency    =split_command[1]
                        self.tag_comment       =line+self.aditional_comment#raw_input("Enter comment: ") 
                        #self.path              =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'  


                    elif split_command[0]=='onetorque':
                        
                        self.torque_start_test  =True;
                        self.test_torque        =split_command[1]
                        self.print_selection_setup(int(split_command[2]))
                        self.tag_comment        =line+self.aditional_comment 
                        
                        self.path                =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'  
                        #print "test_torque: " + self.test_torque + " print selection: "+split_command[2]
                        #raw_input("Enter comment: ") 

                    
                    elif split_command[0]=='pp':
                        
                        #self.start_test=True;
                        self.start_P_test=True
                        self.print_selection_setup(8)
                        self.test_frequency    =split_command[1]
                        self.tag_comment       =line+self.aditional_comment
                        #if (len(split_command)>=3): 
                        #    self.tag_comment       =line#raw_input("Enter comment: ") 
                        self.path              =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'  

                    elif split_command[0]=='pi':
                        
                        #self.start_test=True;
                        self.start_I_test=True
                        self.print_selection_setup(10)
                        self.test_frequency    =split_command[1]
                        self.tag_comment       =line+self.aditional_comment
                        self.path              =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'  


                    elif split_command[0]=='ptorque':
                        
                        #self.start_test=True;
                        self.torque_start_P_test=True
                        self.print_selection_setup(10)
                        self.test_torque    =split_command[1]
                        self.tag_comment       =line+self.aditional_comment
                        #if (len(split_command)>=3): 
                        #    self.tag_comment       =line#raw_input("Enter comment: ") 
                        self.path              =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'  

                    elif split_command[0]=='debug':
                        self.debug_command = split_command[1]
                        self.start_debugging=True
                        self.read_strings=True
                        self.print_selection_setup(11)


                        #self.change_frequency (split_command[1])
                        #self.write_a_line('d 1')                  
                        self.tag_comment       =line+self.aditional_comment
                        self.path              =self.root_path + "["+datetime.datetime.now().ctime() +"] ["+self.tag_comment+"]"+'/'
                        self.old_time =0
                    


    def debug_code(self):
                

        if self.start_debugging==True and self.time !=1 and self.debugging_state=='initial':
        #if self.start_debugging==True and self.time >497 and self.debugging_state=='initial':
            self.write_a_line('Q '+str(self.debug_command))#('d 1') 
            self.debugging_state='initial'
     
        elif self.start_debugging==True and self.time ==1 and self.debugging_state=='initial':
        #elif self.start_debugging==True and self.time <10 and self.debugging_state=='initial':
            #self.capturing_data()         
            self.debugging_state='pre-debugging'
        

        elif self.start_debugging==True and self.time ==2 and self.old_time==1 and self.debugging_state=='pre-debugging':
        #elif self.start_debugging==True and self.time<20 and self.old_time<10 and self.debugging_state=='pre-debugging':

            self.capturing_data()         
            self.debugging_state='debugging'
        

        elif self.debugging_state=='debugging' and self.time>=497: 
            self.end_capturing_data()
            self.debugging_state='initial'
            self.start_debugging=False
            self.read_strings=False

        self.old_time=self.time   
                      

