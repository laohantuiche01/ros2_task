#include<iostream>
#include<opencv2/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<filesystem>
#include<opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

void getContours(Mat imgDil,Mat img) {
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(imgDil,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

    vector<vector<Point> > conPoly(contours.size());
    vector<Rect> boundRect(contours.size());

    string objectType ;

    //drawContours(img,contours,-1,Scalar(255,0,255),10);

    for(int i=0;i<contours.size();i++) {
        auto area=contourArea(contours[i]);
        cout << "area = " << area << endl;
        if(area>1000) {
            float peri = arcLength(contours[i],true);
            approxPolyDP(contours[i],conPoly[i],0.03*peri,true);

            cout << conPoly[i].size() << endl;
            boundRect[i] = boundingRect(conPoly[i]);

            int objCor=(int) conPoly[i].size();

            if (objCor==3){objectType="triangle";}
            else if (objCor==4) {
                float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
                cout << "aspRatio = " << aspRatio << endl;
                if (aspRatio > 0.95 && aspRatio < 1.05) {
                    objectType = "Square";
                }
                else {objectType="rectangle";}
            }
            else if (objCor>4){objectType="circle";}

            drawContours(img,contours,i,Scalar(0,0,0),2);
            rectangle(img,boundRect[i].tl(),boundRect[i].br(),Scalar(0,255,0),5);
            putText(img,objectType,{boundRect[i].x,boundRect[i].y-2},
                FONT_HERSHEY_PLAIN,1,Scalar(0,0,255),1);
        }
    }
}

int main() {
    string path = std::filesystem::path(__FILE__).parent_path().
                        parent_path() / "source" / "shape_2.jpg";
    Mat img=imread(path);
    Mat imgGrey,imgBlur,imgCanny,imgDil,imgErode;

    cvtColor(img,imgGrey,COLOR_BGR2GRAY);
    GaussianBlur(imgGrey,imgBlur,Size(7,7),3,0);
    Canny(imgBlur,imgCanny,30,20);

    Mat kernel=getStructuringElement(MORPH_RECT,Size(3,3));
    dilate(imgCanny,imgDil,kernel);
    erode(imgDil,imgErode,kernel);

    getContours(imgDil,img);

    imshow("img",img);
    // imshow("imgGrey",imgGrey);
    // imshow("imgBlur",imgBlur);
    // imshow("imgCanny",imgCanny);
    // imshow("imgDil",imgDil);
    // imshow("imgErode",imgErode);

    waitKey(0);
    return 0;
}