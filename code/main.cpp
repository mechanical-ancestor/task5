#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
using namespace std;
using namespace cv;

//全局变量用于滑动条回调
int h_min = 0, h_max = 20;
int s_min = 100, s_max = 255;
int v_min = 100, v_max = 255;

//形态学操作参数
int morph_size = 2;
int min_area = 500;

//窗口名称
const string window_original = "Original";
const string window_mask = "Mask";
const string window_result = "Result";
const string window_trackbar = "Trackbars";

//滑动条回调
static void on_trackbar(int,void*){}

//main函数
int main() {
	string video_path = "../assets/origin.mp4";
	VideoCapture cap(video_path);
	if (!cap.isOpened() ){
		cerr<<"Error:Cannot open video file "<<video_path<<endl;
		return -1;
	}

	//创建显示窗口
	namedWindow(window_original, WINDOW_NORMAL);
	namedWindow(window_mask, WINDOW_NORMAL);
	namedWindow(window_result, WINDOW_NORMAL);
	namedWindow(window_trackbar, WINDOW_NORMAL);

	//创建滑动条
	createTrackbar("H_min", window_trackbar, &h_min, 180, on_trackbar);
	createTrackbar("H_max", window_trackbar, &h_max, 180, on_trackbar);
	createTrackbar("S_min", window_trackbar, &s_min, 255, on_trackbar);
	createTrackbar("S_max", window_trackbar, &s_max, 255, on_trackbar);
	createTrackbar("V_min", window_trackbar, &v_min, 255, on_trackbar);
	createTrackbar("V_max", window_trackbar, &v_max, 255, on_trackbar);
	createTrackbar("Morph Size", window_trackbar, &morph_size, 10, on_trackbar);//调整形态学操作的核大小
	createTrackbar("Min Area", window_trackbar, &min_area, 5000, on_trackbar);//调整轮廓最小面积阈值

	Mat frame, hsv, mask, morph_mask;
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;

	//帧率计算变量
	double fps = cap.get(CAP_PROP_FPS);  //获取视频的帧率
	int delay = (fps > 0) ? static_cast<int>(1000 / fps) : 30;//1000/帧率=每帧需要等待的毫秒数，用于原速播放，保底30ms

	const double frame_interval = (fps > 0) ? (1000.0 / fps) : 30.0; //修改一

	while (1) {
		int64 start_tick = getTickCount();//修改二：记录开始时间戳

		cap >> frame;//读取一帧
		if (frame.empty()) {
			cout << "End of video." << endl;
			break;
		}

		//转换为hsv色彩空间
		cvtColor(frame, hsv, COLOR_BGR2HSV);

		//根据滑动条生成二值掩膜
		Scalar lower(h_min, s_min, v_min);
		Scalar upper(h_max, s_max, v_max);
		inRange(hsv, lower, upper, mask);

		//形态学操作：去除噪点、填充空间
		int ksize = max(1, morph_size * 2 + 1);  //确保正奇数 
		Mat kernel = getStructuringElement(MORPH_ELLIPSE,Size(ksize,ksize));//生成形态学操作的卷积核，MORPH_ELLIPSE-椭圆形核，Size( , )-核的宽高
		morphologyEx(mask, morph_mask, MORPH_OPEN, kernel);  //开运算：去除噪点
		morphologyEx(morph_mask, morph_mask, MORPH_CLOSE, kernel);//闭运算：填充空间

		//查找轮廓
		findContours(morph_mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		//在原图上绘制识别结果
		Mat result = frame.clone();
		for (const auto& contour : contours) {
			double area = contourArea(contour); //计算面积
			if (area < min_area)continue; //面积过滤

			//拟合最小外接圆(检测圆形）
			Point2f center;
			float radius;
			minEnclosingCircle(contour, center, radius);//拟合能完全包裹的最小圆形，输出圆心和半径

			////计算圆形度（这里感觉不用
			//double perimeter = arcLength(contour, true); //计算周长，true表示轮廓是闭合的
			//if (perimeter > 0) { 
			//	double circularity = 4 * CV_PI * area / (perimeter * perimeter); //圆度计算公式
			//	if (circularity < 0.7) continue;  //低于0.7就跳过
			//}

			//绘制外接圆和中心点
			circle(result, center, static_cast<int>(radius), Scalar(0, 255, 0), 2);//绘制外接圆（绿色）
			circle(result, center, 3, Scalar(0, 0, 255), -1);//绘制圆心（红色）
			//显示面积数值
			putText(result, "Area:" + to_string(static_cast<int>(area)),
				Point(static_cast<int>(center.x) - 30, static_cast<int>(center.y) - 20),
				FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 0), 1);
		}

		imshow(window_original, frame);
		imshow(window_mask, morph_mask);
		imshow(window_result, result);

		double process_time = (getTickCount() - start_tick) / getTickFrequency() * 1000;//修改三：计算处理本帧实际耗时

		int wait_time = max(1, static_cast<int>(frame_interval - process_time));//修改四：计算实际等待时间：标准间隔-处理耗时，兜底1毫秒

		int key = waitKey(wait_time);//修改五：将delay修改成计算后真实时间
		if (key == 27)break; //esc键
		if (key == 32) {  //空格键暂停/继续
			waitKey(0);
		}
	}

	cap.release();
	
	cv::destroyAllWindows();

	return 0;

}