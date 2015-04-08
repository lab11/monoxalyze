#!/usr/bin/python

import cv2
import numpy as np
from matplotlib import pyplot as plt


cam = cv2.VideoCapture('vid.mp4')
frames = cam.get(cv2.cv.CV_CAP_PROP_FRAME_COUNT)
frame = 0

top_left = (0,0)

lastCode = ""
numCode = 0
onCode = 0
finalCode = ""
waiting = 0

while frame < frames:
	ret, src = cam.read()
	frame += 1

	img = cv2.cvtColor(src,cv2.COLOR_BGR2GRAY)	

	temp = cv2.imread('template.jpg',0)

	w, h = temp.shape[:2]
	M = cv2.getRotationMatrix2D((h/2,w/2+25),-90,0.8)
	template = cv2.warpAffine(temp,M,(h,w+50))

	template = template[0:h, 85:200]
	
	method = eval('cv2.TM_CCOEFF')
	meth = method
	# Apply template Matching
	res = cv2.matchTemplate(img,template,method)
	min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)

	top_left = max_loc
	bottom_right = (top_left[0] + w-30, top_left[1] + h-30)

	circ1 = (top_left[0] + 45, top_left[1] + 75)
	circ2 = (top_left[0] + 45, top_left[1] + 115)
	circ3 = (top_left[0] + 45, top_left[1] + 155)

	cv2.rectangle(img,top_left, bottom_right, 255, 2)
	cv2.circle(img, circ1, 15, 2)
	cv2.circle(img, circ2, 15, 2)
	cv2.circle(img, circ3, 15, 2)
	
	one  = (cv2.sumElems(img[circ1[1]-8:circ1[1]+8,circ1[0]-8:circ1[0]+8]))
	two = (cv2.sumElems(img[circ2[1]-8:circ2[1]+8,circ2[0]-8:circ2[0]+8]))
	three = (cv2.sumElems(img[circ3[1]-8:circ3[1]+8,circ3[0]-8:circ3[0]+8]))

	code = ""
	if(one[0] > 40000):
		code += "1"
	else:
		code += "0"

	if(two[0] > 40000):
		code += "1"
	else:
		code += "0"

	if(three[0] > 40000):
		code += "1"
	else:
		code += "0"

	if(code == "000"):
		pass
	else:
		if(code == lastCode and waiting == 0):
			numCode+=1
			if(numCode > 3):
				finalCode += code
				waiting = 1
		elif(code == lastCode and waiting == 1):
			pass
		elif(code != lastCode and waiting == 0):
			pass
		else:
			waiting = 0

	lastCode = code



	cv2.imshow("w",img)
	cv2.imshow("r",template)
	cv2.waitKey(1)

print(finalCode)
