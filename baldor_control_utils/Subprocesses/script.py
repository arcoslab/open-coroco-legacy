import os
import subprocess

#Change directory. 
directory = '/home/lenouch/local/src/open-coroco/baldor_control_utils/Subprocesses/'
try: 
	yarpserver = 'xfce4-terminal --working-directory='+directory+' -e "yarpserver --write" --title=Yarpserver --geometry 40x20+0-0 '
	joystick = 'xfce4-terminal --working-directory='+directory+' -e "python ps3controller.py" --title=Joystick --geometry 40x20-0+0 '
	test_1 = 'xfce4-terminal --working-directory='+directory+' -e "python test_1.py " --title=Test1 --geometry 40x20-0-0 '
	test_2 = 'xfce4-terminal --working-directory='+directory+' -e "python test_2.py " --title=Test2 --geometry 40x20+0+0 '
	test_3 = 'xfce4-terminal --working-directory='+directory+' -e "python test_3.py " --title=Test3 --geometry 40x20-300-100'
	test_4 = 'xfce4-terminal --working-directory='+directory+' -e "python test_4.py " --title=Test4 --geometry 40x20+300+100 '

	os.system(yarpserver)	
	os.system(joystick)
	os.system(test_1)
	os.system(test_2)
	os.system(test_3)
	os.system(test_4)


	while(True): 
		prog = input('Program running ')

except KeyboardInterrupt:
	
	print ("Killing all ")
	kill = "killall python yarpserver"
	os.system(kill)
	


