import serial   as s
from serial import SerialException


class motor_position(object):
    position='0'
    serial=0


class motor_tools(object):

    #serial port configuration
    baudrate=921600
    timeout=1
    serial_device_counter=0
    MAX_serial_device_counter=100
    device_type="/dev/ttyACM"
    #com=s.Serial(device_type+str(serial_device_counter), baudrate=baudrate,timeout=timeout)
    character="f"
    connected=False
    position='0'




    #--------------serial port connection------------------------------

    def connecting_to_stm32(self,motor_number,serial_1,serial_2,serial_3,serial_4):

        while not self.connected:
            try:
                print "self.serial: ",self.serial_device_counter," serial_1: ",serial_1
                #raw_input("raw_input")
                #sleep(0.01)

                print "self.serial: ",self.serial_device_counter," serial_1: ",serial_1
                #raw_input("raw_input")
                #sleep(0.01)
                
                print "motor number: ", motor_number
                #raw_input ("raw_input")
                
                #serial_device_counter=3;
                if motor_number==2 or motor_number==3 or motor_number==4:
           
                    print "self.serial: ",self.serial_device_counter," serial_1: ",serial_1
                    #raw_input("raw_input")
                    if self.serial_device_counter==serial_1:
                        self.serial_device_counter=serial_1+1
                        print "first"
                if motor_number==3 or motor_number==4:
                
                    if self.serial_device_counter==serial_2:
                        self.serial_device_counter=serial_2+1
                        print "second"
                if motor_number==4:

                    if self.serial_device_counter==serial_3:
                        self.serial_serial_device_counter=serial_3+1
                        print "third"
                



                #serial_device_counter=3;
                self.com=s.Serial(self.device_type+str(self.serial_device_counter), baudrate=self.baudrate,timeout=self.timeout)
                print "Connecting toooo "+self.device_type+str(self.serial_device_counter)
            except:
                
                if (self.serial_device_counter>self.MAX_serial_device_counter):
                  self.serial_device_counter=0
                print "Connect opencoroco usb cable, trying: "+self.device_type+str(self.serial_device_counter)  
                print "stm32_position: " + self.position     
                self.serial_device_counter=self.serial_device_counter+1  
            else:
                self.connected=True
                print "Connected to "+self.device_type+str(self.serial_device_counter) 
                #print "stm32_position: " + stm32_position


    def reading_serial_port(self):
        line=self.com.readline() 
        print "line_from_stm: ",line

        try: 
            line_split=line.split()        
            self.position=line_split[0]
            print "stm32_position: " + self.position
        except: 
            self.position='0'
            print "stm32_position: " + self.position
        

    def serial_port_communication(self,motor,cmd_speed,value):

        #waiting for data from stm32 serial port
        while self.com.inWaiting()>0:
            self.com.read(self.com.inWaiting())
            print "waiting"  
       
        #writing data from stm32
        self.com.write(self.character+" "+str(cmd_speed)+"\n\r")
        print "writing to stm32: "+ str(cmd_speed)+"original_value: "+str(value)+"test: "
        print "Connected to /dev/ttyACM"+str(self.serial_device_counter)   

        #reading data from stm32
        self.reading_serial_port()



    def retry_serial_connection(self):
        self.connected=False
        print "wrong stm32, detected: ",self.position
        self.position='0'
        self.serial_device_counter=self.serial_device_counter+1
        
    def serial_exception_handling(self):
        #com.close()
        #del com
        print self.com.inWaiting()
        print "error! error!"
        #print line
        self.connected=False  




    #------------------yarp functions--------------------------

    def checking_motor_position(self,motor_number,motor,input_port,input_port_name):
        while motor.position!=motor_number and motor.position!='0':

            #receaving a bottle from the input port 1
            input_bottle=input_port.read(False)
            if input_bottle:
                input_data=input_bottle.get(0)
                motor.position=input_data.asString()

                input_data=input_bottle.get(1)
                motor.serial=input_data.asInt()
                print "there is a bottle from ",input_port_name
                print "bottle content: position=>",motor.position," serial=>",motor.serial
                
            else:
                print "there is no bottle from ",input_port_name    
                
        return motor



    def send_a_bottle(self,out,position,serial_device_counter):

        bottle_out=out.prepare()
        bottle_out.clear()
        bottle_out.addString(position)
        bottle_out.addInt(serial_device_counter)                
        out.write()


    def receive_a_motor_bottle(self,input_port,port_name):
        
        input_bottle=input_port.read(False)
        if input_bottle:
            input_data=input_bottle.get(0)
            position=input_data.asString()
            print "there is a bottle from ",port_name
            print "bottle content: ",position
            return position
            
        else:
            print "there is no bottle from ",port_name    
            return 0


    def receive_a_joystick_bottle(self,input_port,port_name):

        input_bottle=input_port.read(False)
        if input_bottle:
            input_data=input_bottle.get(0)
            speed_value=input_data.asDouble()
            cmd_speed=speed_value
            print "there is a bottle from ",port_name
            print "bottle content: ",speed_value
            return cmd_speed
            
        else:
            print "there is no bottle from ",port_name 
            return 0 
        
