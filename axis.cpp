#include <iostream>
#include <stdio.h>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <chrono>
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>


#include "axis.h"

using curl::curl_easy;
using curl::curl_form;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool invalidChar (char c) 
{  
    return !(c>=0 && c <128);   
} 
void stripUnicode(string & str) 
{ 
    str.erase(remove_if(str.begin(),str.end(), invalidChar), str.end());  
}

bool String2Int(const std::string& str, int& result)
{
    try
    {
        std::size_t lastChar;
        result = std::stoi(str, &lastChar, 10);
        return lastChar == str.size();
    }
    catch (std::invalid_argument&)
    {
        std::cout << "String2Int-->invalid arg" << std::endl;
        return false;
    }
    catch (std::out_of_range&)
    {
        std::cout << "String2Int-->out of range" << std::endl;
        return false;
    }
}


Axis::Axis(void){
	std::cout << "Default constructor called." << std::endl;
	ShowInfo();
}

Axis::Axis(std::string ip){
	std::cout << "Overloaded constructor called." << std::endl;
	ip_ = ip;
    ShowInfo();
}

Axis::~Axis(void){
    std::cout << "Destructing Axis object" << std::endl;
}

void Axis::ShowInfo(){
	std::cout << "### Axis Information ###";
    std::cout << "Target CCTV-IP: " << ip_ << std::endl;
}


void Axis::SetPassword(){
    // Get password
    std::cout << "Enter root password for AXIS camera:" << std::endl;
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

void Axis::Connect(){
	    // Create a stream object
    std::ostringstream str;
    // Create a writer object, passing the stream object.
    curl_writer<ostringstream> writer(&str);
    curl_form form;
    curl_easy easy(writer);

    curl_pair<CURLformoption,string> name_form(CURLFORM_COPYNAME,"user");
    curl_pair<CURLformoption,string> name_cont(CURLFORM_COPYCONTENTS,"you username here");
    curl_pair<CURLformoption,string> pass_form(CURLFORM_COPYNAME,"passw");
    curl_pair<CURLformoption,string> pass_cont(CURLFORM_COPYCONTENTS,"your password here");

	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
	
      // Current ms used for control input to camera using continous motion
      //std::cout << ms.count() << std::endl;

	// Virker ikke om man ikke allerede er logget inn...?
	//std::string query_string = "?camera=" + std::to_string(camera_) + "&continouspantiltmove=15.0,15.0&timestamp=" + std::to_string(ms.count());

	// Virker ikke om man ikke allerede er logget inn...?
	//std::string query_string = "?query=position,limits&camera=" + std::to_string(camera_) + "&html=no&timestamp=" + std::to_string(ms.count());

    //std::string query_string = "?camera=" + std::to_string(camera_) + "&pan=45&timestamp=" + std::to_string(ms.count());
    //std::string query_string = "?query=position,limits&camera=" + std::to_string(camera_) + "&html=no&timestamp=" + std::to_string(ms.count());
    
    std::string query_string = "?query=position,limits&camera=" + std::to_string(camera_);

    //std::string readBuffer; CURLcode
   std::string  res;
 vector<string> tokens;
    try {
       form.add(name_form,name_cont);
       form.add(pass_form,pass_cont);


       easy.add(curl_pair<CURLoption,long>(CURLOPT_VERBOSE,0L));
      //readBuffer.clear();
      // easy.add(curl_pair<CURLoption,string>(CURLOPT_WRITEFUNCTION, WriteCallback));
       // easy.add(curl_pair<CURLoption,string>(CURLOPT_WRITEDATA, &readBuffer));

       easy.add(curl_pair<CURLoption,string>(CURLOPT_URL,"http://129.241.154.24/axis-cgi/com/ptz.cgi" + query_string));
       easy.add(curl_pair<CURLoption,long>(CURLOPT_FOLLOWLOCATION,1L));
       easy.add(curl_pair<CURLoption,string>(CURLOPT_USERAGENT,"Mozilla/4.0"));
       easy.add(curl_pair<CURLoption,long>(CURLOPT_HTTPAUTH,CURLAUTH_DIGEST)); // Bitmask for MD5 Digest
       easy.add(curl_pair<CURLoption,string>(CURLOPT_USERNAME,"root"));
       easy.add(curl_pair<CURLoption,string>(CURLOPT_PASSWORD,pw_));
       easy.add(curl_pair<CURLoption,long>(CURLOPT_NOBODY,0L)); // This removed BODY from response, may be ok when sending commands! Not when getting information.
       easy.add(curl_pair<CURLoption,long>(CURLOPT_TCP_KEEPALIVE,0L)); // DISABLE KEEPALIVE PROBES
       easy.add(curl_pair<CURLoption,long>(CURLOPT_TCP_KEEPIDLE,20L)); 
      easy.add(curl_pair<CURLoption,long>(CURLOPT_TCP_KEEPINTVL,10L));
    
      easy.perform();

    std::istringstream ss(str.str());
    std::string token;
    std::string SplitVec; // #2: Search for tokens

    uint count = 0;
    uint confirmed = 0;
    while(std::getline(ss, token, '\n')) {
      //std::cout << "<-" << token << std::endl;
      boost::algorithm::trim_right(token); // Remove newline and spaces at the end
      if (token.find('=') != string::npos) {
            std::string first_element = token.substr(0,token.find('='));
            std::string second_element = token.substr(token.find('=')+1);
            
            int outval = 0;
            if (String2Int(second_element, outval)){
              CameraLimits_.insert(std::pair<std::string, int>(first_element,outval));
              confirmed++;
            } else {
              std::cout << "element is not an integer: " << second_element << std::endl;
            }
            
      } else {
            std::cout  << "line is not valid key=value pair:" << token << std::endl;
      }
      count++;
    }
    std::cout << "# Limits read # Count of limits: " << count << " # Count of confirmed: " << confirmed << std::endl;

    } catch (curl_easy_exception error) {
       // If you want to get the entire error stack we can do:
       vector<pair<string,string>> errors = error.get_traceback();
       // Otherwise we could print the stack like this:
       error.print_traceback();
       // Note that printing the stack will erase it
    }


   

        //std::cout << str.str() << std::endl;    
        std::cout << "EEEH" << std::endl; 
}
