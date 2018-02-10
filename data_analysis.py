#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Fri Feb  9 17:02:27 2018

@author: jerry
"""

file_name_heads = ["accel", "gyro", "analog"]

for file_name_head in file_name_heads:
    with open(file_name_head + "_data_analysis.txt", 'w') as analysis_file:
        with open(file_name_head + "_output.txt", 'r') as read_file:
            print(file_name_head + " Data analysis:")
            
            dataCount = 0
            totalTime = 0
            periodStart = 0
            minDiff = 0
            maxDiff = 0
            
            reading = read_file.read()
            currentRead = reading
            
            #find the number of samples, total time spent gathering data, and min and max differences in times of sampling.
            while dataCount < 5000:
                if "start: " in currentRead:
                    dataCount += 1
                    totalTime = int(currentRead[currentRead.find("start: ") + 7: currentRead.find("ms")])
                    currentRead = currentRead[currentRead.find("\n") + 1:]
                    if dataCount == 1:
                        minDiff = totalTime
                        maxDiff = totalTime
                    else:
                        if totalTime - periodStart > maxDiff:
                            maxDiff = totalTime - periodStart
                        elif totalTime - periodStart < minDiff:
                            minDiff = totalTime - periodStart
                    periodStart = totalTime
                else:
                    break
                
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
                    periodEnd = int(currentRead[currentRead.find("start: ") + 7: currentRead.find("ms")])
                    currentRead = currentRead[currentRead.find("\n") + 1:]
                    if currentSample == 1:
                        sumOfDiffSq += (periodEnd - meanTime) * (periodEnd - meanTime)
                    else:
                        sumOfDiffSq += (periodEnd - periodStart) * (periodEnd - periodStart)
                    periodStart = periodEnd
                else:
                    break
    
            #calculate standard deviation
            standDev = (sumOfDiffSq / dataCount)**(0.5)
            
            #print the number of missing samples
            nextLine = (5000 - dataCount) + " missing samples"
            print(nextLine)
            analysis_file.write(nextLine + "\n")
            
            #print the number of analyzed samples
            nextLine = dataCount + " analyzed samples"
            print(nextLine)
            analysis_file.write(nextLine + "\n")
            
            #print the mean time difference between samples
            nextLine = "Mean time difference: " + meanTime
            print(nextLine)
            analysis_file.write(nextLine + "\n")
            
            #print the standard deviation of time difference between samples
            nextLine = "Standard deviation of time difference: " + standDev
            print(nextLine)
            analysis_file.write(nextLine + "\n")
            
            #print the minimum time difference between samples
            nextLine = "Minimum time difference: " + minDiff
            print(nextLine)
            analysis_file.write(nextLine + "\n")
            
            #print the maximum time difference between samples
            nextLine = "Maximum time difference: " + maxDiff
            print(nextLine)
            analysis_file.write(nextLine + "\n")
         