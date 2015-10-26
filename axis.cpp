#include <iostream>
#include <stdio.h>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <chrono>

#include "axis.h"

using curl::curl_easy;
using curl::curl_form;

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
    ostringstream str;
    // Create a writer object, passing the stream object.
    curl_writer<ostringstream> writer(&str);
    curl_form form;
    curl_easy easy(writer);

    curl_pair<CURLformoption,string> name_form(CURLFORM_COPYNAME,"user");
    curl_pair<CURLformoption,string> name_cont(CURLFORM_COPYCONTENTS,"you username here");
    curl_pair<CURLformoption,string> pass_form(CURLFORM_COPYNAME,"passw");
    curl_pair<CURLformoption,string> pass_cont(CURLFORM_COPYCONTENTS,"your password here");

	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
	std::cout << ms.count() << std::endl;

	// Virker ikke om man ikke allerede er logget inn...?
	//std::string query_string = "?camera=" + std::to_string(camera_) + "&continouspantiltmove=15.0,15.0&timestamp=" + std::to_string(ms.count());

	// Virker ikke om man ikke allerede er logget inn...?
	std::string query_string = "?query=position,limits&camera=" + std::to_string(camera_) + "&html=no&timestamp=" + std::to_string(ms.count());


    try {
       form.add(name_form,name_cont);
       form.add(pass_form,pass_cont);


       //easy.add(curl_pair<CURLoption,long>(CURLOPT_VERBOSE,1L));

       easy.add(curl_pair<CURLoption,string>(CURLOPT_URL,"http://129.241.154.24/axis-cgi/com/ptz.cgi" + query_string));
       easy.add(curl_pair<CURLoption,long>(CURLOPT_FOLLOWLOCATION,1L));
       easy.add(curl_pair<CURLoption,string>(CURLOPT_USERAGENT,"Mozilla/4.0"));
       easy.add(curl_pair<CURLoption,long>(CURLOPT_HTTPAUTH,CURLAUTH_DIGEST)); // Bitmask for MD5 Digest
       easy.add(curl_pair<CURLoption,string>(CURLOPT_USERNAME,"root"));
       easy.add(curl_pair<CURLoption,string>(CURLOPT_PASSWORD,pw_));
       easy.add(curl_pair<CURLoption,long>(CURLOPT_NOBODY,1L));


       easy.perform();

    } catch (curl_easy_exception error) {
       // If you want to get the entire error stack we can do:
       vector<pair<string,string>> errors = error.get_traceback();
       // Otherwise we could print the stack like this:
       error.print_traceback();
       // Note that the printing the stack will erase it
    }
        std::cout << str.str() << std::endl;    
}