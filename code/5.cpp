#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(){
   VideoCapture  vedio_1("/home/wrt/视频/origin.mp4");

Mat a,vedio_2_hsv;
Size change_size(700,500);//调一下视频大小


Scalar min(5,116,95);//表示最大最小阀值
Scalar max(30,255,255);


while(true){
    vedio_1 >> a;
    if (a.empty()) break;
    
    Mat a2;
resize(a,a2,change_size);
imshow("原视频",a2);
cvtColor(a2,vedio_2_hsv,COLOR_BGR2HSV);//生成hsv
Mat mask;
inRange(vedio_2_hsv,min,max,mask);//生成黑白掩码 



vector<vector<Point>> contours;
vector<Vec4i> hierarchy;
findContours(mask,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);



for(size_t i=0;i<contours.size();i++){
    Point2f c;
    float r;
    minEnclosingCircle(contours[i],c,r);
    circle(a2,c,r,Scalar(144,200,255),2);
                                                    //将篮球标记
 
    for(size_t i=0;i<contours.size();i++){
    Rect z=boundingRect(contours[i]);
    rectangle(a2,z,Scalar(0,255,0),2);
}
}

imshow("hsv vedio",vedio_2_hsv);
imshow("黑白掩码vedio",mask);
imshow("result",a2);
waitKey(30);
}
destroyAllWindows();
return 0;
}