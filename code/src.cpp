#include<iostream>
using namespace std;
#include<opencv2/opencv.hpp>
using namespace cv;
#include<vector>  

// 存储阈值的结构体
class Thresholds {
public:
    int H_low;  int H_high;
    int S_low;  int S_high;
    int V_low;  int V_high;
};
Thresholds thresh{5, 20, 100, 255, 150, 255}; // 初始阈值

// 滑动条回调函数
// void onTrackbarChange(int, void* userData) {
//     Thresholds* t = static_cast<Thresholds*>(userData);
//     t->H_low = getTrackbarPos("H-low", "Video");
//     cout << "当前H-low初始值: " << t->H_low << endl;
//     t->H_high = getTrackbarPos("H-high", "Video");
//     cout << "当前H-high初始值: " << t->H_high << endl;
//     t->S_low = getTrackbarPos("S-low", "Video");
//     cout << "当前S-low初始值: " << t->S_low << endl;
//     t->S_high = getTrackbarPos("S-high", "Video");
//     cout << "当前S-high初始值: " << t->S_high << endl;
//     t->V_low = getTrackbarPos("V-low", "Video");
//     cout << "当前V-low初始值: " << t->V_low << endl;
//     t->V_high = getTrackbarPos("V-high", "Video");
//     cout << "当前V-high初始值: " << t->V_high << endl;
//     cout << "------------------------" << endl;
// }

// 滑动条回调函数(分开创建，否则只能获取第一个滑动条的值，后面全为0)
void onTrackbarChange1(int, void* userData) {
    Thresholds* t = static_cast<Thresholds*>(userData);
    t->H_low = getTrackbarPos("H-low", "Video");
    cout << "当前H-low初始值: " << t->H_low << endl;
}
void onTrackbarChange2(int, void* userData) {
    Thresholds* t = static_cast<Thresholds*>(userData);
    t->H_high = getTrackbarPos("H-high", "Video");
    cout << "当前H-high初始值: " << t->H_high << endl;
}
void onTrackbarChange3(int, void* userData) {
    Thresholds* t = static_cast<Thresholds*>(userData);
    t->S_low = getTrackbarPos("S-low", "Video");
    cout << "当前S-low初始值: " << t->S_low << endl;
}
void onTrackbarChange4(int, void* userData) {
    Thresholds* t = static_cast<Thresholds*>(userData);
    t->S_high = getTrackbarPos("S-high", "Video");
    cout << "当前S-high初始值: " << t->S_high << endl;
}
void onTrackbarChange5(int, void* userData) {
    Thresholds* t = static_cast<Thresholds*>(userData);
    t->V_low = getTrackbarPos("V-low", "Video");
    cout << "当前V-low初始值: " << t->V_low << endl;
}
void onTrackbarChange6(int, void* userData) {
    Thresholds* t = static_cast<Thresholds*>(userData);
    t->V_high = getTrackbarPos("V-high", "Video");
    cout << "当前V-high初始值: " << t->V_high << endl;
}
// 当前H-low初始值: 5
// 当前H-high初始值: 20
// 当前S-low初始值: 100
// 当前S-high初始值: 255
// 当前V-low初始值: 150


