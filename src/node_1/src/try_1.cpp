#include<iostream>
#include<opencv2/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<filesystem>
#include<opencv2/imgproc.hpp>
//#include<opencv4/opencv2/highgui.hpp>
//#include<opencv4/opencv2/imgproc.hpp>

using namespace cv;
using namespace std;


int main() {
    VideoCapture cap(0);
    Mat frame;
    while(true) {
        cap.read(frame);

        CascadeClassifier faceCascade;
        faceCascade.load(std::filesystem::path(__FILE__).parent_path()
                .parent_path()/"source"/"Resources"/"haarcascade_frontalface_default.xml");

        if (faceCascade.empty()) {cout << "No cascade images found" << endl; }

        vector<Rect> faces;
        faceCascade.detectMultiScale(frame, faces,1.1,10);

        for (int i = 0; i < faces.size(); i++) {
            rectangle(frame, faces[i].tl(), faces[i].br(), Scalar(0, 255, 0),3);
        }

        imshow("frame", frame);
        waitKey(1);
    }
    return 0;
}