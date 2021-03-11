#!/usr/bin/python2.7
import numpy as np # for arrays, numerical processing
import csv
import matplotlib.pyplot as plt

model = True # Set to true if want to plot with model, false if you just want to plot the data. 


## READS IN MEASURED DATA
imes = []
dists = []
start = 2.75
end = 4
yerror = 0.5/np.sqrt(3)

with open("data.csv") as csvData:
	reader = csv.reader(csvData)
	for row in reader:
		if (start < float(row[0]) < end):
			times.append(float(row[0]))
			dists.append(float(row[1]))

# MODEL PARAMETERS
rho = 1.1839       # Density of air
A = 0.06237        # Cross-sectional area of object
cd = 1.28          # Drag constant for object; 1.28 for lamina with normal parallel to the direction of motion
m = 0.005*5        # mass of objects (5 papers of 5g each)
g = 9.81           # gravitational acceleration

# ADJUSTABLE
starttime = 3.2    # Start of the fall of the object
endtime = 3.8      # End of the fall of the object
vinit = 0          # Initial velocity (zero if dropped from rest)
yinit = 0.82       # Initial y-position of the object
b = 0.06           # Laminar drag coefficient b
dt = 0.01          # Time step of simulation

# SET INITIAL VALUES
t = starttime
y = yinit
v = vinit

tlist = []
ylist = []


# EULER METHOD SIMULATION
while t < endtime and y > 0:  # Keeps running until endtime or if object modelled to hit ground
	tlist.append(t)
	ylist.append(y*100)       # At each step, appends time and the height (in cm)
	a = - g + (b * (-v) / m) + (0.5 * rho * v**2 * cd * A / m) # Net forces in y is sum of gravity, laminar drag, turbulent drag
	v += a*dt                 # Update velocity
	y += v*dt                 # Update position
	t += dt                   # Update time


# PLOTTING
plt.errorbar(times, dists, yerr = yerror, xerr = None, fmt = "bo", label = "Data")
if model:
	plt.plot(tlist, ylist, "r-", label = "Simulation")
plt.legend(loc="best")
plt.title("Distance of papers from ultrasonic sensor vs. time with model")
plt.xlabel("Time (s)")
plt.ylabel("Distance of papers from ultrasonic sensor (cm)")
plt.show()