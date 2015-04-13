#!/usr/bin/python

#/*
# *This file is part of the open-coroco project.
# *
# *  Copyright (C) 2013  Sebastian Chinchilla Gutierrez <tumacher@gmail.com>
# *
# *  This program is free software: you can redistribute it and/or modify
# *  it under the terms of the GNU General Public License as published by
# *  the Free Software Foundation, either version 3 of the License, or
# *  (at your option) any later version.
# *
# *  This program is distributed in the hope that it will be useful,
# *  but WITHOUT ANY WARRANTY; without even the implied warranty of
# *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# *  GNU General Public License for more details.
# *
# *  You should have received a copy of the GNU General Public License
# *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
# */


import math
import sys

table_size=int(sys.argv[1])
table_file = open('sine_table.h','w')

#print "float sine_table [SIN_TABLE_SIZE] =", table_size,";"  
#print "{"



table_file.write('/*')
table_file.write("\n *This file is part of the open-coroco project.\n")
table_file.write(" *")
table_file.write(" *  Copyright (C) 2013  Sebastian Chinchilla Gutierrez <tumacher@gmail.com>\n")
table_file.write(" *\n")
table_file.write(" *  This program is free software: you can redistribute it and/or modify\n")
table_file.write(" *  it under the terms of the GNU General Public License as published by\n")
table_file.write(" *  the Free Software Foundation, either version 3 of the License, or\n")
table_file.write(" *  (at your option) any later version.\n")
table_file.write(" *\n")
table_file.write(" *  This program is distributed in the hope that it will be useful,\n")
table_file.write(" *  but WITHOUT ANY WARRANTY; without even the implied warranty of\n")
table_file.write(" *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n")
table_file.write(" *  GNU General Public License for more details.\n")
table_file.write(" *\n")
table_file.write(" *  You should have received a copy of the GNU General Public License\n")
table_file.write(" *  along with this program.  If not, see <http://www.gnu.org/licenses/>.\n")
table_file.write(" */\n")





table_file.write ("#define SIN_TABLE_SIZE   ")
table_file.write (repr(table_size))
table_file.write ("\n#define SIN_TABLE_SIZE_F ")
table_file.write (repr(table_size-1))
table_file.write ("\n\nfloat sine_table [SIN_TABLE_SIZE] = ")
table_file.write ("\n{\n")


for position in range(table_size):
  sine=math.sin(position *(360*math.pi/180)/(table_size-1) )
  table_file.write (repr(sine))
  table_file.write ("f,\n")
  #print "sine: ",sine,"f,"

  

table_file.write ("};")
table_file.close()
#print "}"
