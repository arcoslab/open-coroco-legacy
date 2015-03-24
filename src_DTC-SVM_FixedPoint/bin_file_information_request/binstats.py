# script to print memory usage stats for binaries produced by mbed online compiler
# Version 1.0, 2010-01-23
# Author: Igor Skochinsky

import sys, struct

RAM_START  = 0x10000000
RAM_SIZE   = 0x00008000 # 32K
FLASH_SIZE = 0x00080000 # 512K

# read a 4-byte dword (little-endian) from file
def getDword(f, off):
  f.seek(off)
  data = f.read(4)
  if len(data) != 4:
    print "Trying to read ouside the file!"
    return 0
  return struct.unpack("<I", data)[0]

class RegionEntry:
  def __init__(self, f, off):
    f.seek(off)
    data = f.read(4*4)
    if len(data) != 4*4:
      print "Trying to read ouside the file!"
      return
    values = struct.unpack("<IIII", data)
    self.source = values[0]
    self.dest   = values[1]
    self.size   = values[2]
    self.func   = values[3]

def show_stats(f):
  sp = getDword(f, 0)
  if sp != RAM_START + RAM_SIZE:
    print "Warning: stack pointer (%08X) is different from expected value (%08X)" % (sp, RAM_START + RAM_SIZE)
  entry = getDword(f, 4)
  print "Entry point address: %08X" % entry
  if entry & 1 != 1:
    print "Doesn't look like a Cortex-M3 binary"
    return
  entry &= ~1
  f.seek(entry)
  startup = f.read(8)
  # ldr r0, =HWInit
  # blx r0
  # ldr r0, =__main
  # bx  r0
  if startup != "\x06\x48\x80\x47\x06\x48\x00\x47":
    print "Wrong startup code, probably not an mbed 1768 binary"
    return
  __main = getDword(f, entry + 0x20) & ~1
  f.seek(__main)
  if f.read(4) != "\x00\xF0\x02\xF8":
    print "Wrong startup code, probably not an mbed 1768 binary"
    return
  #fetch scatter region table pointers 
  tblptrs = __main+0x34
  tblstart = getDword(f, tblptrs)   + tblptrs
  tblend   = getDword(f, tblptrs+4) + tblptrs
  tblsize = tblend - tblstart
  tblentries = tblsize / 16
  if tblsize % 16 != 0 or tblentries == 0 or tblentries > 2:
    print "Wrong size of scatter region table (%d entries)" % tblentries
    return
  entry1 = RegionEntry(f, tblstart)
  # first entry sets up RW data
  # so its src address will be code + RO data size
  code_rodata = entry1.source
  # size will be the size of RW data
  rwdata = entry1.size
  if tblentries == 2:
    # second entry sets up ZI data
    # its "source" will be the total flash size, but we'll just use the file size
    entry2 = RegionEntry(f, tblstart+16)
    zidata = entry2.size
  else:
    # no ZI data
    zidata = 0
  usedram = rwdata + zidata
  f.seek(0, 2)
  usedflash = f.tell()

  print "Code + RO data = %-7d (0x%05x)" % (code_rodata, code_rodata)
  print "RW data        = %-7d (0x%05x)" % (rwdata, rwdata)
  print "ZI data        = %-7d (0x%05x)" % (zidata, zidata)  
  print "Used flash     = %-7d (0x%05x)" % (usedflash, usedflash)
  print "Free flash     = %-7d (0x%05x)" % (FLASH_SIZE - usedflash, FLASH_SIZE - usedflash)
  print "Used RAM       = %-7d (0x%05x)" % (usedram, usedram)
  print "Free RAM       = %-7d (0x%05x)" % (RAM_SIZE - usedram, RAM_SIZE - usedram)

print "mbed 1768 binary stats script 1.0"
if len(sys.argv) >= 2:
  inf = open(sys.argv[1],"rb")
  show_stats(inf)
else:
  print "Usage: binstats.py file.bin"