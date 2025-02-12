#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include<filesystem>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    string path=std::filesystem::path(__FILE__).
                    parent_path().parent_path()/ "source" / "1.jpg";
    Mat img = imread(path);
    Mat imgResize,imgCrop;

    cout << "img size: " << img.size << endl;
    resize(img, imgResize, Size(),0.7,0.7);

    Rect roi(300 , 220 , 300 , 300);
    imgCrop = img(roi);

    imshow("image", img);
    //imshow("resize", imgResize);
    imshow("crop", imgCrop);

    waitKey(0);
    return 0;
}