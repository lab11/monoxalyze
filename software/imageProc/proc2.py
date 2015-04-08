#!/usr/bin/python

import cv2
import numpy as np
from matplotlib import pyplot as plt


cam = cv2.VideoCapture('vid.mp4')
im = cv2.imread('template.jpg')
frames = cam.get(cv2.cv.CV_CAP_PROP_FRAME_COUNT)
frame = 0

while 1:
	ret, src = cam.read()
	frame += 1

	cv2.imshow("w",src)
	cv2.waitKey(30)

