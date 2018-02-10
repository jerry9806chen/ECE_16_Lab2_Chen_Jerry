#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Fri Feb  9 23:20:11 2018

@author: jerry
"""

file_name_heads = ["accel", "gyro"]

for file_name_head in file_name_heads:
    with open(file_name_head + "_response_analysis.txt", 'w') as analysis_file:
        with open(file_name_head + "_response_time.txt", 'r') as read_file:
            print(file_name_head + " Response analysis:")
            
            dataCount = 0
            totalTime = 0
            currTime = 0
            minTime = 0
            maxTime = 0
            
            reading = read_file.read()
            currentRead = reading
            
            #find the number of samples, total time spent gathering data, and min and max differences in times of sampling.
            while "time: " in currentRead:
                dataCount += 1
                currTime = float(currentRead[currentRead.find("time: ") + 6: currentRead.find("seconds")])
                totalTime += currTime
                currentRead = currentRead[currentRead.find("\n") + 1:]
                if dataCount == 1:
                    minTime = totalTime
                    maxTime = totalTime
                else:
                    if currTime > maxTime:
                        maxDiff = currTime
                    elif currTime < minTime:
                        minDiff = currTime
                periodStart = totalTime
                
            #calculate the mean
            meanTime = totalTime / dataCount
            
            currentRead = reading
            periodStart = 0
            periodEnd = 0
            sumOfDiffSq = 0
            currentSample = 0
            
            # find the sum of the squares of the differences between each period and the mean period to determine standard deviantion.
            while currentSample < dataCount:
                if "start: " in currentRead:
                    currentSample += 1
                    periodEnd = float(currentRead[currentRead.find("time: ") + 6: currentRead.find("seconds")])
                    currentRead = currentRead[currentRead.find("time: ") + 1:]
                    if currentSample == 1:
                        sumOfDiffSq += (periodEnd - meanTime) * (periodEnd - meanTime)
                    else:
                        sumOfDiffSq += (periodEnd - periodStart) * (periodEnd - periodStart)
                    periodStart = periodEnd
                else:
                    break
    
            #calculate standard deviation
            standDev = (sumOfDiffSq / dataCount)**(0.5)
            
            #print the mean time difference between samples
            nextLine = "Mean time difference: " + str(meanTime)
            print(nextLine)
            analysis_file.write(nextLine + "\n")
            
            #print the standard deviation of time difference between samples
            nextLine = "Standard deviation of time difference: " + str(standDev)
            print(nextLine)
            analysis_file.write(nextLine + "\n")
            
            #print the minimum time difference between samples
            nextLine = "Minimum time difference: " + str(minDiff)
            print(nextLine)
            analysis_file.write(nextLine + "\n")
            
            #print the maximum time difference between samples
            nextLine = "Maximum time difference: " + str(maxDiff)
            print(nextLine)
            analysis_file.write(nextLine + "\n")