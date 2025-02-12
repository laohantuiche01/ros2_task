#include<iostream>
#include<opencv2/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<filesystem>
#include<opencv2/imgproc.hpp>


using namespace cv;
using namespace std;

vector<vector<int>> myColors {{136,40,63,168,115,207}//紫色
    ,{117,51,110,134,99,184}, //蓝色
    {162,60,192,179,169,255},//粉色
    {13,73,166,41,117,255},//黄色
    {0,75,203,19,240,255} ,//橙色
    {0,29,0,36,150,129}
};

vector<Scalar> myScalars = {{128,0,128},//紫色
    {255,0,0},  //蓝色
    {149,126,224},   //粉色
    {0,255,255},
    {0,128,255},
    {0,255,0}
};

vector<vector<int>> newPoints;
Mat img;

Point getContours(Mat imgDil) {
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(imgDil,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

    vector<vector<Point> > conPoly(contours.size());
    vector<Rect> boundRect(contours.size());

    string objectType ;

    //drawContours(img,contours,-1,Scalar(255,0,255),10);

    Point myPoint(0,0);

    for(int i=0;i<contours.size();i++) {
        auto area=contourArea(contours[i]);
        cout << "area = " << area << endl;
        if(area>1000) {
            float peri = arcLength(contours[i],true);
            approxPolyDP(contours[i],conPoly[i],0.01*peri,true);

            cout << conPoly[i].size() << endl;
            boundRect[i] = boundingRect(conPoly[i]);
            myPoint.x=boundRect[i].x+boundRect[i].width/2;
            myPoint.y=boundRect[i].y;

            drawContours(img,contours,i,Scalar(255,0,255),2);
            rectangle(img,boundRect[i].tl(),
                boundRect[i].br(),Scalar(0,255,0),5);
        }
    }
    return myPoint;
}

void findColor(Mat img) {
    Mat imgHSV;
    cvtColor(img, imgHSV, COLOR_BGR2HSV);

    for (int i = 0; i < myColors.size(); i++) {
        Scalar lower(myColors[i][0],myColors[i][1],myColors[i][2]);
        Scalar upper(myColors[i][3],myColors[i][4],myColors[i][5]);
        Mat mask;
        inRange(imgHSV, lower, upper, mask);
        Point myPoint=getContours(mask);
        //imshow(to_string(i), mask);

        if (myPoint.x!=0 && myPoint.y!=0) {
            newPoints.push_back({myPoint.x,myPoint.y,i});
        }

    }
}

void drawOnCanvas(vector<vector<int>> newPoints,vector<Scalar> myScalars)
{
    for(int i=0;i<newPoints.size();i++) {
        circle(img,Point(newPoints[i][0],newPoints[i][1]),
            10,myScalars[newPoints[i][2]],FILLED);
    }
}

int main() {

    VideoCapture cap(0);
    Mat pxrSB;

    while(true) {
        cap.read(img);

        findColor(img);

        drawOnCanvas(newPoints,myScalars);
        flip(img, zxkk, 1);


        imshow("img", zxkSB);
        waitKey(1);
    }
    return 0;
}