#include <stdio.h>
#include <iostream>
#include <fstream>

#include <iomanip>
#include <string>
#include <chrono>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/core/ocl.hpp>

#include "globals.h"
#include "camera.h"
#include "controller.h"
#include "log.h"

// As of 23. October 2015
// Cam IP: 129.241.154.24
// Default ports open:
// 21/TCP
// 80/TCP
// 554/TCP
// 49155/TCP
// Server IP: 129.241.154.97

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
        cv::ocl::setUseOpenCL(false);
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
    }

    int process(cv::VideoCapture& capture) {
        int n = 0;
        char filename[200];
        std::cout << "press space to save a picture. q or esc to quit" << endl;
        
        cv::UMat gpuFrame;
        cv::UMat gpuBW;
        cv::UMat gpuBlur;
        cv::UMat gpuEdges;
        for (;;) {
            capture >> gpuFrame; // get a new frame from camera
            cv::cvtColor(gpuFrame, gpuBW, cv::COLOR_BGR2GRAY);
            cv::GaussianBlur(gpuBW, gpuBlur, cv::Size(1,1), 1.5, 1.5);
            cv::Canny(gpuBlur, gpuEdges, 1, 3, 3);
            cv::imshow("gpuFrame", gpuEdges);

            char key = (char)cv::waitKey(30); //delay N millis, usually long enough to display and capture input
            switch (key) {
                case 'q':
                case 'Q':
            case 27: //escape key
            return 0;
            case ' ': //Save an image
            sprintf(filename,"filename%.3d.jpg",n++);
            cv::imwrite(filename,gpuFrame);
            cout << "Saved " << filename << endl;
            break;
            default:
            break;
        }
    }
    return 0;
}
int process_two_cameras(cv::VideoCapture& capture1, cv::VideoCapture& capture2) {
    std::cout << "q or esc to quit" << endl;

    cv::UMat gpuFrameCam1;
    cv::UMat gpuFrameCam2;



    cv::UMat gpuBW;
    cv::UMat gpuBlur;
    cv::UMat gpuEdges;
    for (;;) {
        capture1 >> gpuFrameCam1;
        capture2 >> gpuFrameCam2;

        std::cout << capture1.get(cv::CAP_PROP_FRAME_WIDTH) << " jidfossdjfoioeasijfseijofedoj " << std::endl;

            //cv::Size sz1 = gpuFrameCam1.Size();
            //cv::Size sz2 = gpuFrameCam2.Size();
            //std::cout << sz1 << std::endl;
            //cv::UMat mosaic();


            //cv::cvtColor(gpuFrame, gpuBW, cv::COLOR_BGR2GRAY);
            //cv::GaussianBlur(gpuBW, gpuBlur, cv::Size(1,1), 1.5, 1.5);
            //cv::Canny(gpuBlur, gpuEdges, 0, 30, 3);
        cv::imshow("gpuFrame1", gpuFrameCam1);
        cv::imshow("gpuFrame2", gpuFrameCam2);



            char key = (char)cv::waitKey(30); //delay N millis, usually long enough to display and capture input
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
        return 0;
    }
}

int main(int ac, char** av) {
    auto chrono_main_start = std::chrono::steady_clock::now();
    inform_clocks();

    std::cout << "CLOCKS_PER_SEC=" << CLOCKS_PER_SEC << std::endl;
    std::cout << "Precision Ticks per second:" << std::chrono::high_resolution_clock::period::den << std::endl;

    cv::ocl::setUseOpenCL(false);

    Axis6045 ptzcam("129.241.154.24");
    Webcam cam(0);

    ptzcam.SetPassword("ptz");
    ptzcam.RefreshPosition();
    ptzcam.ShowInfo();

    cam.ShowInfo();

    std::ofstream a_file ( "debug.log" );
          a_file << "# X i" << " " << "Y ms" << std::endl;


    ptzcam.OpenDevice();
    cam.OpenDevice();
    cv::startWindowThread();

    int i = 0;
    while(i < 100){
        auto chrono_cycle_start = std::chrono::steady_clock::now();

        //std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
       // std::cout << ms << std::endl;

        ptzcam.RefreshPosition();
        ptzcam.ShowInfo();

        ptzcam.GrabFrame(); // Gather Frame data
        cam.GrabFrame(); // Gather Frame data
        
        ptzcam.RetrieveFrame(); // Decode Frame data
        cam.RetrieveFrame(); // Decode Frame data

        ptzcam.DisplayPicture("PTZ");
        cam.DisplayPicture("Webcam");
        cv::waitKey(1);

        i++;
        auto chrono_cycle_end = std::chrono::steady_clock::now();
        auto chrono_cycle_diff = chrono_cycle_end - chrono_cycle_start;
        std::cout << "Cycle took " << std::chrono::duration <double, std::milli> (chrono_cycle_diff).count() << " milliseconds" << endl;
        a_file << i << " " << std::chrono::duration <double, std::milli> (chrono_cycle_diff).count() << std::endl;

    }
    a_file.close();

    auto chrono_main_end = std::chrono::steady_clock::now();
    auto chrono_main_diff = chrono_main_end - chrono_main_start;
    std::cout << std::chrono::duration <double, std::milli> (chrono_main_diff).count() << " milliseconds" << endl;

    return 0;
}
