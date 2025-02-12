#include<iostream>
#include<opencv2/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<filesystem>
#include<opencv2/imgproc.hpp>
#include<opencv2/objdetect.hpp>

using namespace cv;
using namespace std;

int main() {
    string path = std::filesystem::path(__FILE__).parent_path().
                  parent_path() / "source" / "4.jpg";
    Mat img = imread(path);

    CascadeClassifier faceCascade;
    faceCascade.load(std::filesystem::path(__FILE__).parent_path()
            .parent_path()/"source"/"Resources"/"haarcascade_frontalface_default.xml");

    if (faceCascade.empty()) {cout << "No cascade images found" << endl; }

    vector<Rect> faces;
    faceCascade.detectMultiScale(img, faces,1.1,10);

    for (int i = 0; i < faces.size(); i++) {
        rectangle(img, faces[i].tl(), faces[i].br(), Scalar(0, 255, 0),3);
    }

    imshow("img", img);
    waitKey(0);
    return 0;
}
