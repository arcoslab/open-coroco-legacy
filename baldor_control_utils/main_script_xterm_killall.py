#!/usr/bin/env python

import os
from time   import sleep
'''
#Commands
yarpserver = 'xterm -geometry +600+0 -e "yarpserver --write" &'
joystick = 'xterm -geometry +0+0 -e "python ps3controller.py" &'
test_1 = 'xterm   -geometry +0-0 -e "python test_1.py " &'
test_2 = 'xterm  -geometry +1280+0 -e "python test_2.py " &'
test_3 = 'xterm  -geometry +1280-0 -e "python test_3.py " &'
test_4 = 'xterm  -geometry +600+400 -e "python test_4.py " &'
'''    



try:	


    
	#Commands
	joystick = 'xterm  -geometry +1200+0 -e "python dualshock3_test.py " &'
	yarpserver = 'xterm  -geometry +1280-0 -e "yarpserver --write" &'
	test_1 = 'xterm -geometry +0+0 -e "python motor_1.py " &'
	test_2 = 'xterm -geometry +600+0 -e "python motor_2.py" &'
   	test_3 = 'xterm   -geometry +0-0 -e "python motor_3.py " &'
	test_4 = 'xterm  -geometry +600+600 -e "python motor_4.py " &'

    	#Run the commands
	os.system(yarpserver)
    	sleep(1.1)
	os.system(joystick)
    	sleep(1.1)
	os.system(test_1)
    	sleep(0.01)
    	os.system(test_2)
    	sleep(0.01)
	os.system(test_3)
   	sleep(0.01)
	os.system(test_4)

	#Just for keeping the program running
	while(True):
		s = raw_input('Just keeping the program running ')

#If Ctrl+C... then kill all the python programs and yarpserver
except KeyboardInterrupt:
	
	print "Killing all "
	kill = "killall yarpserver python "
	os.system(kill)


'''
joystick = 'xterm -geometry +0+0 -e "python dualshock3_test.py " &'
yarpserver = 'xterm -geometry +600+0 -e "yarpserver --write" &'



test_1 = 'xterm   -geometry +0-0 -e "python motor_1.py " &'
test_2 = 'xterm  -geometry +1200+0 -e "python motor_2.py " &'
test_3 = 'xterm  -geometry +1280-0 -e "python motor_3.py " &'
test_4 = 'xterm  -geometry +600+600 -e "python motor_4.py " &'
'''
