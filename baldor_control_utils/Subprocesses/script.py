import os
import subprocess

joystick = 'xfce4-terminal --working-directory=/home/lenouch/local/src/open-coroco/baldor_control_utils/Subprocesses/ -e "python ps3controller.py" -H '
test_1 = 'xfce4-terminal --working-directory=/home/lenouch/local/src/open-coroco/baldor_control_utils/Subprocesses/ -e "python test_1.py " -H '
test_2 = 'xfce4-terminal --working-directory=/home/lenouch/local/src/open-coroco/baldor_control_utils/Subprocesses/ -e "python test_2.py " -H '
test_3 = 'xfce4-terminal --working-directory=/home/lenouch/local/src/open-coroco/baldor_control_utils/Subprocesses/ -e "python test_3.py " -H '
test_4 = 'xfce4-terminal --working-directory=/home/lenouch/local/src/open-coroco/baldor_control_utils/Subprocesses/ -e "python test_4.py " -H '


os.system(joystick)
os.system(test_1)
os.system(test_2)
os.system(test_3)
os.system(test_4)
