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

def find_fingerboard(hsv_input):
    hsv_fingerboard_lower = np.array([80,50,50])
    hsv_fingerboard_upper = np.array([170,255,255])
    hsv_fingerboard_mask = cv2.inRange(hsv_input, hsv_fingerboard_lower, hsv_fingerboard_upper)
    cv2.dilate(src=hsv_fingerboard_mask, kernel=None, dst=hsv_fingerboard_mask, iterations=2)
    cv2.imshow('FingerboardMask',hsv_fingerboard_mask)
    minimum_fingerboard_area = 1000
    maximum_fingerboard_area = 20000
    hsv_fingerboard_res = cv2.bitwise_and(source,source, mask= hsv_fingerboard_mask)
    fingerboard_contours, fingerboard_hierarchy = cv2.findContours(hsv_fingerboard_mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    nr_potential_fingerboards = 0
    for contour in fingerboard_contours:
        area = cv2.contourArea(contour)
        if area >= minimum_fingerboard_area and area <= maximum_fingerboard_area :
            nr_potential_fingerboards += 1
            cv2.drawContours(hsv_fingerboard_res, [contour], 0, (0,255,0), 1) 
            cv2.drawContours(source, [contour], 0, (0,255,0), 1)  
                #moment = cv2.moments(contour)
                #potential_glyphs.append(PotentialGlyph(nr, contour, approx_poly, area, moment))
    cv2.imshow('FingerboardRes',hsv_fingerboard_res)
    print "Number of potential fingerboards = ",nr_potential_fingerboards

def find_tubulars(hsv_input):
    hsv_tubulars_lower = np.array([0,50,50])
    hsv_tubulars_upper = np.array([15,255,255])
    hsv_tubulars_mask = cv2.inRange(hsv_input, hsv_tubulars_lower, hsv_tubulars_upper)
    #cv2.dilate(src=hsv_tubulars_mask, kernel=None, dst=hsv_tubulars_mask, iterations=1)
    cv2.erode(src=hsv_tubulars_mask, kernel=None, dst=hsv_tubulars_mask, iterations=1)
    cv2.imshow('TubularMask',hsv_tubulars_mask)
    minimum_tubulars_area = 100
    maximum_tubulars_area = 20000
    hsv_tubulars_res = cv2.bitwise_and(source,source, mask= hsv_tubulars_mask)
    tubulars_contours, tubulars_hierarchy = cv2.findContours(hsv_tubulars_mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    nr_potential_tubulars = 0
    for contour in tubulars_contours:
        area = cv2.contourArea(contour)
        if area >= minimum_tubulars_area and area <= maximum_tubulars_area :
            nr_potential_tubulars += 1
            cv2.drawContours(hsv_tubulars_res, [contour], 0, (0,0,255), 1) 
            cv2.drawContours(source, [contour], 0, (0,0,255), 1)  
                #moment = cv2.moments(contour)
                #potential_glyphs.append(PotentialGlyph(nr, contour, approx_poly, area, moment))
    cv2.imshow('TubularRes',hsv_tubulars_res)
    print "Number of potential tubulars = ",nr_potential_tubulars

    params = cv2.SimpleBlobDetector_Params()

    # Change thresholds
    params.minThreshold = 0
    params.maxThreshold = 500

    # Filter by Area.
    params.filterByArea = True
    params.minArea = 200
    params.maxArea = 1200

    # Filter by Circularity
    params.filterByCircularity = True
    params.minCircularity = 0.1
    params.maxCircularity = 1.0

    # Filter by Convexity
    params.filterByConvexity = True
    params.minConvexity = 0.7
    params.maxConvexity = 1.0

    # Filter by Inertia
    params.filterByInertia = True
    params.minInertiaRatio = 0.4
    params.maxInertiaRatio = 1.0

    # Create a detector with the parameters
    ver = (cv2.__version__).split('.')
    if int(ver[0]) < 3 :
        detector = cv2.SimpleBlobDetector(params)
    else : 
        detector = cv2.SimpleBlobDetector_create(params)
    blobs = detector.detect(hsv_input)
    return blobs

def find_fingers(hsv_input):
    hsv_fingers_lower = np.array([15,50,50])
    hsv_fingers_upper = np.array([40,255,255])
    hsv_fingers_mask = cv2.inRange(hsv_input, hsv_fingers_lower, hsv_fingers_upper)
    #cv2.dilate(src=hsv_fingers_mask, kernel=None, dst=hsv_fingers_mask, iterations=1)
    cv2.imshow('FingersMask',hsv_fingers_mask)
    hsv_fingers_res = cv2.bitwise_and(source,source, mask= hsv_fingers_mask)
    cv2.imshow('FingersRes',hsv_fingers_res)

if __name__ == "__main__":
    print "FB POC. Use ESC to exit program."
    print "Written by Joakim Skjefstad (skjefstad.joakim@gmail.com) Autumn 2015"
    print "Case of master thesis, M.Sc in Technical Cybernetics at NTNU, Norway."
    filename = "cam4180115_1421589601.jpg"
    source = cv2.imread(os.path.join('dataset',filename))
    if source is None:
        print "Failed, no image loaded!"
    h_source, w_source, c_source = source.shape
    print "File loaded", filename

    source_gray = cv2.cvtColor(source, cv2.COLOR_BGR2GRAY)
    source_hsv = cv2.cvtColor(source, cv2.COLOR_BGR2HSV)

    find_fingerboard(source_hsv)
    tubular_blobs = find_tubulars(source_hsv)
    for keypoint in tubular_blobs:
        cv2.circle(source, (int(round(keypoint.pt[0])),int(round(keypoint.pt[1]))), 20, color=(255,0,255), thickness=2, lineType=1)

    cv2.imshow('Output',source)
    key = cv2.waitKey(0)
    if key == 27:
        exit(0)