#include<iostream>
#include<opencv2/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<filesystem>
#include<opencv2/imgproc.hpp>
//#include<opencv4/opencv2/highgui.hpp>
//#include<opencv4/opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

// int main() {
//     //string path="/home/laohantuiche/program/opencv_first_try/src/node_1/source/4.jpg";
//     //string path="source/4.jpg";
//     string path = std::filesystem::path(__FILE__).parent_path().parent_path() / "source" / "5.jpg";
//     Mat img=imread(path);
//     if (img.empty()) {
//         cerr << "Could not open or find the image: " << path << endl;
//         return -1;
//     }
//     imshow("img",img);
//     waitKey(0);
//     return 0;
// }

// int main() {
//     string path = std::filesystem::path(__FILE__).
//                     parent_path().parent_path() / "source" / "8.mp4";Mat img=imread(path);
//     VideoCapture cap(path);
//     Mat frame;
//     while(true) {
//         cap.read(frame);
//         imshow("frame", frame);
//         waitKey(10);
//     }
//     return 0;
// }

int main() {
    VideoCapture cap(0);
    Mat frame;
    while(true) {
        cap.read(frame);
        imshow("frame", frame);
        waitKey(1);
    }
    return 0;
}