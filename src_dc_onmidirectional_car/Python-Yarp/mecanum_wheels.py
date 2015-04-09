import serial   as s
from serial import SerialException
from numpy  import pi

class mecanum_wheels(object):

#Note: lines 64 to 94 where edited by Wilber

        #getting position from left joystick
        pg.event.get()
	x1=js.get_axis(0) #Left joystick left to right values -1.0 to 0.99
        y1=js.get_axis(1) #Left joystick up to down values -1.0 to 0.99
        y1_out=-y1 #Controller gives negative values for the upbutton
	x1_out=x1

	#getting position from right joystick
	x2=js.get_axis(2) #Left joystick left to right values -1.0 to 0.99
        y2=js.get_axis(3) #Left joystick up to down values -1.0 to 0.99
        y2_out=-y2 #Controller gives negative values for the upbutton
	x2_out=x2

	#getting speed from velocity vector
        speed= math.sqrt(x1_out**2+y1_out**2)

	#getting angle form velocity vector
        direction=math.atan2(y1_out,x1_out) #TODO make sure in which quadrant the angle is 

	#radius: diagonal distance between wheels split in two
	radius = 0.35/2 #carrito
	angularSpeed = math.sqrt(x2_out**2+y2_out**2) 
	
	#tangencial speed for the wheels	
	vTheta = angularSpeed*radius
	
	#speed for the four wheels
	v1=speed*math.sin(direction+(math.pi/4))+vTheta #speed upper-left wheel
        v2=speed*math.cos(direction+(math.pi/4))-vTheta #speed upper-right wheel
        v3=speed*math.cos(direction+(math.pi/4))+vTheta #speed down-left wheel
        v4=speed*math.sin(direction+(math.pi/4))-vTheta #speed down-right wheel

#	V1=y_out
#        V2=V1#V1-1000.0
#        V3=V1#V2-1000.0
#        V4=V1#-1000.0
        
        #sending position to stm32s

#Note: V1, V2, V3, V4 where changed to v1, v2, v3, v4 
