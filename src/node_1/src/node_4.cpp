#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include<filesystem>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    Mat img(512,512,CV_8UC3,Scalar(255,255,255));

    circle(img,Point(256,256),155,Scalar(255,50,160),FILLED);
    rectangle(img,Point(130,226),
            Point(382,286),Scalar(255,255,255),-1);
    line(img,Point(130,296),Point(382,296),Scalar(255,255,255),2);
    putText(img,"FUCK YOU!",Point(137,262),
        FONT_HERSHEY_TRIPLEX,1,Scalar(0,69,255),2);

    imshow("image", img);

    waitKey(0);

    return 0;
}