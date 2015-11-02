#ifndef __AXIS_H_INCLUDED__
#define __AXIS_H_INCLUDED__
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
        void RefreshPosition();

        // Constructors
        Axis();
        Axis(std::string ip);
        // Destructor
        ~Axis();

    private:
        void UpdatePosition_(std::string& html_response);
        bool QueryCamera_(const std::string query_string, std::string& response_string, bool nobody);
        
        // general camera
        std::string ip_ = "0.0.0.0";
        std::string pw_ = "";
        std::string usr_="";

        // position
        float pan_ = 0.0;
        float tilt_ = 0.0;
        uint zoom_ = 0;
        uint iris_ = 0;
        uint focus_ = 0;
        bool autofocus_ = false;
        bool autoiris_ = false;
        uint camera_ = 1;

        // limits
        int min_pan_ = 0;
        int max_pan_ = 0;
        uint min_tilt_ = 0;
        uint max_tilt_ = 0;
        uint min_zoom_ = 0;
        uint max_zoom_ = 0;
        uint min_iris_ = 0;
        uint max_iris_ = 0;
        uint min_focus_ = 0;
        uint max_focus_ = 0;
        uint min_field_angle_ = 0;
        uint max_field_angle_ = 0;
        uint min_brightness_ = 0;
        uint max_brightness_ = 0;

        static constexpr const char* QueryPosition = "some useful string constant";
        static constexpr const char* QueryLimits = "some useful string constant";
};

#endif