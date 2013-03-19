#!/usr/bin/python
from math import sin
sinTableSize=3600.0 #equivalent to 360 degrees

print "#define sinTableSize ", int(sinTableSize)
print "PROGMEM prog_int16_t sin_table[sinTableSize]=",
def print_sin(tablesin):
    print "{",
    for i in range(len(tablesin)-1):
        print tablesin[i],",",
    print tablesin[len(tablesin)-1],"};"

def sin_t(angle):
    return int(0x01ff*sin((angle/sinTableSize)*2*3.1416))



angles=range(int(sinTableSize))

sinValues=map(sin_t,angles)

print_sin(sinValues)

