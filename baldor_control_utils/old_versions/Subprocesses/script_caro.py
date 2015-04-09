#!/usr/bin/env python

import os
import subprocess

#yarpserver= 'xterm  -e "yarpserver" &' 
joystick = 'xterm  -e "python dualshock3_test.py" &'
motor_1 = 'xterm  -e "python motor_1.py " &'
motor_2 = 'xterm  -e "python motor_2.py " &'
motor_3 = 'xterm  -e "python motor_3.py " &'
motor_4 = 'xterm  -e "python motor_4.py " &'
#motor_2 = 'xterm --working-directory=/home/lenouch/local/src/open-coroco/baldor_control_utils/Subprocesses/ -e "python motor_2.py " -H '
#motor_3 = 'xterm --working-directory=/home/lenouch/local/src/open-coroco/baldor_control_utils/Subprocesses/ -e "python motor_3.py " -H '
#motor_4 = 'xterm --working-directory=/home/lenouch/local/src/open-coroco/baldor_control_utils/Subprocesses/ -e "python motor_4.py " -H '

#os.system(yarpserver)
os.system(joystick)
os.system(motor_1)
os.system(motor_2)
os.system(motor_3)
os.system(motor_4)
