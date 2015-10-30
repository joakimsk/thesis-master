#!/bin/bash
# Mac
g++ -std=c++11 main.cpp axis.cpp -L/Users/joakimsk/Github/curlcpp/build/src -I/Users/joakimsk/Github/curlcpp/include -lcurlcpp -lcurl -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_videoio -lopencv_imgproc
# Ubuntu
# g++ -std=c++11 main.cpp axis.cpp -L/home/joakimsk/github/curlcpp/build/src/ -L/usr/local/lib/ -I/home/joakimsk/github/curlcpp/include/ -lcurlcpp -lcurl -lOpenCL -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_ocl -lopencv_videoio -lopencv_imgproc
