#!/usr/bin/python

import cv2
import numpy as np
from matplotlib import pyplot as plt

src = cv2.imread('image.jpg',0)
rows,cols = src.shape

M = cv2.getRotationMatrix2D((cols/2,rows/2),90,1)

img = cv2.warpAffine(src,M,(cols,rows))

img2 = img.copy()
template = cv2.imread('template.jpg',0)
w, h = template.shape[::-1]

# All the 6 methods for comparison in a list
methods = ['cv2.TM_CCOEFF', 'cv2.TM_CCOEFF_NORMED', 'cv2.TM_CCORR',
            'cv2.TM_CCORR_NORMED', 'cv2.TM_SQDIFF', 'cv2.TM_SQDIFF_NORMED']

img = img2.copy()
method = eval('cv2.TM_CCOEFF')
meth = method

# Apply template Matching
res = cv2.matchTemplate(img,template,method)
min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)

# If the method is TM_SQDIFF or TM_SQDIFF_NORMED, take minimum
if method in [cv2.TM_SQDIFF, cv2.TM_SQDIFF_NORMED]:
	top_left = min_loc
else:
	top_left = max_loc
bottom_right = (top_left[0] + w, top_left[1] + h)

circ1 = (top_left[0] + 85, top_left[1] + 70)
circ2 = (top_left[0] + 125, top_left[1] + 70)
circ3 = (top_left[0] + 165, top_left[1] + 70)

cv2.rectangle(img,top_left, bottom_right, 255, 2)
cv2.circle(img, circ1, 15, 2)
cv2.circle(img, circ2, 15, 2)
cv2.circle(img, circ3, 15, 2)

plt.subplot(121),plt.imshow(res,cmap = 'gray')
plt.title('Matching Result'), plt.xticks([]), plt.yticks([])
plt.subplot(122),plt.imshow(img,cmap = 'gray')
plt.title('Detected Point'), plt.xticks([]), plt.yticks([])
plt.suptitle(meth)

plt.show()
