import serial   as s
from serial import SerialException

class motor_tools(object):

    #serial port configuration
    baudrate=921600
    timeout=1
    serial_device_counter=0
    MAX_serial_device_counter=100
    device_type="/dev/ttyACM"
    

    connected=False
    position='0'

    def connecting_to_stm32(self):

        while not self.connected:
            try:
                #serial_device_counter=3;
                com=s.Serial(self.device_type+str(self.serial_device_counter), baudrate=self.baudrate,timeout=self.timeout)
                print "Connecting toooo "+self.device_type+str(self.serial_device_counter)
            except:
                
                if (self.serial_device_counter>self.MAX_serial_device_counter):
                  self.serial_device_counter=0
                print "Connect opencoroco usb cable, trying fff: "+self.device_type+str(self.serial_device_counter)  
                print "stm32_position: " + self.position     
                self.serial_device_counter=self.serial_device_counter+1  
            else:
                self.connected=True
                print "Connected to "+self.device_type+str(self.serial_device_counter) 
                #print "stm32_position: " + stm32_position
