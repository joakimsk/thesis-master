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

Axis6045::Axis6045(std::string ip){
	std::cout << "Axis6045 IP overloaded constructor called." << std::endl;
	ip_ = ip;
   // ShowInfo();
}

Axis6045::Axis6045(void){
  std::cout << "Axis6045 default constructor called." << std::endl;
}

Webcam::Webcam(void){
  std::cout << "Webcam default constructor called." << std::endl;
}


Camera::~Camera(void){
  std::cout << "Destructing Axis object" << std::endl;
}

void Camera::ShowInfo(){
  std::cout << "### Camera Information ###" << std::endl;
  std::cout << "I am a CAMERA" << std::endl;
  std::cout << "### End of Camera Information ###" << std::endl;
}

void Camera::DisplayPicture(std::string window_name){
  cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
  imshow(window_name, grab_picture_);
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

void Webcam::ShowInfo(){
  std::cout << "### Camera-> WEBCAM Information ###" << std::endl;

}

void Camera::CaptureFrame(){
  std::cout << "### Camera->CaptureFrame NOT IMPLEMENTED ###" << std::endl;
}

cv::Mat Camera::GetPicture(){
  std::cout << "### Camera->GetPicture returns picture###" << std::endl;
  return grab_picture_;
}

void Webcam::GrabFrame(){
  std::cout << "### Webcam->GrabFrame ###" << std::endl;
  if(!capture_.isOpened()){
    std::cout << "### Webcam->GrabFrame-> CAPTURE WAS NOT OPEN. OPENING! ###" << std::endl;
    capture_.open(0);  
  }
  if(!capture_.grab()){
    std::cout << "Webcam->GrabFrame->  Can not grab images." << std::endl;
  }else{
    std::cout << "Webcam->GrabFrame->GRAB OK" << std::endl;
  }
}

void Axis6045::GrabFrame(){
  std::cout << "### Axis6045->GrabFrame ###" << std::endl;
  if(!capture_.isOpened()){
    std::cout << "### Axis6045->GrabFrame-> CAPTURE WAS NOT OPEN. OPENING! ###" << std::endl;
    capture_.open("http://ptz:ptz@129.241.154.24/mjpg/video.mjpg");  
  }
  if(!capture_.grab()){
    std::cout << "Axis6045->GrabFrame->  Can not grab images." << std::endl;
  }else{
    std::cout << "Axis6045->GrabFrame->GRAB OK" << std::endl;

  }
}

void Axis6045::RetrieveFrame(){
  std::cout << "### Axis6045->RetrieveFrame ###" << std::endl;
  if(!capture_.retrieve(grab_picture_)){
    std::cout << "Axis6045->RetrieveFrame->  Can not retrieve image." << std::endl;
  }else{
    std::cout << "Axis6045->RetrieveFrame->Retrieved OK" << std::endl;

  }
}

void Webcam::RetrieveFrame(){
  std::cout << "### Webcam->RetrieveFrame ###" << std::endl;
  if(!capture_.retrieve(grab_picture_)){
    std::cout << "Webcam->RetrieveFrame->  Can not retrieve image." << std::endl;
  }else{
    std::cout << "Webcam->RetrieveFrame->Retrieved OK" << std::endl;

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

void Webcam::CaptureFrame(){
  std::cout << "### Webcam->CaptureFrame ###" << std::endl;
  if(!capture_.isOpened()){
    std::cout << "### Webcam->CaptureFrame-> CAPTURE WAS NOT OPEN. OPENING! ###" << std::endl;
    capture_.open(0);  
  }
  capture_ >> grab_picture_;
  std::cout << "### Webcam->CaptureFrame DONE ###" << std::endl;
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

//////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////

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

      // Current ms used for control input to camera using continous motion
      //std::cout << ms.count() << std::endl;
	// Virker ikke om man ikke allerede er logget inn...?
	//std::string query_string = "?camera=" + std::to_string(camera_) + "&continouspantiltmove=15.0,15.0&timestamp=" + std::to_string(ms.count());
	// Virker ikke om man ikke allerede er logget inn...?
	//std::string query_string = "?query=position,limits&camera=" + std::to_string(camera_) + "&html=no&timestamp=" + std::to_string(ms.count());
    //std::string query_string = "?camera=" + std::to_string(camera_) + "&pan=45&timestamp=" + std::to_string(ms.count());
    //std::string query_string = "?query=position,limits&camera=" + std::to_string(camera_) + "&html=no&timestamp=" + std::to_string(ms.count());
    //std::string query_string = "?query=position&camera=" + std::to_string(camera_);

    try {
      easy.add(curl_pair<CURLoption,long>(CURLOPT_VERBOSE,0L));
      //readBuffer.clear();
      // easy.add(curl_pair<CURLoption,string>(CURLOPT_WRITEFUNCTION, WriteCallback));
      // easy.add(curl_pair<CURLoption,string>(CURLOPT_WRITEDATA, &readBuffer));
      easy.add(curl_pair<CURLoption,string>(CURLOPT_URL,"http://129.241.154.24/axis-cgi/com/ptz.cgi" + query_string + "&camera=" + std::to_string(camera_) + "&timestamp=" + std::to_string(ms.count())));
      easy.add(curl_pair<CURLoption,long>(CURLOPT_FOLLOWLOCATION,1L));
      easy.add(curl_pair<CURLoption,string>(CURLOPT_USERAGENT,"Mozilla/4.0"));
      easy.add(curl_pair<CURLoption,long>(CURLOPT_HTTPAUTH,CURLAUTH_DIGEST)); // Bitmask for MD5 Digest
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
