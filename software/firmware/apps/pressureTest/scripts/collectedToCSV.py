#!/usr/bin/env

# open file
inFile = open("collect.out", "r")
outFile = open("collectOut.csv", "w")

#read to the start of memory we want
line = inFile.readline()
while(line[:7] != "00002000"):
	line = inFile.readline()

#we got to where we want - process to csv
outFile.write("Pressure,Temperature")

#first line
initLine = line.split()
currPress = int(initLine[2], 16)

currPress = currPress + int(initLine[3][:3],16)
currTemp = int(initLine[3][4:7],16)
outFile.write(str(currPress) + "," + str(currTemp))

currPress = currPress + int(initLine[4][:3],16)
currTemp = int(initLine[4][4:7],16)
outFile.write(str(currPress) + "," + str(currTemp))

currPress = currPress + int(initLine[5][:3],16)
currTemp = int(initLine[5][4:7],16)
outFile.write(str(currPress) + "," + str(currTemp))


while(line != "" && line != "\n"):
	words = line.split()
	for i in range(2,6):
		currPress = currPress + int(initLine[i][:3],16)
		currTemp = int(initLine[i][4:7],16)
		outFile.write(str(currPress) + "," + str(currTemp))

inFile.close()
outFile.close()
		


