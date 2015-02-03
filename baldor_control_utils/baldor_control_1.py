#!/usr/bin/env python

wheel_diameter=5.95 #inches

import yarp as y
import serial as s
from numpy import pi
from time import sleep






y.Network.init()
port=y.BufferedPortBottle()
portname="/coroco/baldor/in"
port.open(portname)
portout=y.BufferedPortBottle()
portnameout="/coroco/baldor/out"
portout.open(portnameout)


max_speed=12*2*pi
last_raw_angle=0.
raw_angle=0.
angle=0.
rads=0.



    #pyserial configuration
    self.root_path           = "/home/tumacher/local/src/repositories/arcoslab/another_open-coroco/open-coroco/src_DTC-SVM/Python/measures/"
    self.dev_type       ="/dev/ttyACM"
    self.serial_speed   =115200
    self.serial_timeout =1


    def __init__(self):
        self.connecting_to_stm32F4()
               

    def __del__(self):
        self.log_file.close()
        self.ser.close()
        print "---Serial port closed: disconnected from the STM32F4---"
	


    def connecting_to_stm32F4(self):
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

    def read_data_from_stm32_strings(self):
        bytes = 1
        string=''
        #info = ''   #info needs to be set before splitting it, otherwise pythons says it is uninitialized
        single_character   = self.ser.read(bytes)
        if(single_character == "X"):

            while (single_character != "\r"):
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

    def write_a_line(self,line):
        #self.ser.write('\n')
        #self.ser.write('\r') 
        self.ser.write(line)
        self.ser.write('\n')
        self.ser.write('\r')    


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



while True:
    connected=False
    while not connected:
        try:
            com=s.Serial("/dev/ttyACM0", baudrate=921600)
        except:
            print "Connect opencoroco usb cable"
        else:
            connected=True
        sleep(0.1)
    while com.inWaiting()>0:
        com.read(com.inWaiting())
    try:
        while True:
            bottle=port.read(False)
            if bottle:
                yvalue=bottle.get(0)
                value=yvalue.asDouble()
                cmd_speed=value*max_speed
            #print "cmd_speed: ", cmd_speed
                com.write("f "+str(cmd_speed)+"\n\r")
                line=com.readline()
            #line=com.read(com.inWaiting())
            #print "Coroco line: ", line
                line_split=line.split()
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
                portout.write(True)

            #print "raw angle: ", raw_angle, " rads: ", rads, " odometry: ", odometry , " est_freq: ", est_freq
            #while com.inWaiting()>0:
            #    print "Reading stuff", com.inWaiting()
            #    com.read(com.inWaiting())
            #print
            #sleep(1)
    except:
        com.close()
        del com

port.close()
y.Network.fini()
