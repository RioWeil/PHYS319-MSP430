#!/usr/bin/python2.7
import serial # for serial port
import numpy as np # for arrays, numerical processing

#define the serial port. Pick one:
#port = "/dev/ttyACM0"  #for Linux
port = "/dev/tty.uart-EAFF4676258F275F" #For Mac?
with serial.Serial(port,9600,timeout = 0.050) as ser:
    print(ser.name)
    while(1): #loop forever
        data = ser.read(1) # look for a character from serial port - will wait for up to 50ms (specified above in timeout)
        if len(data) > 0: #was there a byte to read?
            print(ord(data))