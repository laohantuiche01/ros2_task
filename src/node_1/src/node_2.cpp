#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include<filesystem>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    string path=std::filesystem::path(__FILE__).
                    parent_path().parent_path()/ "source" / "6.jpg";
    Mat img = imread(path);
    Mat imgGrey,imgBlur,imgCanny,imgDil,imgErode;

    cvtColor(img,imgGrey,COLOR_BGR2GRAY);

    GaussianBlur(img,imgBlur,Size(7,7),3,0);

    Canny(imgBlur,imgCanny,30,20);

    Mat kernel =getStructuringElement(MORPH_RECT,Size(3,3));

    dilate(imgCanny,imgDil,kernel);

    erode(imgCanny,imgErode,kernel);

    //imshow("image", img);
    //imshow("imageGrey", imgGrey);
    //imshow("imageBlur", imgBlur);
    imshow("imageCanny", imgCanny);
    imshow("dilation", imgDil);
    imshow("erode", imgErode);

    waitKey(0);
    return 0;
}