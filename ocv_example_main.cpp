#include <iostream>      /* printf */
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
    Mat src, dst, color_dst, cannymat;
    if( argc != 2 || !(src=imread(argv[1], 0)).data)
        return -1;
    blur(src, dst, Size(3,3));
    Canny( dst, cannymat, 50, 200, 3 );
    cvtColor( cannymat, color_dst, COLOR_GRAY2BGR );
#if 0
    vector<Vec2f> lines;
    HoughLines( dst, lines, 1, CV_PI/180, 100 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0];
        float theta = lines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        Point pt1(cvRound(x0 + 1000*(-b)),
                  cvRound(y0 + 1000*(a)));
        Point pt2(cvRound(x0 - 1000*(-b)),
                  cvRound(y0 - 1000*(a)));
        line( color_dst, pt1, pt2, Scalar(0,0,255), 3, 8 );
    }
#else
    vector<Vec4i> lines;
    HoughLinesP( cannymat, lines, 1, CV_PI/180, 80, 30, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        double res = norm(Point(lines[i][0], lines[i][1])- Point(lines[i][2], lines[i][3]));
        if (res > 150.0){
            line( color_dst, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
            double res = norm(Point(lines[i][0], lines[i][1])- Point(lines[i][2], lines[i][3]));
            cout << i << "=" << res << endl;
        }
    }
#endif
    namedWindow( "Source", 1 );
    imshow( "Source", src );
    namedWindow( "Detected Lines", 1 );
       // imshow( "Canny Lines", cannymat );

    imshow( "Detected Lines", color_dst );
    waitKey(0);
    return 0;
}