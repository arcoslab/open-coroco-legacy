#!/usr/bin/env python3

import serial
from numpy import pi
import struct

class Open_coroco(object):
    def __init__(self):
        ''' Speed in radians/s '''
        self.baudrate=921600
        self.timeout=1
        self.device_name=""
        self.connected=False
        self.cur_angle=0.0
        self.ad2s1210_fault=0
        self.est_speed=0.0
        self.currents=[0.0]*4
        self.currents_rsense=[0.0]*4
        self.voltages=[0.0]*4
        self.temp=0.0
        self.ref_speed=0.0
        self.raw_line=bytes()

    def connect(self, device_name="/dev/ttyACM0", baudrate=921600, timeout=1):
        self.baudrate=baudrate
        self.timeout=timeout
        self.device_name=device_name
        print("connecting to "+device_name)
        self.serial_dev=serial.Serial(self.device_name, baudrate=self.baudrate, timeout=self.timeout)


    def set_speed(self, speed):
        self.ref_speed=speed
        raw_out=struct.pack("h", int(self.ref_speed*100.0))
        print("Data out: "+raw_out.hex())
        self.serial_dev.write(bytes("s", "utf-8")+raw_out)

    def set_ascii_speed(self, speed):
        print("Sending speed")
        self.ref_speed=speed
        send_data=bytes("f "+str(int(speed))+"\n\r", "utf-8")
        print(send_data)
        self.serial_dev.write(send_data)

    def update_ascii_data(self):
        found_n=False
        while self.serial_dev.in_waiting>0:
            #print("Reading serial data")
            raw_data=self.serial_dev.read(self.serial_dev.in_waiting)
            #print("Raw data: "+str(raw_data))
            for n, my_byte in enumerate(raw_data):
                #print("Comparing: "+str(my_byte)+" with "+str(bytes('\n', "utf-8")))
                if my_byte==bytes('\n', "utf-8")[0]:
                    print("End of line")
                    found_n=True
                    break
            n+=len(self.raw_line)
            self.raw_line+=raw_data
        if found_n:
            complete_line=self.raw_line[:n]
            self.raw_line=self.raw_line[n+1:]
            print("New line found and separated: "+str(complete_line))
            print("Next line: "+str(self.raw_line))
            return(True)
        return(False)

    def update_data(self):
        found=False
        while (not found) and (self.serial_dev.in_waiting>0):
            #print("Reading serial data")
            raw_data=self.serial_dev.read(1)
            if raw_data==bytes.fromhex("55"):
                print("55 found")
                while self.serial_dev.in_waiting<=0:
                    pass
                #print("Reading serial data")
                raw_data=self.serial_dev.read(1)
                if raw_data==bytes.fromhex("AA"):
                    print("AA found")
                    found=True

        if found:
            raw_data=bytes()
            while len(raw_data)<(2+1+1+2+2*4+2*4+2+2+2):
                if (self.serial_dev.in_waiting>0):
                    raw_data+=self.serial_dev.read(1)
            print("All data received")
            print(len(raw_data))
            data=struct.unpack("HBh4H4HHHH", raw_data)
            for i in data:
                print("% 06d " % i, end='');
            print("");
        return(found)

    def get_cur_speed(self):
        return(self.est_speed)

    def get_currents(self):
        return(self.currents)


if __name__=="__main__":
    open_coroco_1=Open_coroco()
    open_coroco_1.connect()

    open_coroco_1.set_speed(0*2.0*pi)

    while (True):
        if open_coroco_1.update_data():
            open_coroco_1.set_speed(10*2.0*pi)
            #open_coroco_1.set_ascii_speed(0*2*pi)
            print("Current speed: "+str(open_coroco_1.get_cur_speed()))
            print("Currents: "+str(open_coroco_1.get_currents()))
