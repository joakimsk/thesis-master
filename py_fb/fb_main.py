#!/usr/bin/env python
import cv2
#ssimport cv2.cv
import sys
import numpy as np
import random
import os
import math

CV_LOAD_IMAGE_COLOR = 1
# Fingerboard POC
# Written by Joakim Skjefstad (skjefstad.joakim@gmail.com) Autumn 2015
# Using OpenCV 2.4.12 for Python on OS X

def find_fingerboard(source):
    input_hsv = cv2.cvtColor(source, cv2.COLOR_BGR2HSV)

    hsv_fingerboard_lower = np.array([80,50,50])
    hsv_fingerboard_upper = np.array([170,255,255])
    hsv_fingerboard_mask = cv2.inRange(input_hsv, hsv_fingerboard_lower, hsv_fingerboard_upper)
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



def find_tubulars(input_image):
    input_hsv = cv2.cvtColor(input_image, cv2.COLOR_BGR2HSV)
    input_gray = cv2.cvtColor(input_image, cv2.COLOR_BGR2GRAY)
    hsv_gray_gblur = cv2.GaussianBlur(input_gray,(3,3),0)
    hsv_gray_edges = cv2.Canny(hsv_gray_gblur,50,250, 3)
    cv2.imshow('TubularsCanny',hsv_gray_edges)

    hsv_tubulars_lower = np.array([0,50,50])
    hsv_tubulars_upper = np.array([15,255,255])
    hsv_tubulars_mask = cv2.inRange(input_hsv, hsv_tubulars_lower, hsv_tubulars_upper)
    #cv2.dilate(src=hsv_tubulars_mask, kernel=None, dst=hsv_tubulars_mask, iterations=1)
    cv2.erode(src=hsv_tubulars_mask, kernel=None, dst=hsv_tubulars_mask, iterations=1)
    cv2.imshow('TubularMask',hsv_tubulars_mask)
    minimum_tubulars_area = 100
    maximum_tubulars_area = 20000
    hsv_tubulars_res = cv2.bitwise_and(input_image,input_image, mask= hsv_tubulars_mask)

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

#    lines = cv2.HoughLinesP(hsv_tubulars_mask,5,np.pi/180,threshold=10, minLineLength=50, maxLineGap=4)
 #   if lines is not None:
  #      linmemat = source
   #     for x1,y1,x2,y2 in lines[0].tolist():
    #        cv2.line(linmemat,(x1,y1),(x2,y2),(0,255,0),2)
    #    cv2.imshow('HoughLines',linmemat)
    #else:
    #    print "no lines found in fingerboard!"

    params = cv2.SimpleBlobDetector_Params()

    # Only pick dark blobs
    params.blobColor = 0

    # Change thresholds
    params.minThreshold = 0
    params.maxThreshold = 500

    # Filter by Area.
    params.filterByArea = True
    params.minArea = 200
    params.maxArea = 12000 # 1200 ok for small

    # Filter by Circularity
    params.filterByCircularity = True
    params.minCircularity = 0.7
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
    blobs = detector.detect(hsv_gray_gblur)
    return blobs

def find_fingers(input_image):
    input_hsv = cv2.cvtColor(input_image, cv2.COLOR_BGR2HSV)

    hsv_fingers_lower = np.array([15,50,50])
    hsv_fingers_upper = np.array([40,255,255])
    hsv_fingers_mask = cv2.inRange(input_hsv, hsv_fingers_lower, hsv_fingers_upper)
    #cv2.dilate(src=hsv_fingers_mask, kernel=None, dst=hsv_fingers_mask, iterations=1)
    cv2.imshow('FingersMask',hsv_fingers_mask)
    minimum_fingers_area = 50
    maximum_fingers_area = 200
    hsv_fingers_res = cv2.bitwise_and(input_image,input_image, mask= hsv_fingers_mask)
    fingers_contours, fingers_hierarchy = cv2.findContours(hsv_fingers_mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    nr_potential_fingers = 0
    for contour in fingers_contours:
        area = cv2.contourArea(contour)
        if area >= minimum_fingers_area and area <= maximum_fingers_area :
            nr_potential_fingers += 1
            cv2.drawContours(hsv_fingers_res, [contour], 0, (0,255,255), 2) 
            cv2.drawContours(source, [contour], 0, (0,255,255), 2)  
                #moment = cv2.moments(contour)
                #potential_glyphs.append(PotentialGlyph(nr, contour, approx_poly, area, moment))
    cv2.imshow('FingersRes',hsv_fingers_res)
    print "Number of potential fingers = ",nr_potential_fingers

if __name__ == "__main__":
    print "FB POC. Use ESC to exit program."
    print "Written by Joakim Skjefstad (skjefstad.joakim@gmail.com) Autumn 2015"
    print "Case of master thesis, M.Sc in Technical Cybernetics at NTNU, Norway."
    filename = "input.jpg"
    #filename = "a.png"
    source = cv2.imread(os.path.join('dataset',filename))
    if source is None:
        print "Failed, no image loaded!"
    h_source, w_source, c_source = source.shape
    print "File loaded", filename

    find_fingerboard(source)
    find_fingers(source)
    tubular_blobs = find_tubulars(source)
    for keypoint in tubular_blobs:
        cv2.circle(source, (int(round(keypoint.pt[0])),int(round(keypoint.pt[1]))), 20, color=(255,0,255), thickness=2, lineType=1)

    

    cv2.imshow('Output',source)
    cv2.imwrite("output.png", source)
    key = cv2.waitKey(0)
    if key == 27:
        exit(0)