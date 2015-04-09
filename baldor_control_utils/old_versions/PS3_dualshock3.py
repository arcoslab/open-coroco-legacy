#!/usr/bin/env python

import pygame, sys, time
from pygame.locals import *

pygame.init()
pygame.joystick.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()
#screen = pygame.display.set_mode((400,300))
#pygame.display.set_caption('Hello World')

interval = 0.01

# get count of joysticks=1, axes=27, buttons=19 for DualShock 3

joystick_count = pygame.joystick.get_count()
print("joystick_count")
print(joystick_count)
print("--------------")

numaxes = joystick.get_numaxes()
print("numaxes")
print(numaxes)
print("--------------")

numbuttons = joystick.get_numbuttons()
print("numbuttons")
print(numbuttons)
print("--------------")

loopQuit = False
raw_input("Enter to continue: ")  
while loopQuit == False:
    '''
	#test joystick axes
    outstr = ""
    for i in range(0,joystick.get_numaxes()):
        axis = joystick.get_axis(i)
        outstr = outstr + str(i) + ":" + str(axis) + "|"
    print(outstr)

	# test controller buttons
    outstr = ""
    for i in range(0,numbuttons):
            button = joystick.get_button(i)
            outstr = outstr + str(i) + ":"+str(button) + "|"
    print(outstr)
    '''
    for event in pygame.event.get():
        if event.type==QUIT:
            #if event.type == QUIT:
        	loopQuit = True
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                loopQuit = True
           	 
    	#other event tests, but polling seems to work better in main loop
    	if event.type == pygame.JOYBUTTONDOWN:
    		print("joy button down")
    	if event.type == pygame.JOYBUTTONUP:
    	    print("joy button up")
    	if event.type == pygame.JOYBALLMOTION:
    		print("joy ball motion")
    	# axis motion is movement of controller
    	#dominates events when used
    	if event.type == pygame.JOYAXISMOTION:
        	 print("joy axis motion")
    
	# pygame.display.update()
    time.sleep(interval)

pygame.quit()
sys.exit()




