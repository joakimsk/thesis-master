#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/core/ocl.hpp>

#include "camera.h"
#include "controller.h"

// As of 23. October 2015
// Cam IP: 129.241.154.24
// Default ports open:
// 21/TCP
// 80/TCP
// 554/TCP
// 49155/TCP
// Server IP: 129.241.154.97

namespace {
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
        clock_t cpu_t_start = clock();
        //inform_opencl();


        cv::ocl::setUseOpenCL(false);

        Axis6045 ptzcam;
        Webcam cam;

        ptzcam.ShowInfo();
        cam.ShowInfo();

        ptzcam.GrabPicture();
        cam.GrabPicture();

        cam.GrabPicture();

        cam.GrabPicture();

        cam.GrabPicture();

        //axiscam.SetPassword();
        //axiscam.RefreshPosition();

    //cv::VideoCapture web_camera_capture;
    //cv::VideoCapture ptz_camera_capture;
    
    //ptz_camera_capture.open("http://ptz:ptz@129.241.154.24/mjpg/video.mjpg");
    
    //if (!ptz_camera_capture.isOpened()) {
     //   std::cerr << "ptz camera not opened!\n" << endl;
    //}

    //process(ptz_camera_capture);
    
    //web_camera_capture.open(0);
    //std::cout << "Failed isOpened, opening as video camera" << endl;
    //if (!ptz_camera_capture.isOpened()) {
    //    std::cerr << "Failed to open the ptz_camera_capture!\n" << endl;
    //}


    
    //process_two_cameras(ptz_camera_capture, web_camera_capture);

    //printf("Main ran");
    //cctv.SetPassword();
    //cctv.RefreshPosition();
    clock_t cpu_t_stop = clock();
    clock_t delta_cpu_t = cpu_t_stop - cpu_t_start;
      printf ("Execution of main() took %d clicks (%f seconds). Destructing objects and returning.\n",(int)delta_cpu_t,((float)delta_cpu_t)/CLOCKS_PER_SEC);
    return 0;
}
