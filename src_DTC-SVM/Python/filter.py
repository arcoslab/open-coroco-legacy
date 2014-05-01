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




from scipy.signal import butter, lfilter, filtfilt, freqz
import numpy as np
import matplotlib.pyplot as plt




#butterworth filter variables
sample_frequency=40000.0
nyquist_frequency=sample_frequency/2.0
butterworth_order=4.0
cutoff_frequency=2.0*np.pi*600.0
normalized_cutoff_frequency=cutoff_frequency/nyquist_frequency
butterworth_type='low'

#butterworth filter
b,a=butter(butterworth_order,normalized_cutoff_frequency,butterworth_type)

#LPF 1+T*WCUTOFF
#b=[(1.0/sample_frequency) ,  0.0]
#a=[(1.0+(1/sample_frequency)*cutoff_frequency) , -1.0]

#LPF 1-Wc*Ts
a_1=[1.0/(1.0-cutoff_frequency*1.0/sample_frequency)  ,   -1]
b_1=[1.0/sample_frequency , 0.0]

#pure integrator
b_pure=[1.0/sample_frequency , 0.0]
a_pure=[1.0 , -1.0]  

print "b: " + str(b)  
print "a: " + str(a)

#frequency response of butterworth filter
w_radians,h=freqz(b,a)
w_radians_pure,h_pure=freqz(b_pure,a_pure)
#print "w_radians: " + str(w_radians)
#print "h: " + str(h)

angles=np.rad2deg(np.unwrap(np.angle(h)))#angle of the complex argument
w_Hz=w_radians/np.pi*sample_frequency/2.0
h_decibels=20*np.log10(np.absolute(h))



angles_pure=np.rad2deg(np.unwrap(np.angle(h_pure)))#angle of the complex argument
w_Hz_pure=w_radians_pure/np.pi*sample_frequency/2.0
h_decibels_pure=20*np.log10(np.absolute(h_pure))


#print "angles: " + str(angles)
#print "w_Hz: " + str(w_Hz)
#print "h_decibels: " + str(h_decibels)


#plt.figure(num=1, figsize=(10, 6), dpi=300, facecolor='w', edgecolor='k') 
plt.figure(num=1, figsize=(10, 6),  facecolor='w', edgecolor='k') 
#plt.subplot(rows=2,columns=1,subplot_index=1)
plt.subplot(2,1,1)


plt.plot(w_Hz,h_decibels,linewidth=2,label='integrator+filter')
plt.plot(w_Hz_pure,h_decibels_pure,linewidth=2,label='pure integrator')
plt.title('Frequency response')
plt.ylabel("Magnitude [dB]")
#ax11=plt.axes([.16,.59,.2,.2])
plt.plot(cutoff_frequency/(2.0*np.pi),-3,'ro')
plt.legend()

plt.subplot(2,1,2)
plt.plot(w_Hz,angles,linewidth=2,label='integrator+filter')
plt.plot(w_Hz_pure,angles_pure,linewidth=2,label='pure integrator')
plt.title('Phase response')
plt.xlabel("Frequency [Hz]")
plt.ylabel("Phase [degrees]")
plt.plot(cutoff_frequency/(2.0*np.pi),-45.0,'ro')
plt.legend()
#ax1.grid();ax11.grid();ax2.grid();
plt.show()


#fig,(ax1,ax2)= plt.subplot(2,1,sharex=True, figsize=(10,6))
'''
ax1.plot(w_radians,h_decibels,linewidth=2)
ax1.set_title('Frequency response')
ax1.set_ylabel("Magnitude [dB]")
ax11=plt.axes([.16,.59,.2,.2])
ax11.plt(w_radians,h_decibles,linewidth=2)
ax1.plot(5,-3.01,'ro');ax11.plot(5,-3.01,'ro')
ax11.set_ylim([-6,0]),ax11.set_xlim([0,10])
ax2.plot(w_radians,angles,linewidth=2)
ax2.set_title('Phase response')
ax2.set_xlabel("Frequency [Hz]"); ax2.set_ylabel("Phase [degrees]")
ax2.plot(5,-180,'ro')
ax1.grid();ax11.grid();ax2.grid();
'''

plt.close()
