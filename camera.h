#ifndef __CAMERA_H_INCLUDED__
#define __CAMERA_H_INCLUDED__
#include <iostream>
#include "curl_easy.h"
#include "curl_form.h"
#include <map>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */

#include <boost/algorithm/string.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/core/ocl.hpp>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
bool String2Int(const std::string& str, int& result);
bool String2Float(const std::string& str, float& result);
void clear();

class Camera {
public:
    virtual void ShowInfo();
    virtual void CaptureFrame();
    cv::Mat GetPicture();
        // Constructors
    Camera();
        //Camera(std::string ip);
        // Destructor
    ~Camera();
protected:
          clock_t cpu_t_of_grab_picture_;
          cv::Mat grab_picture_;
          cv::VideoCapture capture_;
private:

};

class Axis6045 final : public Camera{
public:
    Axis6045();
    Axis6045(std::string ip);
    void CaptureFrame();
    void ShowInfo();
    void Login();
    void SetPassword();
    void SetPassword(std::string s_password);
    void RefreshPosition();
    void GrabFrame();
     void RetrieveFrame();
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

class Webcam final : public Camera{
public:
    Webcam();
    Webcam(uint device);
    void ShowInfo();
    void CaptureFrame();  
    void GrabFrame();
    void RetrieveFrame();
private:
    uint device_ = 0;
    
};

#endif