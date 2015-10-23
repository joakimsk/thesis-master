#include "curl_easy.h"
#include <CL/cl.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/core/ocl.hpp>

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

int main(){
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cout<<"No platforms found."<<endl;
        exit(1);
    } else {
        std::cout<<"Found platforms:"<<endl;
        for (int i = 0; i < all_platforms.size(); i++){
          std::cout << "Platform " << i << "=" << all_platforms[i].getInfo<CL_PLATFORM_NAME>() << endl;
      }
    }
    cl::Platform default_platform=all_platforms[0];
    std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>()<<endl;

    // Get the default device of the platform

    std::vector<cl::Device> all_devices;

    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        std::cout << "No devices found. Check OpenCL!" << endl;
        exit(1);
    }
    cl::Device default_device=all_devices[0];
    std::cout << "Using device: " <<default_device.getInfo<CL_DEVICE_NAME>() << endl;


    cl::Context context({default_device});

    cl::Program::Sources sources;

    std::string kernel_code=
    "   void kernel simple_add(global const int* A, global const int* B, global int* C){       "
    "       C[get_global_id(0)]=A[get_global_id(0)]+B[get_global_id(0)];                       "
    "   }                                                                                      "; 

    sources.push_back({kernel_code.c_str(),kernel_code.length()});

    cl::Program program(context,sources);
    if(program.build({default_device})!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)<<"\n";
        exit(1);
    }

    // create buffers on the device
    cl::Buffer buffer_A(context,CL_MEM_READ_WRITE,sizeof(int)*10);
    cl::Buffer buffer_B(context,CL_MEM_READ_WRITE,sizeof(int)*10);
    cl::Buffer buffer_C(context,CL_MEM_READ_WRITE,sizeof(int)*10);

    int A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int B[] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0};

    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(context,default_device);

    //write arrays A and B to the device
    queue.enqueueWriteBuffer(buffer_A,CL_TRUE,0,sizeof(int)*10,A);
    queue.enqueueWriteBuffer(buffer_B,CL_TRUE,0,sizeof(int)*10,B);

    // Run the kernel
    cl::Kernel kernel_add=cl::Kernel(program, "simple_add");
    kernel_add.setArg(0,buffer_A);
    kernel_add.setArg(1,buffer_B);
    kernel_add.setArg(2,buffer_C);
    queue.enqueueNDRangeKernel(kernel_add,cl::NullRange,cl::NDRange(10),cl::NullRange);
    queue.finish();

    int C[10];
    
    //read result C from the device to array C
    queue.enqueueReadBuffer(buffer_C,CL_TRUE,0,sizeof(int)*10,C);

    std::cout<<" result: \n";
    for(int i=0;i<10;i++){
        std::cout<<C[i]<<" ";
    }

    std::cout << "Starting OpenCV" << endl;

    if(!cv::ocl::haveOpenCL()){

        std::cout << "OpenCL is not available for OpenCV!" << endl;
    }

    cv::ocl::Context ocl_context;
    if (!ocl_context.create(cv::ocl::Device::TYPE_GPU))
    {
        std::cout << "Failed creating the context..." << endl;
        return 1;
    }

    std::cout << ocl_context.ndevices() << " GPU devices are detected." << endl;

    cv::Mat image;
    image = cv::imread("test.jpg",cv::IMREAD_COLOR);
    cv::namedWindow("Window",cv::WINDOW_AUTOSIZE);
    cv::imshow("Window", image);
    cv::waitKey(0);

    curl_easy easy;
    printf("Main ran");
    return 0;
}
