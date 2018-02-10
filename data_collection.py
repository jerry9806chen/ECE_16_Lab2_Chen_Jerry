#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Tue Feb  6 17:41:53 2018

@author: jerry
"""

import serial

# this port address is for the serial Arduino port
SERIAL_PORT = '/dev/cu.usbmodem1421'
#the same rate used on the Arduino
SERIAL_RATE = 115200
#the number of times a set of samples has been printed
printCount = 0

def main():
    # connect to serial and send user input to request data collection from either one of the three sources.
    ser = serial.Serial(SERIAL_PORT, SERIAL_RATE, timeout=1)
    source = input("Please select an input: 1 (Accelerometer), 2 (Gyroscope), or 3 (Analog Pin)")
    ser.write(str(source).encode('ascii'))
 
    # select a file to write to depending on the selected input.
    if source == 1:
        fileName = "accel_output.txt"
    elif source == 2:
        fileName = "gyro_output.txt"
    elif source == 3:
        fileName = "analog_output.txt"
    
    # reads the first line
    reading = str(ser.readline().decode('utf-8'))
    printCount = 0
    
    # opens the selected file for writing
    fo = open(fileName, "w")
    
    # reads and prints the two lines in every 5000 samples and writes it to the selected file. Increments printCount for every printed line up to 10000.
    while printCount < 5000 * 2:
        if "\n" not in reading:
            reading += str(ser.readline().decode('utf-8'))
        else:
            endIndex = reading.index("\n")
            writeString = reading[0:endIndex]
            print(writeString)
            fo.write(writeString + "\n")
            reading = reading[endIndex + 1:]
            printCount += 1
            
    # Close the serial and file connections
    ser.close()
    fo.close()
    
# run the main() function.
if __name__ == "__main__":
    main()
        
        