#!/usr/bin/env python
import pygame
import math
# Define some colors
BLACK    = (   0,   0,   0)
WHITE    = ( 255, 255, 255)

# This is a simple class that will help us print to the screen
# It has nothing to do with the joysticks, just outputing the
# information.
class TextPrint:
    def __init__(self):
        self.reset()
        self.font = pygame.font.Font(None, 20)
        
    def printd (self, screen, textString):
        textBitmap = self.font.render(textString, True, BLACK)
        screen.blit(textBitmap, [self.x, self.y])
        self.y += self.line_height
      
    def reset(self):
        self.x = 10
        self.y = 10
        self.line_height = 15
      
    def indent(self):
        self.x += 10
      
    def unindent(self):
        self.x -= 10  
          
pygame.init() 

#Loop until the user clicks the close button.
 done = False
# Used to manage how fast the screen updates
clock = pygame.time.Clock()

# Initialize the joysticks
pygame.joystick.init()    
# Get ready to print

textPrint = TextPrint()

# -------- Main Program Loop -----------
while done==False:
    # EVENT PROCESSING STEP
    for event in pygame.event.get(): # User did something
        if event.type == pygame.QUIT: # If user clicked close
            done=True # Flag that we are done so we exit this loop
            
    # Possible joystick actions: JOYAXISMOTION JOYBALLMOTION JOYBUTTONDOWN JOYBUTTONUP JOYHATMOTION
        if event.type == pygame.JOYBUTTONDOWN:
            print("Joystick button pressed.")
            
      if event.type == pygame.JOYBUTTONUP:        
            print("Joystick button released.") 
                    
    # Get count of joysticks
    joystick_count = pygame.joystick.get_count()
       
    # For each joystick:
    for i in range(joystick_count):
        joystick = pygame.joystick.Joystick(i)
        joystick.init()  
             
    #Usually axis run in pairs, up/down for one, and left/right for the other
      
        axes = joystick.get_numaxes()
      
        for i in range( axes ):
            axis = joystick.get_axis( i )
                
        y=joystick.get_axis(3);
        x=joystick.get_axis(2);
        
        velocidad= math.sqrt(x**2+y**2);
        direccion=math.atan2(y,x);
     
        v1=-(velocidad*math.sin(direccion+(math.pi/4)));
        v2=velocidad*math.cos(direccion+(math.pi/4));
        v3=velocidad*math.cos(direccion+(math.pi/4));
        v4=-(velocidad*math.sin(direccion+(math.pi/4)));
        print v1
        print v2
        print v3
        print v4        
         
       # Limit to 20 frames per second
       clock.tick(20)    
       pygame.quit ()
