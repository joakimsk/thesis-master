#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/core/ocl.hpp>

#include "curl_easy.h"

// As of 23. October 2015
// Cam IP: 129.241.154.24
// Default ports open:
// 21/TCP
// 80/TCP
// 554/TCP
// 49155/TCP

// Server IP: 129.241.154.97

#include <stdio.h>
#include <iostream>

using curl::curl_easy;

//hide the local functions in an anon namespace
namespace {
    void help(char** av) {
        cout << "The program captures frames from a video file, image sequence (01.jpg, 02.jpg ... 10.jpg) or camera connected to your computer." << endl
             << "Usage:\n" << av[0] << " <video file, image sequence or device number>" << endl
             << "q,Q,esc -- quit" << endl
             << "space   -- save frame" << endl << endl
             << "\tTo capture from a camera pass the device number. To find the device number, try ls /dev/video*" << endl
             << "\texample: " << av[0] << " 0" << endl
             << "\tYou may also pass a video file instead of a device number" << endl
             << "\texample: " << av[0] << " video.avi" << endl
             << "\tYou can also pass the path to an image sequence and OpenCV will treat the sequence just like a video." << endl
             << "\texample: " << av[0] << " right%%02d.jpg" << endl;
    }

    int process(cv::VideoCapture& capture) {
        int n = 0;
        char filename[200];
        string window_name = "video | q or esc to quit";
        std::cout << "press space to save a picture. q or esc to quit" << endl;
        cv::namedWindow(window_name, cv::WINDOW_KEEPRATIO); //resizable window;
        cv::Mat frame;

        for (;;) {
            capture >> frame;
            if (frame.empty())
                break;

            cv::imshow(window_name, frame);
            char key = (char)cv::waitKey(30); //delay N millis, usually long enough to display and capture input

            switch (key) {
            case 'q':
            case 'Q':
            case 27: //escape key
                return 0;
            case ' ': //Save an image
                sprintf(filename,"filename%.3d.jpg",n++);
                cv::imwrite(filename,frame);
                cout << "Saved " << filename << endl;
                break;
            default:
                break;
            }
        }
        return 0;
    }
}

int main(int ac, char** av) {
    if (ac != 2) {
        help(av);
        return 1;
    }

    std::cout << "# Starting OpenCV, checking capabilities" << endl;
    std::cout << "# haveOpenCL = " << cv::ocl::haveOpenCL() << endl;
    std::cout << "# haveAmdBlas = " << cv::ocl::haveAmdBlas() << endl;
    std::cout << "# haveAmdFft = " << cv::ocl::haveAmdFft() << endl;
    std::cout << "# haveSVM = " << cv::ocl::haveSVM() << endl;

    cv::ocl::Context ocl_context;
    if (!ocl_context.create(cv::ocl::Device::TYPE_GPU))
    {
        std::cout << "Failed creating the OCL context." << endl;
        return 1;
    }

    std::cout << "# GPU devices detected = " << ocl_context.ndevices() << endl;

    cv::Mat image;
    image = cv::imread("test.jpg",cv::IMREAD_COLOR);
    cv::namedWindow("Window",cv::WINDOW_AUTOSIZE);
    cv::imshow("Window", image);
    //cv::waitKey(0);

    std::string arg = av[1];
    cv::VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file or image sequence
    if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
        capture.open(atoi(arg.c_str()));
    if (!capture.isOpened()) {
        std::cerr << "Failed to open the video device, video file or image sequence!\n" << endl;
        help(av);
        return 1;
    }
    return process(capture);

    curl_easy easy;
    printf("Main ran");
    return 0;
}
