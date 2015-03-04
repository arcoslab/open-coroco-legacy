#!/usr/bin/env python

import os

try:	
	#Commands
	yarpserver = 'xterm -geometry +600+0 -e "yarpserver --write" &'
	joystick = 'xterm -geometry +0+0 -e "python ps3controller.py" &'
	test_1 = 'xterm   -geometry +0+900 -e "python test_1.py " &'
	test_2 = 'xterm  -geometry +1280+0 -e "python test_2.py " &'
	test_3 = 'xterm  -geometry +1280-0 -e "python test_3.py " &'
	test_4 = 'xterm  -geometry +600+400 -e "python test_4.py " &'

	#Run the commands
	os.system(yarpserver)
	os.system(joystick)
	os.system(test_1)
	os.system(test_2)
	os.system(test_3)
	os.system(test_4)

	#Just for keeping the program running
	while(True):
		s = raw_input('Just keeping the program running ')

#If Ctrl+C... then kill all the python programs and yarpserver
except KeyboardInterrupt:
	
	print "Killing all "
	kill = "killall python yarpserver"
	os.system(kill)
