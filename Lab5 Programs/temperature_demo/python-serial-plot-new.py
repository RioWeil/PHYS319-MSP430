#Use python 3.7 (Anaconda 3 and Spider)
# Use a command %matplotlib qt in the IPython console before 
# starting the program
#!/usr/bin/python3.7
import serial# for serial port
import matplotlib.pyplot as plt
import time

start=time.time()   


#define the serial port. Pick one:
#port = "/dev/ttyACM0"  #for Linux
port = "COM3" #For Windows Com5 or Com7 Find out which is connected to Launchpad
port = "/dev/tty.uart-EAFF4676258F275F" #For Mac?

times=[]
temps=[]

plt.figure()
plt.xlabel("Time (s)")
plt.ylabel("Temperature (F)")
#function that gets called when a key is pressed:
with serial.Serial(port,9600, timeout = 0.050) as ser:      # 50ms timeout
    print(ser.name)  # Print port name
    while(1): #loop forever 
        data = ser.read(1)              # look for a character from serial port - will wait for up to 50ms (specified above in timeout)
        if len(data) > 0:               # was there a byte to read?
            temp=ord(data)              # Takes a char, outputs unicode encoding as int
            t=time.time()-start         # calculates time as the current time (absolute) minus the start time
            print(t,temp)               # prints the time and temperature to terminal
            plt.scatter(t,temp,c='b')   # Plots time/temperatures as blue dots
            times.append(t)             # Appends time to list
            temps.append(temp)          # Append temperature to list
            plt.pause(0.05)             # Wait for 50ms

plt.show()