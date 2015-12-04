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

#include "camera.h"
#include "cheap_functions.h"

using curl::curl_easy;
using curl::curl_form;

Camera::Camera(void){
	std::cout << "Default Camera constructor called." << std::endl;
}

Camera::~Camera(void){
  std::cout << "Destructing Axis object" << std::endl;
}

void Camera::CaptureFrame(){
  std::cout << "### Camera->CaptureFrame NOT IMPLEMENTED ###" << std::endl;
}

void Camera::OpenDevice(){
  std::cout << "### Camera->OpenDevice NOT IMPLEMENTED ###" << std::endl;
}

cv::UMat Camera::GetPicture(){
  std::cout << "### Camera->GetPicture returns picture###" << std::endl;
  return grab_picture_;
}

void Camera::GrabFrame(){
  if(!capture_.grab()){
    std::cout << "Camera->GrabFrame->  Can not grab images." << std::endl;
  }else{
    std::cout << "Camera->GrabFrame->GRAB OK" << std::endl;
  }
}

void Camera::DisplayPicture(){
  cv::namedWindow(window_name_, cv::WINDOW_AUTOSIZE);
  cv::imshow(window_name_, grab_picture_);
  std::cout << "Camera->DisplayPicture done" << std::endl;
}

void Camera::DisplayCvDebugPicture(){
  cv::namedWindow("CV Debug", cv::WINDOW_AUTOSIZE);
  cv::imshow("CV Debug", grab_cv_debug_picture_);
  std::cout << "Camera->DisplayCvDebugPicture done" << std::endl;
}

void Camera::SavePicture(){
  std::stringstream ss;
  ss << window_name_ << cpu_t_of_grab_picture_ << ".jpg";
  std::string filename = ss.str();
  cv::imwrite(filename, grab_picture_);
  std::cout << "Camera->SavePicture done" << std::endl;
}


void Camera::ShowInfo(){
  std::cout << "### Camera Information ###" << std::endl;
  std::cout << "I am a CAMERA" << std::endl;
  std::cout << "### End of Camera Information ###" << std::endl;
}

void Camera::SetWindowName(std::string window_name){
  window_name_ = window_name;
  std::cout << "Camera->SetWindowName window_name was set" << std::endl;
}

