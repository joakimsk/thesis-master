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

#include "globals.h"
#include "camera.h"

/*
Auto CCTV Tracking with GPU-acceleration implementation
 Written by Joakim Skjefstad (skjefstad.joakim@gmail.com) Autumn 2015
 Using OpenCV 3.0.0 and C++11

IN ORDER TO USE OPENCL:
 OPENCV_OPENCL_DEVICE needs to be toggled, also!
 On Linux:

    ENABLE
export OPENCV_OPENCL_DEVICE=":GPU:0"

    DISABLE
export OPENCV_OPENCL_DEVICE="qqq"

 You also need to toggle this boolean:
*/




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

namespace {
    void inform_clocks(){
        std::cout << "system_clock" << std::endl;
        std::cout << std::chrono::system_clock::period::num << std::endl;
        std::cout << std::chrono::system_clock::period::den << std::endl;
        std::cout << "steady = " << std::boolalpha << std::chrono::system_clock::is_steady << std::endl << std::endl;
        
        std::cout << "high_resolution_clock" << endl;
        std::cout << std::chrono::high_resolution_clock::period::num << endl;
        std::cout << std::chrono::high_resolution_clock::period::den << endl;
        std::cout << "steady = " << std::boolalpha << std::chrono::high_resolution_clock::is_steady << std::endl << std::endl;

        std::cout << "steady_clock" << std::endl;
        std::cout << std::chrono::steady_clock::period::num << endl;
        std::cout << std::chrono::steady_clock::period::den << endl;
        std::cout << "steady = " << std::boolalpha << std::chrono::steady_clock::is_steady << std::endl << std::endl;
    }

    void inform_opencl(){
        std::cout << "inform_opencl(): Entry" << endl;
        std::cout << "# Checking capabilities" << endl;
        std::cout << "# haveOpenCL = " << cv::ocl::haveOpenCL() << endl;
        std::cout << "# haveAmdBlas = " << cv::ocl::haveAmdBlas() << endl;
        std::cout << "# haveAmdFft = " << cv::ocl::haveAmdFft() << endl;
        std::cout << "# haveSVM = " << cv::ocl::haveSVM() << endl;
        
        cv::ocl::Context ocl_context;
        if (!ocl_context.create(cv::ocl::Device::TYPE_GPU)){
            std::cerr << "Failed creating the OCL context." << endl;
        }
        std::cout << "# GPU devices detected = " << ocl_context.ndevices() << endl;
        for (int i = 0; i < ocl_context.ndevices(); i++){
            cv::ocl::Device device = ocl_context.device(i);
            cout << "name:              " << device.name() << endl;
            cout << "available:         " << device.available() << endl;
            cout << "imageSupport:      " << device.imageSupport() << endl;
            cout << "OpenCL_C_Version:  " << device.OpenCL_C_Version() << endl;
            cout << endl;
        }
        std::cout << "inform_opencl(): Exit" << endl;

        cv::ocl::Device(ocl_context.device(0));
    }
}

