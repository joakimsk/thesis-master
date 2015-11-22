#!/usr/bin/env python
import cv2
import sys
import numpy as np
import requests
import random
import os

import jsg

baseline = cv2.getTextSize('+', cv2.FONT_HERSHEY_PLAIN, 2, 2)
CV_LOAD_IMAGE_COLOR = 1

# Glyph tracking dry-run
# Based upon code written Autumn 2014
# Written by Joakim Skjefstad (skjefstad.joakim@gmail.com) Autumn 2015

print "Glyph-tracking proof of concept. Use ESC to exit program."
print "Written by Joakim Skjefstad (skjefstad.joakim@gmail.com) Autumn 2014, updated 2015"
print "Subcase of master thesis, M.Sc in Technical Cybernetics at NTNU, Norway."

# Target specification
TARGET = np.matrix('1 1 1 1 1; 1 0 1 0 1; 1 0 1 1 1; 1 0 0 0 1; 1 1 1 1 1')
RESIZE_TO_WIDTH = 600.0

target_list = [TARGET]
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
   
    r = RESIZE_TO_WIDTH / source.shape[1]
    dim = (int(RESIZE_TO_WIDTH), int(source.shape[0] * r))
    resized = cv2.resize(source, dim, interpolation = cv2.INTER_AREA)
    h_resized, w_resized, c_resized = resized.shape
    source = resized
    
    cv2.putText(source,'+', (w_resized/2-(baseline[0][0]/2),h_resized/2+(baseline[0][1]/2)), cv2.FONT_HERSHEY_PLAIN, 2, (0,0,255), 2)
    
    temp_img = jsg.preprocess(source)
    
    potential_glyphs = jsg.find_potential_glyphs(temp_img, 100.0)
    glyph_hit = False
    for glyph in potential_glyphs:
        print "Glyph eeeeee", glyph
        glyph.compute_glyph(source)
        if jsg.compare_glyphs(glyph.glyph_matrix,target_list):
            cv2.drawContours(source,[glyph.approx_poly],0,(0,255,0),1)
            cv2.circle(source,(glyph.cx,glyph.cy),2,(255,0,255),-1)
            #cv2.putText(source,str(glyph.nr), (glyph.cx,glyph.cy), cv2.FONT_HERSHEY_PLAIN, 2, (255,255,0), 2)
            delta_array = jsg.delta_to_center(source, glyph)
            glyph_hit = True
            #cv2.imshow('Roi',glyph.img_roi)
            #cv2.imshow('Otsu',glyph.img_roi_otsu)
    cv2.namedWindow('Source')
    for item in cross:
        cv2.putText(source,'+', (item[0]-(baseline[0][0]/2),item[1]+(baseline[0][1]/2)), cv2.FONT_HERSHEY_PLAIN, 2, (255,0,255), 2)
    cv2.putText(source, "src=%sx%s downscaled=%sx%s" % (w_source,h_source, w_resized, h_resized), (10, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255))
    cv2.imshow('Source',source)
    if glyph_hit:
        print filename," HIT"
    else:
        print "!",filename

    key = cv2.waitKey(1)
    if key == 27:
        exit(0)
