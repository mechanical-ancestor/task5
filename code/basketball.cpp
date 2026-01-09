#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;
int main(){
    VideoCapture video_1("/home/w/project2/assets/origin.mp4");
    if(!video_1.isOpened()){
        cout<<"错误:无法打开视频文件!请检查路径是否正确。"<<endl;
        return -1;
    }
    Mat frame,frame_hsv;
    Size resize_size(700,500);
    Scalar hsv_min(5,116,95);
    Scalar hsv_max(30,255,255);
    while(true){
        video_1>>frame;
        if(frame.empty()){
            cout<<"视频播放完毕!"<<endl;
            break;
        }
        Mat frame_resized;
        resize(frame,frame_resized,resize_size);
        imshow("原视频",frame_resized);
        cvtColor(frame_resized,frame_hsv,COLOR_BGR2HSV);
        imshow("HSV视频",frame_hsv);
        Mat mask;
        inRange(frame_hsv,hsv_min,hsv_max,mask);
        imshow("黑白掩码视频",mask);
        vector<vector<Point>>contours;
        vector<Vec4i>hierarchy;
        findContours(mask,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
        for(size_t i=0;i<contours.size();i++){
            Point2f center;
            float radius;
            minEnclosingCircle(contours[i],center,radius);
            if(radius>10){
                circle(frame_resized,center,radius,Scalar(144,200,255),3);
                Rect rect=boundingRect(contours[i]);
                rectangle(frame_resized,rect,Scalar(0,255,0),2);
            }
        }
        imshow("识别结果",frame_resized);
        if(waitKey(30)==27){
            cout<<"用户手动退出!"<<endl;
            break;
        }
    }
    video_1.release();
    destroyAllWindows();
    return 0;
}
