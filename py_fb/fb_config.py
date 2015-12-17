#!/usr/bin/env python
import cv2
import cv2.cv
import sys
import numpy as np
import requests
import random
import os
import math

CV_LOAD_IMAGE_COLOR = 1
# Fingerboard POC
# Written by Joakim Skjefstad (skjefstad.joakim@gmail.com) Autumn 2015
# Using OpenCV 2.4.12 for Python on OS X

print "FB POC. Use ESC to exit program."
print "Written by Joakim Skjefstad (skjefstad.joakim@gmail.com) Autumn 2015"
print "Case of master thesis, M.Sc in Technical Cybernetics at NTNU, Norway."

UPDATE_WINDOW = False

def trackbar_callback(x):
    filename = "input.jpg"
    source = cv2.imread(os.path.join('dataset',filename))
    if source is None:
        print "Failed, no image loaded!"
    h_source, w_source, c_source = source.shape
    print "File loaded", filename
    source_gray = cv2.cvtColor(source, cv2.COLOR_BGR2GRAY)
    source_hsv = cv2.cvtColor(source, cv2.COLOR_BGR2HSV)

    hsv_lower = np.array([cv2.getTrackbarPos('hsv_lower_hue','HSVHue'),cv2.getTrackbarPos('hsv_lower_saturation','HSVHue'),cv2.getTrackbarPos('hsv_lower_value','HSVHue')])
    hsv_upper= np.array([cv2.getTrackbarPos('hsv_upper_hue','HSVHue'),cv2.getTrackbarPos('hsv_upper_saturation','HSVHue'),cv2.getTrackbarPos('hsv_upper_value','HSVHue')])

    print "hsv_upper",hsv_upper
    print "hsv_lower",hsv_lower

    mask = cv2.inRange(source_hsv, hsv_lower, hsv_upper)
    res = cv2.bitwise_and(source,source, mask= mask)
    cv2.imshow('HSVMask',mask)
    cv2.imshow('HSVHue',res)
    #gaussian_kernel = cv2.getTrackbarPos('gaussian_kernel','Gaussian')
    #if gaussian_kernel % 2 == 0:
     #   print "gaussian_kernel IS ODD, aok=", gaussian_kernel
    #else:
    #    gaussian_kernel = 3

    canny_max = cv2.getTrackbarPos('canny_max','CannyEdge')

    source_gray_gblur = cv2.GaussianBlur(source_gray,(3,3),0)
    cv2.imshow('Gaussian',source_gray_gblur)

    canny_min = cv2.getTrackbarPos('canny_min','CannyEdge')
    canny_max = cv2.getTrackbarPos('canny_max','CannyEdge')

    hough_rho = cv2.getTrackbarPos('hough_rho','HoughLines')
    hough_theta = cv2.getTrackbarPos('hough_theta','HoughLines')
    hough_threshold = cv2.getTrackbarPos('hough_threshold','HoughLines')
    hough_minLineLength = cv2.getTrackbarPos('hough_minLineLength','HoughLines')
    hough_maxLineGap = cv2.getTrackbarPos('hough_maxLineGap','HoughLines')

    #print "gaussian_kernel=",gaussian_kernel

    print "canny_min=",canny_min
    print "canny_max=",canny_max

    print "hough_rho=",hough_rho
    print "hough_theta=",hough_theta
    print "np.pi/hough_theta=",np.pi/hough_theta
    print "in degrees=",math.degrees(np.pi/hough_theta)
    print "hough_threshold=",hough_threshold
    print "hough_minLineLength",hough_minLineLength
    print "hough_maxLineGap",hough_maxLineGap

    source_gray_edges = cv2.Canny(source_gray_gblur,canny_min,canny_max, 3)
    cv2.imshow('CannyEdge',source_gray_edges)
    #key = cv2.waitKey(0)
    alines = None
    lines = cv2.HoughLinesP(source_gray_edges,hough_rho,np.pi/hough_theta,threshold=hough_threshold, minLineLength=hough_minLineLength, maxLineGap=hough_maxLineGap)

   # lines = cv2.HoughLinesP(edges, rho=1, theta=np.pi/180, threshold=10, lines=alines, minLineLength=200, maxLineGap=2)
