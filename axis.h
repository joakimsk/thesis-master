//axis_control.h

#ifndef __AXIS_H_INCLUDED__   // if x.h hasn't been included yet...
#define __AXIS_H_INCLUDED__   //   #define this so the compiler knows it has been included
#include <iostream>
#include "curl_easy.h"
#include "curl_form.h"
#include <map>
#include <boost/algorithm/string.hpp>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
bool String2Int(const std::string& str, int& result);
bool String2Float(const std::string& str, float& result);
void clear();
class Axis {
    public:
        void Test();
        void ShowInfo();
        void Login();
        void SetPassword();
        void Connect();
        void UpdatePosition(std::string& html_response);
        // Constructors
        Axis();
        Axis(std::string ip);
        // Destructor
        ~Axis();

    private:
        std::string ip_ = "0.0.0.0";
        std::string pw_ = "";
        std::string usr_="";
        std::map<std::string, int> CameraLimits_;

        float pan_ = 0.0;
        float tilt_ = 0.0;
        uint zoom_ = 0;
        uint iris_ = 0;
        uint focus_ = 0;
        bool autofocus_ = false;
        bool autoiris_ = false;
        uint camera_ = 1;
};

#endif