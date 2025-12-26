#include<iostream>
#include<iomanip>
#include<opencv2/opencv.hpp>
#include<vector>
using namespace std;
using namespace cv;
int hMin=5,sMin=100,vMin=80;
int hMax=20,sMax=255,vMax=255;
Mat src,hsv;

int main(){
     //手动调整
    namedWindow("Control test",WINDOW_NORMAL);
    //创建滑调
    createTrackbar("H Min","Control test",&hMin,179,NULL);
    createTrackbar("H Max","Control test",&hMax,179,NULL);
    createTrackbar("S Min","Control test",&sMin,255,NULL);
    createTrackbar("S Max","Control test",&sMax,255,NULL);
    createTrackbar("V Min","Control test",&vMin,255,NULL);
    createTrackbar("v Max","Control test",&vMax,255,NULL);
    
    waitKey(100); // 延时200ms，确保窗口被系统识别
    //使用系统命令全屏
    system("wmctrl -r \"Control test\" -b add,fullscreen");

    VideoCapture video;
    video.open("/home/gan/opencv/test.mp4");
    cout<<"视频帧率："<<video.get(CAP_PROP_FPS)<<endl;
    cout<<"视频高度："<<video.get(CAP_PROP_FRAME_HEIGHT)<<endl;
    double time=0;
    while(true){
        Mat frame;
        video>>frame;
        if(frame.empty()) {
            cout<<"视频已播放完毕!"<<endl;
            break;
        }
        //flip(frame,frame,1);
        //  resize(frame,frame,Size(),0.5,0.5,INTER_LINEAR);
        //识别橙色
        //设立HSV值
        Mat  hsving;
        GaussianBlur(frame,frame,Size(3,3),0);
        cvtColor(frame,hsving,COLOR_BGR2HSV);
        Scalar lowerOrange=Scalar(hMin,sMin,vMin);
        Scalar upperOrange=Scalar(hMax,sMax,vMax);
         //掩膜创建
        Mat mask;
        inRange(hsving,lowerOrange,upperOrange,mask);
        //形态学优化
        Mat img;
        Mat kernel1 = getStructuringElement(MORPH_ELLIPSE,Size(3,3));
        Mat kernel2 = getStructuringElement(MORPH_ELLIPSE,Size(7,7));
        morphologyEx(mask,img,MORPH_OPEN,kernel1);
        //morphologyEx(img,img,MORPH_CLOSE,kernel);
        dilate(img,img,kernel2);

        //标注
        vector<vector<Point>>contours;
        vector<Vec4i>hierarchy;

        findContours(mask,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

        Mat result=frame.clone();

        for(size_t i=0;i<contours.size();i++){
            double area=contourArea(contours[i]);
            if(area<500) continue;
            
            Rect boundingBox=boundingRect(contours[i]);
            rectangle(result,boundingBox,Scalar(255,0,0),2);

           string text="name:baskball";
           
           int textY =max(10,boundingBox.y-5);
          putText(result,text,Point(boundingBox.x,textY),FONT_HERSHEY_SIMPLEX,0.5,Scalar(255,255,255),1);
        }
         

        // 拼接图片
        Mat frame_copy,img_copy,result_copy;
        resize(frame,frame_copy,Size(),0.5,0.5);
        resize(img,img_copy,Size(),0.5,0.5);
        resize(result,result_copy,Size(),0.5,0.5);

        Mat combineWindows;
        cvtColor(img_copy,img_copy,COLOR_GRAY2BGR);
        hconcat(frame_copy,img_copy,combineWindows);
        hconcat(combineWindows,result_copy,combineWindows);
        imshow("Control test",combineWindows);
        time++;
        if(waitKey(1000/video.get(CAP_PROP_FPS))==27){
            cout<<"手动退出！"<<endl;
            break;
        }
    }
    cout<<"视频时间为:"<<fixed<<setprecision(4)<<time/video.get(CAP_PROP_FPS)<<"秒"<<endl;
    video.release();
    destroyAllWindows();
    return 0;
}