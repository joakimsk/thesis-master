#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/videoio/videoio.hpp>
#include <opencv2/core/ocl.hpp>

#include <thread>

#include "curl_easy.h"
#include "curl_form.h"

// As of 23. October 2015
// Cam IP: 129.241.154.24
// Default ports open:
// 21/TCP
// 80/TCP
// 554/TCP
// 49155/TCP

// Server IP: 129.241.154.97

// Some functions from https://putuyuwono.wordpress.com/2015/05/29/multi-thread-multi-camera-capture-using-opencv/

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>

#include "axis.h"

bool thread_cam1_finished = false;
bool thread_cam2_finished = false;
bool thread_cv_finished = false;


//hide the local functions in an anon namespace
namespace {
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

void hello(){
    std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;
}

int main(int ac, char** av) {
    std::cout << "# Starting OpenCV, checking capabilities" << endl;
    std::cout << "# haveOpenCL = " << cv::ocl::haveOpenCL() << endl;
    std::cout << "# haveAmdBlas = " << cv::ocl::haveAmdBlas() << endl;
    std::cout << "# haveAmdFft = " << cv::ocl::haveAmdFft() << endl;
    std::cout << "# haveSVM = " << cv::ocl::haveSVM() << endl;

    cv::ocl::setUseOpenCL(true);

    cv::ocl::Context ocl_context;
    if (!ocl_context.create(cv::ocl::Device::TYPE_GPU))
    {
        std::cout << "Failed creating the OCL context." << endl;
        return 1;
    }

    std::cout << "# GPU devices detected = " << ocl_context.ndevices() << endl;

for (int i = 0; i < ocl_context.ndevices(); i++)
{
    cv::ocl::Device device = ocl_context.device(i);
    cout << "name:              " << device.name() << endl;
    cout << "available:         " << device.available() << endl;
    cout << "imageSupport:      " << device.imageSupport() << endl;
    cout << "OpenCL_C_Version:  " << device.OpenCL_C_Version() << endl;
    cout << endl;
}

    cv::VideoCapture ptz_camera_capture;
    cv::VideoCapture web_camera_capture;

    std::vector<std::thread> threads;
    for(int i = 0; i < 5; ++i){
        threads.push_back(std::thread(hello));
    }

    for(auto& thread : threads){
        thread.join();
    }


    ptz_camera_capture.open("http://ptz:ptz@129.241.154.24/mjpg/video.mjpg");
    //web_camera_capture.open(0);

    //std::cout << "Failed isOpened, opening as video camera" << endl;
    //if (!ptz_camera_capture.isOpened()) {
        //std::cerr << "Failed to open the ptz_camera_capture!\n" << endl;
        //help(av);
        //return 1;
    //}

    if (!ptz_camera_capture.isOpened()) {
        std::cerr << "Failed to open the web_camera_capture!\n" << endl;
        return 1;
    }
    
    //process_two_cameras(ptz_camera_capture, web_camera_capture);
    process(ptz_camera_capture);
    printf("Main ran");
    
    //Axis cctv("129.241.154.24");
    //cctv.SetPassword();
    //cctv.RefreshPosition();
    std::cout << "finally" << std::endl;
    return 0;
}
