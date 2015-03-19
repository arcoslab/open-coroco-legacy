#!/usr/bin/python

# Numpy is a library for handling arrays (like data points)
import numpy as np

# Pyplot is a module within the matplotlib library for plotting
import matplotlib.pyplot as plt

# Create an array of 100 linearly-spaced points from 0 to 2*pi
#x = np.linspace(0,2*np.pi,100)
#y = np.sin(x)
x=[1,2,3,4]
y=[5,6,7,8]

# Create the plot
plt.plot(x,y)

# Save the figure in a separate file
plt.savefig('sine_function_plain.png')

# Draw the plot to the screen
plt.show()


