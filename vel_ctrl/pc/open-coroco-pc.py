#!/usr/bin/env python3

import serial
from numpy import pi

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

    def get_cur_speed(self):
        return(self.est_speed)

    def get_currents(self):
        return(self.currents)


if __name__=="__main__":
    open_coroco_1=Open_coroco()
    open_coroco_1.connect()

    open_coroco_1.set_speed(2*pi)

    while (True):
        if open_coroco_1.update_ascii_data():
            print("Current speed: "+str(open_coroco_1.get_cur_speed()))
            print("Currents: "+str(open_coroco_1.get_currents()))
