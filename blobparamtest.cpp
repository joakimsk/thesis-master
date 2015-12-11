#include <iostream>
#include <iomanip> 
#include <chrono>
#include <opencv2/opencv.hpp>
#include "stdio.h"

/*
SimpleBlobDetector parameter tester for pipes and casings in Fingerboards
First revision written by Joakim Skjefstad (skjefstad.joakim@gmail.com) 11th of December 2015

Based partially on:
http://www.learnopencv.com/blob-detection-using-opencv-python-c/

Requires OpenCV 3.0
*/
using namespace cv;

std::string filename = "input.jpg";

int ThresholdSliderMax = 500;
int minThresholdSlider = 0;
int maxThresholdSlider = ThresholdSliderMax;

int AreaSliderMax = 5000;
int minAreaSlider = 0;
int maxAreaSlider = AreaSliderMax;

int CircularitySliderMax = 100;
int minCircularitySlider = 0;
int maxCircularitySlider = CircularitySliderMax;
float valueMinCircularity = 0.0;
float valueMaxCircularity = 0.0;

int ConvexitySliderMax = 100;
int minConvexitySlider = 0;
int maxConvexitySlider = ConvexitySliderMax;
float valueMinConvexity = 0.0;
float valueMaxConvexity = 0.0;

int InertiaRatioSliderMax = 100;
int minInertiaRatioSlider = 0;
int maxInertiaRatioSlider = InertiaRatioSliderMax;
float valueMinInertiaRatio = 0.0;
float valueMaxInertiaRatio = 0.0;

void on_trackbar( int, void* )
{
    auto chrono_cycle_start = std::chrono::steady_clock::now();

// Read image
    Mat im = imread(filename, IMREAD_GRAYSCALE );

// Setup SimpleBlobDetector parameters.
    SimpleBlobDetector::Params params;

// Change thresholds
    std::cout  << std::endl << "#######  SimpleBlobDetector parameters  #######" << std::endl;
    std::cout  << "minThresholdSlider = " <<  minThresholdSlider << std::endl;
    std::cout  << "maxThresholdSlider = " <<  maxThresholdSlider << std::endl;
    params.minThreshold = minThresholdSlider;
    params.maxThreshold = maxThresholdSlider;

// Filter by Area.
    params.filterByArea = true;
    std::cout << "minAreaSlider = " << minAreaSlider << std::endl;
    std::cout << "maxAreaSlider = " << maxAreaSlider << std::endl;

    params.minArea = minAreaSlider;
    params.maxArea = maxAreaSlider;

// Filter by Circularity
    params.filterByCircularity = true;
    valueMinCircularity = (float)minCircularitySlider/(float)CircularitySliderMax;
    valueMaxCircularity = (float)maxCircularitySlider/(float)CircularitySliderMax;
    std::cout << "valueMinCircularity = " << std::fixed << std::setprecision(3) << valueMinCircularity  << std::endl;
    std::cout  << "valueMaxCircularity = " << std::fixed << std::setprecision(3) << valueMaxCircularity  << std::endl;
    params.minCircularity = valueMinCircularity;
    params.maxCircularity = valueMaxCircularity;

// Filter by Convexity
    params.filterByConvexity = true;
    valueMinConvexity=  (float)minConvexitySlider/(float)ConvexitySliderMax;
    valueMaxConvexity = (float)maxConvexitySlider/(float)ConvexitySliderMax;
    std::cout  << "valueMinConvexity = " <<  std::fixed << std::setprecision(3) << valueMinConvexity << std::endl;
    std::cout  << "valueMaxConvexity = " << std::fixed << std::setprecision(3) << valueMaxConvexity << std::endl;
    params.minConvexity  =valueMinConvexity;
    params.maxConvexity  = valueMaxConvexity;

// Filter by Inertia
    params.filterByInertia = true;
    valueMinInertiaRatio = (float)minInertiaRatioSlider/(float)InertiaRatioSliderMax;
    valueMaxInertiaRatio = (float)maxInertiaRatioSlider/(float)InertiaRatioSliderMax;
    std::cout   << "valueMinInertiaRatio = " << std::fixed << std::setprecision(3) << valueMinInertiaRatio << std::endl;
    std::cout   << "valueMaxInertiaRatio = " <<  std::fixed << std::setprecision(3) << valueMaxInertiaRatio << std::endl;
    params.minInertiaRatio =valueMinInertiaRatio;
    params.maxInertiaRatio = valueMaxInertiaRatio;

// Set up the detector with default parameters.
    Ptr<SimpleBlobDetector> ptrBlobDetector = SimpleBlobDetector::create(params);

// Detect blobs.
    std::vector<KeyPoint> keypoints;

    ptrBlobDetector->detect( im, keypoints);

// Draw detected blobs as red circles.
// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
    Mat im_with_keypoints;
    drawKeypoints( im, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

// Show blobs
    imshow("BlobWindow", im_with_keypoints );
    auto chrono_cycle_end = std::chrono::steady_clock::now();
    auto chrono_cycle_diff = chrono_cycle_end - chrono_cycle_start;
    std::cout << "Cycle took " << std::chrono::duration <double, std::milli> (chrono_cycle_diff).count() << " milliseconds" << std::endl;
}


int main(int argc, char* argv[])
{

    std::cout << "SimpleBlobDetector for pipe- and casing-testing" << std::endl;
    if (argc == 2){
        filename = argv[1];
    } else {
        std::cout << "Input image not found in argument. Supports JPEG and PNG." << std::endl;
        std::cout << "Call: application <filename>" << std::endl;
        return -1;
    }
    namedWindow("BlobWindow");

    // Create the trackbars
    createTrackbar( "minThreshold", "BlobWindow", &minThresholdSlider, ThresholdSliderMax, on_trackbar );
    createTrackbar( "maxThreshold", "BlobWindow", &maxThresholdSlider, ThresholdSliderMax, on_trackbar );
    createTrackbar( "minArea", "BlobWindow", &minAreaSlider, AreaSliderMax, on_trackbar );
    createTrackbar( "maxArea", "BlobWindow", &maxAreaSlider, AreaSliderMax, on_trackbar );
    createTrackbar( "minCircularity", "BlobWindow", &minCircularitySlider, CircularitySliderMax, on_trackbar );
    createTrackbar( "maxCircularity", "BlobWindow", &maxCircularitySlider, CircularitySliderMax, on_trackbar );
    createTrackbar( "minConvexity", "BlobWindow", &minConvexitySlider, ConvexitySliderMax, on_trackbar );
    createTrackbar( "maxConvexity", "BlobWindow", &maxConvexitySlider, ConvexitySliderMax, on_trackbar );
    createTrackbar( "minInertiaRatio", "BlobWindow", &minInertiaRatioSlider, InertiaRatioSliderMax, on_trackbar );
    createTrackbar( "maxInertiaRatio", "BlobWindow", &maxInertiaRatioSlider, InertiaRatioSliderMax, on_trackbar );
    on_trackbar(0,0); // Initialize window
    waitKey(0);
}