#include <stdio.h>
#include <iostream>
#include <fstream>

#include <iomanip>
#include <string>
#include <chrono>
#include <thread>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/core/ocl.hpp>

class execution_timer final
{
public:
    execution_timer();
    ~execution_timer();

    /// @brief Time elapsed since construction or last reset_timer call
    double elapsed() const;
    void reset_timer();
private:
    typedef std::chrono::high_resolution_clock  high_res_clock;
    std::chrono::time_point<high_res_clock> start_time_;
};

execution_timer::execution_timer()
: start_time_(high_res_clock::now())
{
}

execution_timer::~execution_timer()
{
}

void execution_timer::reset_timer() {
    start_time_ = high_res_clock::now();
}

double execution_timer::elapsed() const {
    typedef std::chrono::duration<double,std::ratio<1>>   seconds;
    auto result = std::chrono::duration_cast<seconds>(
        high_res_clock::now() - start_time_
    ).count();

    return result;
}



void DisplayVisualTimer(double timervalue){
    cv::Mat m = cv::Mat::ones(100, 640, CV_32F);
    char str[200];
    sprintf(str,"%f",timervalue);
    cv::putText(m, str, cv::Point2f(80,80), cv::FONT_HERSHEY_PLAIN, 5,  cv::Scalar(0,0,255,255), 10);
    cv::imshow( "Display window", m );                   // Show our image inside it.
}

int main(int ac, char** av) {

  if ( ac != 3 ) // argc should be 3 for correct execution
    // We print argv[0] assuming it is the program name
    std::cout<<"usage: "<< av[0] <<" <max_seconds> <wait_key_milliseconds (1 and more)>\n";
  else {
    int max_seconds=std::stoi(av[1]);
    int wait_key_milliseconds=std::stoi(av[2]);

    std::cout << "Timer starts..." << std::endl;
    execution_timer thread_time;
        cv::startWindowThread();

    while(thread_time.elapsed() < max_seconds){
        DisplayVisualTimer(thread_time.elapsed());
        char key = (char)cv::waitKey(wait_key_milliseconds); //delay N millis, usually long enough to display and capture input
            switch (key) {
                case 'q':
                case 'Q':
                case 27: //escape key
                return 0;
                break;
                default:
                break;
            }
    }
  }
    return 0;
}
