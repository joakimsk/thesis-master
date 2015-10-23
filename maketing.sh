#!/bin/bash
g++ -std=c++11 main.cpp -L/home/joakimsk/github/curlcpp/build/src/ -I/home/joakimsk/github/curlcpp/include/ -lcurlcpp -lcurl -lOpenCL
