#include <opencv2/opencv.hpp>
#include <iostream>
#include "utils.h"
#include<vector>

using namespace cv;
using namespace std;

int main() {
    Mat dst,M,img1;
   Mat img=imread("/home/gan/opencv/photo.jpg");//绝对路径   IMREAD_GRAYSCALE输出灰度图
    if(img.empty()){
    cout<<"当前图片不存在"<<endl;
    return -1;
   }

   //图像旋转
   int width=img.cols;
   int height=img.rows;
   M=getRotationMatrix2D(Point2f(width/2,height/2),45,1.0);
   warpAffine(img,dst,M,img.size(),INTER_LINEAR,0,Scalar(0,0,0));
   
    //但RGB输出
    Mat RGB_img;
    cvtColor(img,RGB_img,COLOR_BGR2RGB);

   //描边
    Mat gray,binary;//gray 灰度图  binary 二值化图
   cvtColor(img,gray,COLOR_BGR2GRAY);
      
     threshold(gray,binary,128,255,THRESH_BINARY_INV);//二值化操作
     Mat kernel=getStructuringElement(MORPH_RECT,Size(5,5));
      //膨胀操作
     Mat dilated;
     dilate(binary,dilated,kernel);
      
     Mat dilated_inv;
     bitwise_not(dilated,dilated_inv);

     //描边操作
     Mat stroke=img.clone();
     vector<vector<Point>>contours;
     findContours(dilated,contours,RETR_TREE,CHAIN_APPROX_SIMPLE);
     drawContours(stroke,contours,-1,Scalar(0,0,255),3);
   
   //跟换背景颜色
   //创建白色背景的掩模（阈值可根据需要调整）
     Mat mask;
    threshold(gray,mask, 200, 255,THRESH_BINARY);
    Mat result;
    result=img.clone();
   result.setTo(Scalar(0,0,0),mask);  // 设置为黑色 [B, G, R]

     imshow("RGB输出",RGB_img);
     imshow("灰度图",gray);
     imshow("旋转演示",dst);
     imshow("黑色背景",result);
     imshow("描边",stroke);
     imshow("原图",img);
     waitKey(0);
    return 0;
}