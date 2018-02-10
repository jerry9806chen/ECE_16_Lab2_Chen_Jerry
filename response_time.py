#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Thu Feb  8 14:41:14 2018

@author: jerry
"""

import serial

# this port address is for the serial Arduino port
SERIAL_PORT = '/dev/cu.usbmodem1421'
#the same rate used on the Arduino
SERIAL_RATE = 115200

def main():
    # Set up the serial connection.
    ser = serial.Serial(SERIAL_PORT, SERIAL_RATE, timeout=1)
    
    # Read and print the prompt from Arduino
    reading = str(ser.readline().decode('utf-8'))
    print(reading)
    
    # Sends user response to prompt to Arduino.
    source = raw_input()
    ser.write(source.encode('ascii'))
    
    # Determines file to add new information to based on user selection.
    if "G" in source and "A" not in source:
        fileName = "gyro_response_time.txt"
    elif "A" in source and "G" not in source:
        fileName = "accel_response_time.txt"
    
    # Read the first line.
    reading = str(ser.readline().decode('utf-8'))
    
    # Prints out only when the "Response Time" is sent through Serial.
    while "Response" not in reading:
        reading += str(ser.readline().decode('utf-8'))
    print(reading)
    
    # Write/append the read information to the end of the file
    with open(fileName, 'a') as fo:
        fo.write(reading[reading.find("Response"):])
    
    # Close the serial connection.
    ser.close();
    
# Run the main function
if __name__ == "__main__":
    main()
        
        
        