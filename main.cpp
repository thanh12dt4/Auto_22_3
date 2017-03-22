#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "image_function.h"
#include <list>
#include <sstream>
using namespace std;
using namespace cv;

//to calculate edge and mask most of non-road area in the image

int main()
{
    VideoCapture cap("/home/trungnt32/Desktop/qt_opencv/mainWindow/p%04d.png");
    while(cap.isOpened()){
        Mat img;
        cap.read(img);   // Read the file

        if(! img.data )         // Check for invalid input
        {
            cout <<  "Could not open or find the image" << endl ;
            return -1;
        }

        Mat gray;
        cvtColor(img, gray, CV_RGB2GRAY);

        Mat edges;
        edges = calcEdgeAndROI(gray);

        vector<Vec2f> lines;
        HoughLines(edges, lines, 3, CV_PI/180, 180);

        list<float> rads = selectRadians(lines);

        bool flag1 = true, flag2 = true;

        //find the best line to the left and right of the vehicle
        StraightLine leftlane, rightlane;
        if(rads.size() > 0 ){
            float maxr = findMaxRads(rads);
            float minr = findMinRads(rads);
            StraightLine l;

            for(size_t i=0; i<lines.size(); i++){
                float theta = lines[i][1];
                float rho = lines[i][0];
                //for RIGHT LANE
                if(maxr == theta and flag1 == true){
                    l = calcX1Y1X2Y2(maxr, rho);
                    if((flag2 == false && maxr-minr > 0.5) || flag2 == true || (flag1== false && flag2 == false)){
                        img = forLine(l, img);
                        rightlane.A.x = l.A.x;
                        rightlane.A.y = l.A.y;
                        rightlane.B.x = l.B.x;
                        rightlane.B.y = l.B.y;
                        flag1 = false;
                    }
                }

                //for LEFT LANE
                if(minr == theta and flag2 == true){
                    l = calcX1Y1X2Y2(minr, rho);
                    if((flag1 == false && maxr-minr > 0.5) || flag1 == true || (flag1== false && flag2 == false)){
                        leftlane.A.x = l.A.x;
                        leftlane.A.y = l.A.y;
                        leftlane.B.x = l.B.x;
                        leftlane.B.y = l.B.y;
                        img = forLine(l, img);
                        flag2 = false;
                    }
                }
            }
        }
        Point_<int> I = findIntersection(leftlane, rightlane);
        circle(img, I, 10, CV_RGB(255,0,0),5);
        line(img, Point(800,1200), Point(800,0),CV_RGB(255,0,0), 10);
        line(img, Point(800,1200), I, CV_RGB(255,0,0), 10);
        float angle = atan2(I.x-800, I.y) * 180 / CV_PI;
        ostringstream lable;
        lable << angle;
        putText(img, lable.str(), Point(850, 600), FONT_HERSHEY_PLAIN, 5.0, CV_RGB(0,255,0), 5.0);

        // Create a window for display.
        namedWindow( "Display window", CV_WINDOW_NORMAL );
        resizeWindow("Display window", 800, 600);
        imshow( "Display window", img );
        imwrite("hinh.png", img);
        waitKey();              // Wait for a keystroke in the window
    }
    return 0;
}