void Camera::FindGlyph(){
  std::cout << "Camera->FindGlyph->Started" << std::endl;
  cv::Mat IN;
    cv::UMat gpuIN;

  cv::UMat gpuBW;
  cv::UMat gpuHSV;
  cv::UMat gpuBlur;
    cv::UMat gpuEdge;

     // cv::UMat gpuRANGE;
  //cv::cvtColor(grab_picture_, gpuHSV, cv::COLOR_BGR2HSV);
//cv::inRange(gpuHSV, cv::Scalar(0,0,0),cv::Scalar(0,255,255), gpuRANGE);
//imshow("eeh",gpuRANGE);
//std::cout << gpuRANGE.type() << "DEPTH" << std::endl;
//cv::bitwise_and(grab_picture_,grab_cv_debug_picture_, gpuRANGE);

    IN = cv::imread("glyph_m1.png", cv::IMREAD_COLOR);
    //IN.copyTo(gpuIN);
  cv::cvtColor(grab_picture_, gpuBW, cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(gpuBW, gpuBlur, cv::Size(3,3), 0, 0);
  cv::Canny(gpuBlur, gpuEdge, 0, 100, 3, false);

vector<vector<cv::Point> > contours;
vector<cv::Vec4i> hierarchy;

  cv::findContours(gpuEdge, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
  for (size_t i = 0; i < contours.size(); i++){
    std::cout << contours[i] << std::endl;
  }

  cv::imshow("eeha", gpuEdge);

/*
for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                    fabs(contourArea(Mat(approx))) > 1000 &&
                    isContourConvex(Mat(approx)) )
                {
                    double maxCosine = 0;

                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( maxCosine < 0.3 )
                        squares.push_back(approx);
                }
            }
*/
}

Axis6045::Axis6045(std::string ip){
  std::cout << "Axis6045 IP overloaded constructor called." << std::endl;
  ip_ = ip;
   // ShowInfo();
}

Axis6045::Axis6045(void){
  std::cout << "Axis6045 default constructor called." << std::endl;
}

void Axis6045::ShowInfo(){
  std::cout << "### Camera-> AXIS Information ###" << std::endl;
  std::cout << "Target CCTV-IP: " << ip_ << std::endl;
  std::cout << "position->pan_=" << pan_ << std::endl;
  std::cout << "position->tilt_=" << tilt_ << std::endl;
  std::cout << "position->zoom_=" << zoom_ << std::endl;
  std::cout << "position->iris_=" << iris_ << std::endl;
  std::cout << "position->focus_=" << focus_ << std::endl;
  std::cout << "position->autofocus_=" << autofocus_ << std::endl;
  std::cout << "position->autoiris_=" << autoiris_ << std::endl;
  std::cout << "camera_=" << camera_ << std::endl;
}

void Axis6045::OpenDevice(){
  std::cout << "### Axis6045->OpenDevice ###" << std::endl;
  if(!capture_.isOpened()){
    std::cout << "### Axis6045->OpenDevice->Device not open, opening..." << std::endl;
    if(capture_.open("http://ptz:ptz@129.241.154.24/mjpg/video.mjpg")){
      std::cout << "### Axis6045->OpenDevice->Opened successfully" << std::endl;
    } else{
      std::cout << "### Axis6045->OpenDevice->Failed opening device" << std::endl;
    }
  } else {
    std::cout << "### Axis6045->OpenDevice->Device already opened" << std::endl;
  }
}

void Axis6045::RetrieveFrame(){
  std::cout << "### Axis6045->RetrieveFrame ###" << std::endl;
  if(grab_picture_.empty() == 1){
    std::cout << "grab_picture_ no image" << std::endl;
  }
  if(!capture_.retrieve(grab_picture_)){
    std::cout << "Axis6045->RetrieveFrame->  Can not retrieve image." << std::endl;
  } else {
    std::cout << "Axis6045->RetrieveFrame->Retrieved OK" << std::endl;
    if (size_of_capture_.height == 0 && size_of_capture_.width == 0){
      size_of_capture_ = grab_picture_.size();
      std::cout << "Axis6045->RetrieveFrame-> Size detected, width = " << size_of_capture_.width << " height = " << size_of_capture_.height;
    }
  }
}

void Axis6045::CaptureFrame(){
  std::cout << "### Axis6045->CaptureFrame ###" << std::endl;
  if(!capture_.isOpened()){
    std::cout << "### Axis6045->CaptureFrame-> CAPTURE WAS NOT OPEN. OPENING! ###" << std::endl;
    capture_.open("http://ptz:ptz@129.241.154.24/mjpg/video.mjpg");  
  }
  capture_ >> grab_picture_;
  std::cout << "### Axis6045->CaptureFrame DONE ###" << std::endl;
}

void Axis6045::SetPassword(){
    // Get password
  std::cout << "Enter ptz password for AXIS camera:" << std::endl;
  termios oldt;
  tcgetattr(STDIN_FILENO, &oldt);
  termios newt = oldt;
  newt.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  string s_password;
  getline(std::cin, s_password);
    //std::cout << s << endl;
  pw_ = s_password;
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  std::cout << "Password stored in instance." << std::endl;
}

void Axis6045::SetPassword(std::string s_password){
  pw_ = s_password;
  std::cout << "Password stored in instance." << std::endl;
}

void Axis6045::RefreshPosition(){
  std::string query_string = "?query=position";
  std::string response_string  = "";
  Axis6045::QueryCamera_(query_string, response_string, false);
  Axis6045::UpdatePosition_(response_string);
  std::cout << "RefreshPosition() finished." << std::endl;
}

void Axis6045::UpdatePosition_(std::string& html_response){
  std::istringstream ss(html_response);
  std::string token;
    std::string SplitVec; // #2: Search for tokens

    uint count = 0;

    while(std::getline(ss, token, '\n')) {
      std::cout << "<-" << token << std::endl;
      boost::algorithm::trim_right(token); // Remove newline and spaces at the end
      if (token.find('=') != string::npos) {
        std::string first_element = token.substr(0,token.find('='));
        std::string second_element = token.substr(token.find('=')+1);

        if(first_element == "pan"){
          String2Float(second_element, (float&)pan_);
        } else if (first_element == "tilt"){
          String2Float(second_element, (float&)tilt_);
        } else if (first_element == "zoom"){
          String2Int(second_element, (int&)zoom_);
        } else if (first_element == "iris"){
          String2Int(second_element, (int&)iris_);
        } else if (first_element == "focus"){
          String2Int(second_element, (int&)focus_);
        } else if (first_element == "autofocus"){
          if (second_element == "on"){
            autofocus_ = true;
          } else {
            autofocus_ = false;
          }
        } else if (first_element == "autoiris"){
          if (second_element == "on"){
            autoiris_ = true;
          } else {
            autoiris_ = false;
          }
        } else {
          std::cout << "Unknown position element!" << std::endl;  
        }
      } else {
        std::cout  << "line is not valid key=value pair:" << token << std::endl;
      }
      count++;
    }
    std::cout << "<- Count of position elements: " << count << std::endl;
  }

  bool Axis6045::QueryCamera_(const std::string query_string, std::string& response_string, bool nobody){
    std::ostringstream str;
    curl_writer<ostringstream> writer(&str);
    curl_easy easy(writer);
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    try {
      easy.add(curl_pair<CURLoption,long>(CURLOPT_VERBOSE,0L));
    /*
    readBuffer.clear();
    easy.add(curl_pair<CURLoption,string>(CURLOPT_WRITEFUNCTION, WriteCallback));
    easy.add(curl_pair<CURLoption,string>(CURLOPT_WRITEDATA, &readBuffer));
    */
    easy.add(curl_pair<CURLoption,string>(CURLOPT_URL,"http://129.241.154.24/axis-cgi/com/ptz.cgi" + query_string + "&camera=" + std::to_string(camera_) + "&timestamp=" + std::to_string(ms.count())));
    easy.add(curl_pair<CURLoption,long>(CURLOPT_FOLLOWLOCATION,1L));
    easy.add(curl_pair<CURLoption,string>(CURLOPT_USERAGENT,"Mozilla/4.0"));
    easy.add(curl_pair<CURLoption,long>(CURLOPT_HTTPAUTH,CURLAUTH_DIGEST)); 
    easy.add(curl_pair<CURLoption,string>(CURLOPT_USERNAME,"ptz"));
    easy.add(curl_pair<CURLoption,string>(CURLOPT_PASSWORD,pw_));
    if (nobody){
      easy.add(curl_pair<CURLoption,long>(CURLOPT_NOBODY,1L)); // This removed BODY from response, may be ok when sending commands! Not when getting information.
    } else {
      easy.add(curl_pair<CURLoption,long>(CURLOPT_NOBODY,0L));
    }
    easy.add(curl_pair<CURLoption,long>(CURLOPT_TCP_KEEPALIVE,0L)); // DISABLE KEEPALIVE PROBES
    easy.add(curl_pair<CURLoption,long>(CURLOPT_TCP_KEEPIDLE,20L)); 
    easy.add(curl_pair<CURLoption,long>(CURLOPT_TCP_KEEPINTVL,10L));
    easy.perform();
    response_string = str.str();
    return true;
  } catch (curl_easy_exception error) {
    // If you want to get the entire error stack we can do:
    vector<pair<string,string>> errors = error.get_traceback();
    // Otherwise we could print the stack like this:
    error.print_traceback();
    // Note that printing the stack will erase it
  }
  return false;
}

Webcam::Webcam(void){
  std::cout << "Webcam default constructor called." << std::endl;
}

Webcam::Webcam(uint device){
  std::cout << "Webcam spesific constructor called, device set" << device << std::endl;
  device_ = device;
}

void Webcam::RetrieveFrame(){
  std::cout << "### Webcam->RetrieveFrame ###" << std::endl;
  if(!capture_.retrieve(grab_picture_)){
    std::cout << "Webcam->RetrieveFrame->  Can not retrieve image." << std::endl;
  }else{
    std::cout << "Webcam->RetrieveFrame->Retrieved OK" << std::endl;
    if (size_of_capture_.height == 0 && size_of_capture_.width == 0){
      size_of_capture_ = grab_picture_.size();
      std::cout << "Webcam->RetrieveFrame-> Size detected, width = " << size_of_capture_.width << " height = " << size_of_capture_.height;
    }
  }
}

void Webcam::CaptureFrame(){
  std::cout << "### Webcam->CaptureFrame ###" << std::endl;
  if(!capture_.isOpened()){
    std::cout << "### Webcam->CaptureFrame-> CAPTURE WAS NOT OPEN. OPENING! ###" << std::endl;
    capture_.open(device_);  
  }
  capture_ >> grab_picture_;
  std::cout << "### Webcam->CaptureFrame DONE ###" << std::endl;
}

void Webcam::ShowInfo(){
  std::cout << "### Camera-> WEBCAM Information ###" << std::endl;

}

void Webcam::OpenDevice(){
  std::cout << "### Webcam->OpenDevice ###" << std::endl;
  if(!capture_.isOpened()){
    std::cout << "### Webcam->OpenDevice->Device not open, opening..." << std::endl;
    if(capture_.open(device_)){
      std::cout << "### Webcam->OpenDevice->Opened successfully" << std::endl;
    } else{
      std::cout << "### Webcam->OpenDevice->Failed opening device" << std::endl;
    }
  } else {
    std::cout << "### Webcam->OpenDevice->Device already opened" << std::endl;
  }
}