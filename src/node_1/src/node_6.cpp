#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include<filesystem>
#include <iostream>

using namespace cv;
using namespace std;

Mat imgHSV,mask;
int hmin=0,smin=110,vmin=153;
int hmax=19,smax=240,vmax=255;

int main() {
    string path=std::filesystem::path(__FILE__).
                    parent_path().parent_path()/ "source" / "shape_2.jpg";
    Mat img = imread(path);

    cvtColor(img, imgHSV, COLOR_BGR2HSV);

    namedWindow("HSV", (640,200));
    createTrackbar("Hue Min","HSV",&hmin,179);
    createTrackbar("Hue Max","HSV",&hmax,179);
    createTrackbar("Sat Min","HSV",&smin,255);
    createTrackbar("Sat Max","HSV",&smax,255);
    createTrackbar("Val Min","HSV",&vmin,255);
    createTrackbar("Val Max","HSV",&vmax,255);

    while (true) {
        Scalar lower(hmin,smin,vmin);
        Scalar upper(hmax,smax,vmax);
        inRange(imgHSV, lower, upper, mask);

        imshow("image", img);
        //imshow("imageHSV", imgHSV);
        imshow("mask", mask);
        waitKey(1);
    }

    return 0;
}