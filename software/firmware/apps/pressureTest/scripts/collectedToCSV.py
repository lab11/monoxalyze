#!/usr/bin/python

print("started")

import sys

# open file
inFile = open(sys.argv[1], "r")
outFile = open(sys.argv[2], "w")

def toPa(pressure):
	return pressure/40.96

def toC(temp):
	return temp/480.0 + 42.5000

def strToInt(s):
	i = int(s,16)
	if (i >= 2**15):
		i -= 2**16
	return i


#read to the start of memory we want
line = inFile.readline()
while(line[0:8] != "00002000"):
	#print("read " + line[0:8])
	line = inFile.readline()

#we got to where we want - process to csv
outFile.write("Pressure(pa),Temperature(C)\n")

#first line
initLine = line.split()
currPress = strToInt(initLine[2])

for i in range (3,6):
	if(initLine[i] == "FFFFFFFF"):
		outFile.write("na,na\n")
	else:
		currPress = currPress + strToInt(initLine[i][0:4])
		currTemp = strToInt(initLine[i][4:8])
		outFile.write(str(toPa(currPress)) + "," + str(toC(currTemp))+"\n")


#print("processed first line")

line = inFile.readline()

while(line != "" and line != "\n"):
	words = line.split()
	for i in range(2,6):
		if(words[i] == "FFFFFFFF"):
			outFile.write("na,na\n")
		else:
			currPress = currPress + strToInt(words[i][0:4])
			currTemp = strToInt(words[i][4:8])
			outFile.write(str(toPa(currPress)) + "," + str(toC(currTemp))+"\n")

	line = inFile.readline()

inFile.close()
outFile.close()
		


