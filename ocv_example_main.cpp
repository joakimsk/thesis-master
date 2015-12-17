#include <stdio.h>
#include "opencv2/opencv.hpp"
#include <opencv2/core/ocl.hpp>

using namespace cv;
 
int main(int argc, char** argv)
{
    ocl::setUseOpenCL(true);


        std::cout << "inform_opencl(): Entry" << std::endl;
        std::cout << "# Checking capabilities" << std::endl;
        std::cout << "# haveOpenCL = " << ocl::haveOpenCL() << std::endl;
        std::cout << "# haveAmdBlas = " << ocl::haveAmdBlas() << std::endl;
        std::cout << "# haveAmdFft = " << ocl::haveAmdFft() << std::endl;
        std::cout << "# haveSVM = " << ocl::haveSVM() << std::endl;
        ocl::setUseOpenCL(false);
        ocl::Context ocl_context;
        if (!ocl_context.create(ocl::Device::TYPE_GPU)){
            std::cerr << "Failed creating the OCL context." << std::endl;
        }
        std::cout << "# GPU devices detected = " << ocl_context.ndevices() << std::endl;
        for (int i = 0; i < ocl_context.ndevices(); i++){
            ocl::Device device = ocl_context.device(i);
            std::cout << "name:              " << device.name() << std::endl;
            std::cout << "available:         " << device.available() << std::endl;
            std::cout << "imageSupport:      " << device.imageSupport() << std::endl;
            std::cout << "OpenCL_C_Version:  " << device.OpenCL_C_Version() << std::endl;
            std::cout << std::endl;
        }
        std::cout << "inform_opencl(): Exit" << std::endl;

    
    UMat img, gray;
    imread("card.jpg", 1).copyTo(img);
     
    cvtColor(img, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, gray,Size(7, 7), 1.5);
    Canny(gray, gray, 0, 50);
     
    imshow("edges", gray);
    waitKey();
    return 0;
}