int main() {
    // 打开摄像头
    VideoCapture cap(0);
    // VideoCapture cap("asset/origin.mp4");
    // VideoCapture cap("/home/zooyouyoung/task/asset/origin.mp4");
    if(!cap.isOpened()){
        cout << "fuck.............................................you" << endl;
        return -1;
    }

    // // 创建窗口
    // namedWindow("哥哥的篮球识别结果", WINDOW_NORMAL);
    // namedWindow("bin", WINDOW_NORMAL);


    // int H_lowest = 40, H_high = 80;
    // int S_lowest = 40;
    // int V_lowest = 10; 
    // createTrackbar("H-lower", "Video", &H_lowest, 15);
    // createTrackbar("H-high", "Video", &H_high, 40);

    // 创建窗口
    namedWindow("Video", WINDOW_NORMAL);
    // 创建滑动条
    createTrackbar("H-low", "Video", NULL, 180, onTrackbarChange1, &thresh);
    createTrackbar("H-high", "Video", NULL, 180, onTrackbarChange2, &thresh);
    createTrackbar("S-low", "Video", NULL, 255, onTrackbarChange3, &thresh);
    createTrackbar("S-high", "Video", NULL, 255, onTrackbarChange4, &thresh);
    createTrackbar("V-low", "Video", NULL, 255, onTrackbarChange5, &thresh);
    createTrackbar("V-high", "Video", NULL, 255, onTrackbarChange6, &thresh);
    // 设置滑动条初始值
    setTrackbarPos("H-low", "Video", thresh.H_low);
    setTrackbarPos("H-high", "Video", thresh.H_high);
    setTrackbarPos("S-low", "Video", thresh.S_low);
    setTrackbarPos("S-high", "Video", thresh.S_high);
    setTrackbarPos("V-low", "Video", thresh.V_low);
    setTrackbarPos("V-high", "Video", thresh.V_high);

    Mat frame, rst, hsv, bin;
    while(1) {
        cap >> frame;// cap.read(frame);
        frame.copyTo(rst);
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        Scalar orange_low(thresh.H_low, thresh.S_low, thresh.V_low) , orange_high(thresh.H_high, thresh.S_high, thresh.V_high);

        // 生成二值图bin
        inRange(hsv, orange_low, orange_high, bin);
        
        // 降噪(保留目标->先膨胀后腐蚀)
        Mat kernel01 = getStructuringElement(MORPH_RECT, Size(9,9));
        Mat kernel02 = getStructuringElement(MORPH_RECT, Size(3,3));
        dilate(bin, bin, kernel01); // 膨胀
        dilate(bin, bin, kernel01); // 膨胀
        erode(bin, bin, kernel02); // 腐蚀
        
        // 轮廓检测
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(bin, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        vector<Point> Max_contour;
        if(!contours.empty()){
            Max_contour = contours[0];
            for(vector<Point> contour : contours){
                if(contourArea(contour) > contourArea(Max_contour)){
                    Max_contour = contour;
                }
            }
        }
        Rect BasRect = boundingRect(Max_contour);
        rectangle(rst, BasRect, Scalar(0, 255, 0), 2);
        putText(rst, "Basket", Point(BasRect.x, BasRect.y-10), FONT_HERSHEY_SIMPLEX, 0.9, Scalar(0, 0, 255), 2);


        bool isBall = false;
        Point2f circle_center; // 圆心
        float circle_radius; // 半径
        double max_area = 0;
        int target_Contour_index = -1;
        for(size_t i = 0; i < contours.size(); i++) {
            double area = contourArea(contours[i]);
            if(area < 180)  continue; // 面积过滤
            
            //法一：宽高比过滤
            // Rect rec = boundingRect(contours[i]);
            // double width = rec.width;
            // double height = rec.height;
            // double ratio = double(min(width, height)) / max(width, height);
            // if(ratio >= 0.65 && ratio < 1.35){ // 长宽比过滤
            //     basketRect = rec;
            //     isBall = true;
            //     break;
            // }

            // 法二：圆度过滤
            double perimeter = arcLength(contours[i], true); // 计算轮廓周长
            double roundness = 4 * CV_PI * area / (perimeter * perimeter);
            if(roundness >= 0.55){
                max_area = area;
                target_Contour_index = i;
                isBall = true;
            }
        }
            if(isBall && target_Contour_index != -1) {
                minEnclosingCircle(contours[target_Contour_index], circle_center, circle_radius);
                circle(rst, circle_center, static_cast<int>(circle_radius), Scalar(255, 0, 0), 2);
                putText(rst, "Ball", Point(circle_center.x - 20, circle_center.y - circle_radius - 10), FONT_HERSHEY_SIMPLEX, 0.9, Scalar(255, 0, 0), 2);
            }

        // imshow
        imshow("Video", rst);
        imshow("Bin", bin);
        waitKey(30);   
    }
    cap.release();
    destroyAllWindows();

    return 0;
}