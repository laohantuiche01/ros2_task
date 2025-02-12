#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include<filesystem>
#include <iostream>

using namespace cv;
using namespace std;

float w=500,h=700;
Mat matrix,imgWarp1,imgWarp2,imgWarp3,imgWarp4;

int main() {
    string path=std::filesystem::path(__FILE__).
                    parent_path().parent_path()/ "source" / "transform_1.jpg";
    Mat img = imread(path);
    Point2f src[4]={{264,946},{214,682},{642,555},{753,765}};
    Point2f dst[4]={{0.0f,0.0f},{w,0.0f},{w,h},{0.0f,h}};

    matrix = getPerspectiveTransform(src,dst);
    warpPerspective(img,imgWarp1,matrix,Point(w,h));

    for(int i=0;i<4;i++) {
        circle(img,src[i],5,Scalar(255,0,0),2);
    }

    imshow("image", img);
    imshow("image_Warp", imgWarp1);

    waitKey(0);
    return 0;
}