#[0].tolist()
    if lines is not None:
        for x1,y1,x2,y2 in lines[0].tolist():
            cv2.line(source,(x1,y1),(x2,y2),(0,255,0),2)
        cv2.imshow('HoughLines',source)
    else:
        print "no lines found!"
    key = cv2.waitKey(0)
    if key == 27:
        exit(0)
        cv2.destroyAllWindows()
    pass



# Target specification
RESIZE_TO_WIDTH = 600.0

#target_list.append(TARGET)

cross = []

tick_frequency = cv2.getTickFrequency()
tick_at_init = cv2.getTickCount()
print "TICK FREQUENCY"
print "Ticks per second", tick_frequency
print "Ticks at init", tick_at_init



cv2.namedWindow('HSVHue',cv2.WINDOW_AUTOSIZE)
cv2.createTrackbar('hsv_lower_hue','HSVHue',0,255,trackbar_callback)
cv2.createTrackbar('hsv_upper_hue','HSVHue',0,255,trackbar_callback)
cv2.createTrackbar('hsv_lower_saturation','HSVHue',0,255,trackbar_callback)
cv2.createTrackbar('hsv_upper_saturation','HSVHue',0,255,trackbar_callback)
cv2.createTrackbar('hsv_lower_value','HSVHue',0,255,trackbar_callback)
cv2.createTrackbar('hsv_upper_value','HSVHue',0,255,trackbar_callback)

# Found by tuning parameters from test image
# HSV Blue Fingerboard: 80 - 170
# HSV Yellow Fingers: 15-40
# HSV Yellowish Tubulars: 0-15

cv2.setTrackbarPos('hsv_lower_hue','HSVHue',80)
cv2.setTrackbarPos('hsv_upper_hue','HSVHue',170)
cv2.setTrackbarPos('hsv_lower_saturation','HSVHue',50)
cv2.setTrackbarPos('hsv_upper_saturation','HSVHue',255)
cv2.setTrackbarPos('hsv_lower_value','HSVHue',50)
cv2.setTrackbarPos('hsv_upper_value','HSVHue',255)

cv2.namedWindow('Gaussian',cv2.WINDOW_AUTOSIZE)
cv2.createTrackbar('gaussian_kernel','Gaussian',1,13,trackbar_callback)

cv2.namedWindow('CannyEdge',cv2.WINDOW_AUTOSIZE)
cv2.createTrackbar('canny_min','CannyEdge',0,255,trackbar_callback)
cv2.createTrackbar('canny_max','CannyEdge',0,255,trackbar_callback)

cv2.setTrackbarPos('canny_min','CannyEdge',30)
cv2.setTrackbarPos('canny_max','CannyEdge',130)


cv2.namedWindow('HoughLines',cv2.WINDOW_AUTOSIZE)
cv2.createTrackbar('hough_rho','HoughLines',1,255,trackbar_callback)
cv2.createTrackbar('hough_theta','HoughLines',1,180,trackbar_callback)
cv2.createTrackbar('hough_threshold','HoughLines',1,255,trackbar_callback)
cv2.createTrackbar('hough_minLineLength','HoughLines',1,180,trackbar_callback)
cv2.createTrackbar('hough_maxLineGap','HoughLines',1,255,trackbar_callback)

cv2.setTrackbarPos('hough_rho','HoughLines',1)
cv2.setTrackbarPos('hough_theta','HoughLines',30)
cv2.setTrackbarPos('hough_threshold','HoughLines',5)
cv2.setTrackbarPos('hough_minLineLength','HoughLines',120)
cv2.setTrackbarPos('hough_maxLineGap','HoughLines',7)

trackbar_callback(0)
key = cv2.waitKey(0)
if key == 27:
    exit(0)
