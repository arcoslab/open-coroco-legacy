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




from  Serial_Stm32f4 import *
    

def main():
    try:
        stm32f4 = Serial_Stm32f4()
        while True:
            stm32f4.read()
            stm32f4.write()
    except KeyboardInterrupt:
        print " " 
        print "Sorry, Ctrl-C..."
    except SerialException:
                print"STM32F4 disconnected, cua cua"
   


if __name__ == '__main__':
	main()
	

