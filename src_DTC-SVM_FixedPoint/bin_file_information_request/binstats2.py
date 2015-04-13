# script to print memory usage stats for binaries produced by mbed online compiler
# Version 1.0, 2013-07-26 Author: Igor Skochinsky
# Version 2.0, 2013-07-26 Modifications by John VanLaanen
# Based on an the oroginal script by Igor Skochinsky copied from: http://mbed.org/users/igorsk/notebook/mbed-memory-usage/

import sys, struct

# Memory map for the LCP1768
RAM_START  = 0x10000000
RAM_SIZE   = 0x00008000 # 32K
RAM_END = RAM_START + RAM_SIZE - 1

AHB0_START = 0x2007c000
AHB0_SIZE = 0x4000      # 16K
AHB0_END = AHB0_START + AHB0_SIZE - 1

AHB1_START = 0x20080000
AHB1_SIZE = 0x4000      # 16K
AHB1_END = AHB1_START + AHB1_SIZE - 1

FLASH_SIZE = 0x00080000 # 512K

# read a 4-byte dword (little-endian) from file
def getDword(f, off):
  f.seek(off)
  data = f.read(4)
  if len(data) != 4:
    print "Trying to read ouside the file!"
    return 0
  return struct.unpack("= RAM_START) and (entryn.dest <= RAM_END):
        ram_used += entryn.size
    elif (entryn.dest >= AHB0_START) and (entryn.dest <= AHB0_END):
        ahb0_used += entryn.size
    elif (entryn.dest >= AHB1_START) and (entryn.dest <= AHB1_END):
        ahb1_used += entryn.size
    else:
        print("Region %d in an unknown memory location" % ii)
        
    print('Region %d:     source=0x%08X, dest=0x%08X, size=%8d, func=%4d' % \
                (ii+1, entryn.source, entryn.dest, entryn.size, entryn.func) )

  f.seek(0, 2)
  usedflash = f.tell()
  print("\n")
  print "Used flash     = %-7d (0x%05x)" % (usedflash, usedflash)
  print "Free flash     = %-7d (0x%05x)\n" % (FLASH_SIZE - usedflash, FLASH_SIZE - usedflash)
  print "CPU  RAM Used  = %-7d (0x%05x)" % (ram_used, ram_used)
  print "CPU  Free RAM  = %-7d (0x%05x)\n" % (RAM_SIZE - ram_used, RAM_SIZE - ram_used)
  print "AHB0 RAM Used  = %-7d (0x%05x)" % (ahb0_used, ahb0_used)
  print "AHB0 Free RAM  = %-7d (0x%05x)\n" % (AHB0_SIZE - ahb0_used, AHB0_SIZE - ahb0_used)
  print "AHB1 RAM Used  = %-7d (0x%05x)" % (ahb1_used, ahb1_used)
  print "AHB1 Free RAM  = %-7d (0x%05x)" % (AHB1_SIZE - ahb1_used, AHB1_SIZE - ahb1_used)

print "mbed 1768 binary stats script 2.0\n"
if len(sys.argv) >= 2:
  inf = open(sys.argv[1],"rb")
  show_stats(inf)
else:
  print "Usage: MBED_binstats.py file.bin"