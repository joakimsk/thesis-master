#!/bin/bash
g++ -std=c++11 main.cpp -L/home/joakimsk/github/curlcpp/build/src/ -L/usr/local/lib/ -I/home/joakimsk/github/curlcpp/include/ -lcurlcpp -lcurl -lOpenCL -lopencv_core -lopencv_highgui -lopencv_imgcodecs
