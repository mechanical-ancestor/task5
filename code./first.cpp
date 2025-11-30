
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
using namespace std;
using namespace cv;
int hmin = 5, smin = 100, vmin = 80;
int hmax = 20, smax = 255, vmax = 255;
int main() {
	string path = "/home/wang-zhichou/桌面/task5/images/1.mp4";
	VideoCapture cap (path);
	Mat img, imghsv,mask;
while(true){
    cap.read(img);
    resize(img,img,Size(),0.35,0.5);
    cvtColor(img, imghsv, COLOR_BGR2HSV);
    Scalar lower(hmin, smin, vmin);
    Scalar upper(hmax, smax, vmax);
    inRange(imghsv, lower, upper, mask);//定义颜色下限和上限，因为由于照明和不同的阴影，颜色的值将不完全相同，会是一个值的范围
    vector<vector<Point>> contours;//轮廓检测到的轮廓。每个轮廓线存储为一个点的向量
	vector<Vec4i> hierarchy;
	findContours(mask,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    for(int i=0;i<contours.size();i++)
    {
        Point2f a;
        float b;
        minEnclosingCircle(contours[i],a,b);
        circle(img,a,b,Scalar(255,0,255),3);
        boundingRect(contours[i]);
        rectangle(img,boundingRect(contours[i]),Scalar(255,255,0),3);
    }
    imshow("Image", img);
    imshow("mask",mask);
	waitKey(20);
   }
   destroyAllWindows();
    return 0;
}
