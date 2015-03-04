import os
import subprocess

joystick = 'xterm  -e "python ps3controller.py" &'
test_1 = 'xterm  -e "python test_1.py " &'
test_2 = 'xterm  -e "python test_2.py " &'
test_3 = 'xterm  -e "python test_3.py " &'
test_4 = 'xterm  -e "python test_4.py " &'
#test_2 = 'xterm --working-directory=/home/lenouch/local/src/open-coroco/baldor_control_utils/Subprocesses/ -e "python test_2.py " -H '
#test_3 = 'xterm --working-directory=/home/lenouch/local/src/open-coroco/baldor_control_utils/Subprocesses/ -e "python test_3.py " -H '
#test_4 = 'xterm --working-directory=/home/lenouch/local/src/open-coroco/baldor_control_utils/Subprocesses/ -e "python test_4.py " -H '


os.system(joystick)
os.system(test_1)
os.system(test_2)
os.system(test_3)
os.system(test_4)
