#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() 
{
    string path = "/home/wanqing/my_code/task_5/final_task5/text_img/origin.mp4";
    VideoCapture cap(path);
    Mat img,imgResize, imgHSV,mask;

    int hmin = 5, smin = 100, vmin = 80;//调整后可识别篮球的HSV初始阈值
    int hmax = 20, smax = 255, vmax = 255;

    namedWindow("Trackbars",WINDOW_NORMAL);
    resizeWindow("Trackbars", 640, 200);//创建一个滑块窗口
    createTrackbar("Hue Min", "Trackbars", &hmin, 179/*最大值 */);//创建色调(hue)滑块
    createTrackbar("Hue Max", "Trackbars", &hmax, 179);
    createTrackbar("Sat Min", "Trackbars", &smin, 255);
    createTrackbar("Sat Max", "Trackbars", &smax, 255);
    createTrackbar("Val Min", "Trackbars", &vmin, 255);
    createTrackbar("val Max", "Trackbars", &vmax, 255);

    while(true)
    {
        cap.read(img);

        resize(img, imgResize, Size(), 0.3, 0.3);//调整大小

        cvtColor(imgResize, imgHSV, COLOR_BGR2HSV);//转HSV图

        Scalar lower(hmin, smin, vmin);//应用阈值
        Scalar upper(hmax, smax, vmax);
        inRange(imgHSV, lower, upper, mask);

        vector<vector<Point>>contours;//查找轮廓
        vector<Vec4i>hierarchy;
        findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (int i = 0; i < contours.size(); i++)
        {
            double area = contourArea(contours[i]);
            if (area < 500)//利用面积除噪点
                continue;

            Point2f center;//计算最小外接圆并画出
            float radius;
            minEnclosingCircle(contours[i], center, radius);
            circle(imgResize, center, radius, Scalar(255, 0, 255), 3);

            Rect rect = boundingRect(contours[i]);//画出矩形外框
            rectangle(imgResize, rect, Scalar(0, 255, 0), 2);

            //添加basketball文本
            putText(imgResize, "basketball", Point(rect.x, rect.y - 10), FONT_HERSHEY_SIMPLEX, 0.25, Scalar(0, 255, 255), 1);

        }

        imshow("Image HSV", imgHSV);
        imshow("Image Mask", mask);
        imshow("Image Result", imgResize);//展示结果图
        
        waitKey(20);
    }
return 0;
}
