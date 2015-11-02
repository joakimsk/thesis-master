#include <iostream>
#include <stdio.h>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <algorithm>    // std::remove_if
#include <stdexcept>

#include "cheap_functions.h"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void clear() {
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    std::cout << "\x1B[2J\x1B[H";
}

bool invalidChar (char c) {
  return !(c>=0 && c <128);   
} 
void stripUnicode(std::string & str) { 
    str.erase(std::remove_if(str.begin(),str.end(), invalidChar), str.end());  
}

bool String2Int(const std::string& str, int& result) {
    try {
        std::size_t lastChar;
        result = std::stoi(str, &lastChar, 10);
        return lastChar == str.size();
    } catch (std::invalid_argument&) {
        std::cout << "String2Int-->invalid arg" << std::endl;
        return false;
    } catch (std::out_of_range&) {
        std::cout << "String2Int-->out of range" << std::endl;
        return false;
    }
}


bool String2Float(const std::string& str, float& result)
{
    try
    {
        std::size_t lastChar;
        result = std::stof(str, &lastChar);
        return lastChar == str.size();
    }
    catch (std::invalid_argument&)
    {
        std::cout << "String2Float-->invalid arg" << std::endl;
        return false;
    }
    catch (std::out_of_range&)
    {
        std::cout << "String2Float-->out of range" << std::endl;
        return false;
    }
}