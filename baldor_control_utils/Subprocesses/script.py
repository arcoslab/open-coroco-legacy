import os
import subprocess

#esto cambia el directorio del bash a home
os.chdir ('/home')

#el while repite el proceso indefinidamente, imprime el error solo para info
#subprocess.call corre en el bash lo que esta dentro de ''
#cada comando del bash va separado por " , "
#subprocess.call devuelve 0 si no hay error o 1 si hay error

while subprocess.call(["python", "mcalderon/Desktop/Subprocesses/ps3controller.py"])==1:
    print "error running first file, attempting again"

while subprocess.call(["python","mcalderon/Desktop/Subprocesses/test_1.py"])==1:
    print "error running first file, attempting again"

while subprocess.call(["python", "mcalderon/Desktop/Subprocesses/test_2.py"])==1:
    print "\n error running first file, attempting again \n"
    
while subprocess.call(["python", "mcalderon/Desktop/Subprocesses/test_3.py"])==1:
    print "\n error running first file, attempting again \n"

while subprocess.call(["python", "mcalderon/Desktop/Subprocesses/test_4.py"])==1:
    print "\n error running first file, attempting again \n"