int main(int argc, char * const argv[]) {
    bool OCL = false;

    if ( argc != 2 ){ // argc should be 2 for correct execution
        // We print argv[0] assuming it is the program name
        std::cout<<"usage: "<< argv[0] <<" <use_opencl 1 or 0>\n";
        std::cout<<"example: "<< argv[0] <<" 1\n";
        return 0;
    } else {
        int x = std::atoi(argv[1]);
        if (x == 1){
            OCL = true;
        }
    }

    execution_timer timemagic;  

    auto chrono_main_start = std::chrono::steady_clock::now();
    inform_clocks();

    std::cout << "CLOCKS_PER_SEC=" << CLOCKS_PER_SEC << std::endl;
    std::cout << "Precision Ticks per second:" << std::chrono::high_resolution_clock::period::den << std::endl;

    // ALSO requires set OPENCV_OPENCL_RUNTIME=qqq to disable or = to enable
    if (OCL == true){
        cv::ocl::setUseOpenCL(true);
    } else {
        cv::ocl::setUseOpenCL(false);
    }

    inform_opencl();

            cv::waitKey(4000);

    Axis6045 ptzcam("129.241.154.24");
    //Webcam cam(0);

    ptzcam.SetPassword("ptz");
    ptzcam.RefreshPosition();
    ptzcam.ShowInfo();

    //cam.ShowInfo();
    std::string full_cycle_filename =  "full_cycle.log";
    std::string after_grab_filename =  "after_grab.log";

    if (OCL == true){
        full_cycle_filename = "ocl_full_cycle.log";
        after_grab_filename =  "ocl_after_grab.log";
    }

    std::ofstream full_cycle_file (full_cycle_filename);
    std::ofstream after_grab_file (after_grab_filename);

    full_cycle_file << "# X i" << " " << "Y ms" << std::endl;
    after_grab_file << "# X i" << " " << "Y ms" << std::endl;

    ptzcam.OpenDevice();
    //cam.OpenDevice();
    ptzcam.SetWindowName("PTZ");
    //cam.SetWindowName("Webcam");
    cv::startWindowThread();

    int cyclenr = 0;
    while(cyclenr <  1000){
        auto chrono_cycle_start = std::chrono::steady_clock::now();

        //std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
       // std::cout << ms << std::endl;

        ptzcam.RefreshPosition();
        ptzcam.ShowInfo();

        ptzcam.GrabFrame(); // Gather Frame data
        //cam.GrabFrame(); // Gather Frame data
        
        ptzcam.RetrieveFrame(); // Decode Frame data
        //cam.RetrieveFrame(); // Decode Frame data
        
        ptzcam.RefreshPosition();
        ptzcam.ShowInfo();

        auto chrono_cycle_after_grab = std::chrono::steady_clock::now();

        if (ptzcam.FindGlyph()){ // If we find a glyph
            cv::circle(ptzcam.grab_picture_,cv::Point(ptzcam.glyph_x_,ptzcam.glyph_y_), 4, cv::Scalar(255,0,255), -1, 8, 0 );
            std::cout << "We found a glyph!" << std::endl;
            std::cout << "Calculate delta X and delta Y" << std::endl;
           int ptz_middle_width = int(ptzcam.grab_picture_.cols/2);
           int ptz_middle_height = int(ptzcam.grab_picture_.rows/2);
            std::cout << "middle width and height is " << ptz_middle_width << ptz_middle_height << std::endl;

            cv::circle(ptzcam.grab_picture_,cv::Point(ptz_middle_width,ptz_middle_height), 4, cv::Scalar(255,0,0), -1, 8, 0 );
            int delta_x = ptzcam.glyph_x_-ptz_middle_width;
            int delta_y = ptz_middle_height-ptzcam.glyph_y_;
            float gain_x = 0.03;
            float gain_y = 0.03;
            std::cout << "######################### PTZ DELTA: x=" << delta_x << " y=" << delta_y << std::endl;
            if (abs(delta_x) > 5 || abs(delta_y) > 5 ){
                ptzcam.CommandCamera("?rpan="+std::to_string(delta_x*gain_x));
                ptzcam.CommandCamera("?rtilt="+std::to_string(delta_y*gain_y));
            }
        }


        //if (cam.FindGlyph()){ // If we find a glyph
            //cv::circle(ptzcam.grab_picture_,cv::Point(ptzcam.glyph_x_,ptzcam.glyph_y_), 4, cv::Scalar(255,0,255), -1, 8, 0 );
         //   std::cout << "CAM: We found a glyph!" << std::endl;
            //std::cout << "Calculate delta X and delta Y" << std::endl;
           //int ptz_middle_width = int(ptzcam.grab_picture_.cols/2);
           //int ptz_middle_height = int(ptzcam.grab_picture_.rows/2);
            //std::cout << "middle width and height is " << ptz_middle_width << ptz_middle_height << std::endl;

            //cv::circle(ptzcam.grab_picture_,cv::Point(ptz_middle_width,ptz_middle_height), 4, cv::Scalar(255,0,0), -1, 8, 0 );
            //int delta_x = ptzcam.glyph_x_-ptz_middle_width;
            //int delta_y = ptz_middle_height-ptzcam.glyph_y_;
            //float gain_x = 0.03;
            //float gain_y = 0.03;
            //std::cout << "######################### PTZ DELTA: x=" << delta_x << " y=" << delta_y << std::endl;
            //if (delta_x > 5 || delta_y > 5 ){
             //   ptzcam.CommandCamera("?rpan="+std::to_string(delta_x*gain_x));
             //   ptzcam.CommandCamera("?rtilt="+std::to_string(delta_y*gain_y));
           // }
        //}
        




        ptzcam.DisplayPicture();
        //cam.DisplayPicture();       
        



        //cam.DisplayCvDebugPicture();
        cv::waitKey(1);

        cyclenr++;
        auto chrono_cycle_end = std::chrono::steady_clock::now();

        auto chrono_cycle_diff_full_cycle = chrono_cycle_end - chrono_cycle_start;
        auto chrono_cycle_diff_after_grab = chrono_cycle_after_grab - chrono_cycle_start;

        std::cout << "Full cycle took " << std::chrono::duration <double, std::milli> (chrono_cycle_diff_full_cycle).count() << " milliseconds" << endl;
        std::cout << "After grab took " << std::chrono::duration <double, std::milli> (chrono_cycle_diff_after_grab).count() << " milliseconds" << endl;


        full_cycle_file << cyclenr << " " << std::chrono::duration <double, std::milli> (chrono_cycle_diff_full_cycle).count() << std::endl;
        after_grab_file << cyclenr << " " << std::chrono::duration <double, std::milli> (chrono_cycle_diff_after_grab).count() << std::endl;



    }
    full_cycle_file.close();
    after_grab_file.close();

    //ptzcam.SavePicture();
   // cam.SavePicture();

    auto chrono_main_end = std::chrono::steady_clock::now();
    auto chrono_main_diff = chrono_main_end - chrono_main_start;

    std::cout << "Did we use OCL?" << std::endl;
    if(OCL == true){
        std::cout << "Yes" << std::endl;    
    }else {

         std::cout << "No" << std::endl;    
    }

        std::cout << "Total runtime in ms: " << std::chrono::duration <double, std::milli> (chrono_main_diff).count() << " milliseconds" << endl;

    return 0;
}
