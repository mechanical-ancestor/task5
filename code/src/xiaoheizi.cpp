#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

Mat frame, hsv, mask;
vector<int> s = {5, 20, 100, 255, 80, 255};
vector<string> name = {"h_min", "h_max", "s_min", "s_max", "v_min", "v_max"};
Scalar lower_hsv, upper_hsv;

// 更新HSV值的回调函数
void updateHSV(int, void*) {
    lower_hsv = Scalar(s[0], s[2], s[4]);
    upper_hsv = Scalar(s[1], s[3], s[5]);
}

int main() {
    namedWindow("test", WINDOW_NORMAL);
    
    // 初始化HSV值
    updateHSV(0, 0);

    VideoCapture cap("../../assets/origin.mp4");
    if (!cap.isOpened()) {
        cerr << "Error: Could not open video file." << endl;
        return -1;
    }
    
    // 创建滑动条
    for (int i = 0; i < 6; i++) {
        createTrackbar(name[i], "test", NULL, 255, 
                      [](int val, void* userdata) {
                          int index = *(int*)userdata;
                          s[index] = val;
                          updateHSV(0, 0);
                      }, new int(i));  // 传递索引
        setTrackbarPos(name[i], "test", s[i]);
    }

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        
        resize(frame, frame, Size(480, 320));
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        // 掩膜
        inRange(hsv, lower_hsv, upper_hsv, mask);
   
        // 形态学优化
        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
        morphologyEx(mask, mask, MORPH_CLOSE, kernel);
        GaussianBlur(mask, mask, Size(3, 3), 0);
        
        // 轮廓检测
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        Mat result = frame.clone();
        
        if (!contours.empty()) {
            // 寻找最大轮廓
            int maxIndex = 0;
            double maxArea = 0;
            for (int i = 0; i < contours.size(); i++) {
                double area = contourArea(contours[i]);
                if (area > maxArea) {
                    maxArea = area;
                    maxIndex = i;
                }
            }
            
            if (maxArea > 150) {  // 设置最小面积阈值
                // 绘制边界框
                Rect boundRect = boundingRect(contours[maxIndex]);
                rectangle(result, boundRect, Scalar(255, 0, 0), 2);
                
                // 绘制圆
                Point center(boundRect.x + boundRect.width/2, boundRect.y + boundRect.height/2);
                int Radius = (boundRect.width + boundRect.height)/4;   // 定义半径
                circle(result, center, Radius, Scalar(0, 0, 255), 2);
                
                putText(result, "basketball", Point(boundRect.x+10, boundRect.y+10), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);
            }
        }
        
        // **一定要将二值掩膜转换为彩色掩膜，使其与视频的通道数一致，以实现拼接
        Mat colorMask;
        cvtColor(mask, colorMask, COLOR_GRAY2BGR);
        
        Mat combine, display;
        hconcat(frame, colorMask, combine);
        hconcat(combine, result, display);
        
        imshow("test", display);
        
        if (waitKey(25) == 27) {
            break;
        }
    }

    cap.release();
    destroyAllWindows();
    return 0;
}