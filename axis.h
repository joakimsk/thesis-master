//axis_control.h

#ifndef __AXIS_H_INCLUDED__   // if x.h hasn't been included yet...
#define __AXIS_H_INCLUDED__   //   #define this so the compiler knows it has been included
#include <iostream>
#include "curl_easy.h"
#include "curl_form.h"

class Axis {
    public:
        void Test();
        void ShowInfo();
        void Login();
        void SetPassword();
        void Connect();
        // Constructors
        Axis();
        Axis(std::string ip);
        // Destructor
        ~Axis();

    private:
        std::string ip_ = "0.0.0.0";
        std::string pw_ = "";
        uint camera_ = 1;
};

#endif