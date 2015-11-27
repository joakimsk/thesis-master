#include <iostream>
#include <stdio.h>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <chrono>
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <unistd.h>

#include "controller.h"
#include "cheap_functions.h"

Controller::Controller(void){
	std::cout << "Default constructor called." << std::endl;
}

Controller::~Controller(void){
    std::cout << "Destructing Controller object" << std::endl;
}