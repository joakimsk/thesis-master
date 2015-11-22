#!/usr/bin/env python
import cv2
import cv2.cv
import sys
import numpy as np
import requests
import random
import os

CV_LOAD_IMAGE_COLOR = 1
# Fingerboard POC
# Written by Joakim Skjefstad (skjefstad.joakim@gmail.com) Autumn 2015

print "FB POC. Use ESC to exit program."
print "Written by Joakim Skjefstad (skjefstad.joakim@gmail.com) Autumn 2015"
print "Case of master thesis, M.Sc in Technical Cybernetics at NTNU, Norway."

# Target specification
RESIZE_TO_WIDTH = 600.0

#target_list.append(TARGET)

cross = []

tick_frequency = cv2.getTickFrequency()
tick_at_init = cv2.getTickCount()
print "TICK FREQUENCY"
print "Ticks per second", tick_frequency
print "Ticks at init", tick_at_init

for filename in os.listdir('dataset'):
    source = cv2.imread(os.path.join('dataset',filename))
    if source is None:
        print "Failed, no image loaded!"
    h_source, w_source, c_source = source.shape

    cimg = cv2.cvtColor(source, cv2.COLOR_BGR2GRAY)
    #img_blur = cv2.GaussianBlur(img_gray,(3,3),0)

    circles = cv2.HoughCircles(cimg,cv2.cv.CV_HOUGH_GRADIENT,1,20,
                            param1=50,param2=30,minRadius=0,maxRadius=0)

    circles = np.uint16(np.around(circles))
    for i in circles[0,:]:
        # draw the outer circle
        cv2.circle(cimg,(i[0],i[1]),i[2],(0,255,0),2)
        # draw the center of the circle
        cv2.circle(cimg,(i[0],i[1]),2,(0,0,255),3)
    
    cv2.imshow('detected circles',cimg)


    key = cv2.waitKey(1)
    if key == 27:
        exit(0)
