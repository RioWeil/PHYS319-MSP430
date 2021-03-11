#!/usr/bin/python2.7
import serial # for serial port
import numpy as np # for arrays, numerical processing
import time
import csv


#define the serial port. Pick one:
#port = "/dev/ttyACM0"  #for Linux
port = "/dev/tty.uart-EAFF4676258F275F" #For Mac?

filename = "name.csv"       # Filename for csv

timelimit = 5          # How long the program will run for.

f = open(filename, "w")  # Opens csv
writer = csv.writer(f)   # Creates writer for csv

now = 0
start = time.time()


with serial.Serial(port,9600,timeout = 0.050) as ser:
	print(ser.name)
	while(now < timelimit): #loop forever
		data = ser.read(1) # look for a character from serial port - will wait for up to 50ms (specified above in timeout)
		if len(data) > 0: #was there a byte to read?
			dist = ord(data)
			now = time.time() - start
			print(now, dist)
			row = [now, dist]
			writer.writerow(row)
	f.close()

