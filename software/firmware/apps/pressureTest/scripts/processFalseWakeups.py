#!/usr/bin/python

import sys

#check if we have enough arguments
if(len(sys.argv) <5):
	print("use: ./proccessFalseWakeups *.csv MINREBASE MAXREBASE MINDELTA MAXDELTA")
	sys.exit(1)

#get a list from the csv file
csvList = None
import csv
with open(sys.argv[1], 'r') as f:
	reader = csv.reader(f)
	csvList = list(reader)



outFile = open("processedOut.csv" ,"w")
#outFile.write("rebase delta averageWakeups\n")

falseWakeups = 0

minRebase = int(sys.argv[2])
maxRebase = int(sys.argv[3])
minDelta = int(sys.argv[4])
maxDelta = int(sys.argv[5])

if(minRebase == 0):
	minRebase = 1

if(minDelta == 0):
	minDelta = 1

#test for all rebase times in the range
for rebase in range(minRebase,maxRebase):
	print("rebase: " + str(rebase))
	#test for all thresholds in the range	
	for delta in range(minDelta, maxDelta):
		#start at the beginning of the list
		pressure = 1

		#go until the end of the list
		while (pressure < len(csvList)):

			#get the base for the threshold
			basePressure = csvList[pressure][0]

			#error check
			if(basePressure == "na"):
				break

			#iterate over the rebase time and see if you crossed the threshold
			for start in range(pressure,pressure+rebase):
				if(pressure+rebase > len(csvList)):
					break
				if (csvList[start][0] == "na"):
					break
				if (float(csvList[start][0]) - float(basePressure)) > delta:
					falseWakeups += 1
					break

			#rebase again	
			pressure += rebase

		outFile.write(str(rebase) + " " + str(delta) + " " + str(falseWakeups) + "\n")
		falseWakeups = 0
	outFile.write("\n")


