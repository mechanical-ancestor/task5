#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;
int h_min = 5, h_max = 20;
int s_min = 100, s_max = 255;
int v_min = 80, v_max = 255;
const string window_name = "Basketball Detection (3-Screen)";
void on_trackbar(int, void*) {}

int main() {
    VideoCapture cap("/home/aurora/task5/assets/origin.mp4");
    if (!cap.isOpened()) {
        cout << "无法打开视频文件！请检查路径是否正确" << endl;
        return -1;
    }
    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(CAP_PROP_FPS);
    Size frame_size(frame_width, frame_height);
    VideoWriter output_video("/home/aurora/task5/assets/result_video.avi", 
                             VideoWriter::fourcc('M','J','P','G'), 
                             fps, 
                             Size(frame_width*3, frame_height));
    namedWindow(window_name, WINDOW_NORMAL);
    resizeWindow(window_name, frame_width*3, frame_height + 100);
    createTrackbar("H_min", window_name, &h_min, 179, on_trackbar);
    createTrackbar("H_max", window_name, &h_max, 179, on_trackbar);
    createTrackbar("S_min", window_name, &s_min, 255, on_trackbar);
    createTrackbar("S_max", window_name, &s_max, 255, on_trackbar);
    createTrackbar("V_min", window_name, &v_min, 255, on_trackbar);
    createTrackbar("V_max", window_name, &v_max, 255, on_trackbar);
    Mat frame, hsv, mask, result_frame, mask_color;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;
        result_frame = frame.clone();
        mask_color = Mat::zeros(frame_size, CV_8UC3);
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        Scalar lower_orange = Scalar(h_min, s_min, v_min);
        Scalar upper_orange = Scalar(h_max, s_max, v_max);
        inRange(hsv, lower_orange, upper_orange, mask);
        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
        erode(mask, mask, kernel);
        dilate(mask, mask, kernel);
        for (int i = 0; i < frame_height; i++) {
            for (int j = 0; j < frame_width; j++) {
                if (mask.at<uchar>(i, j) == 255) {
                    mask_color.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
                }
            }
        }
        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        for (size_t i = 0; i < contours.size(); i++) {
            Rect rect = boundingRect(contours[i]);
            if (rect.width > 30 && rect.height > 30 && abs(1.0 - (double)rect.width/rect.height) < 0.2) {
                rectangle(result_frame, rect, Scalar(255, 0, 0), 2);
                putText(result_frame, "basketball", Point(rect.x, rect.y-10), 
                        FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);
            }
        }
        Mat combined_frame(Size(frame_width*3, frame_height), CV_8UC3);
        frame.copyTo(combined_frame(Rect(0, 0, frame_width, frame_height)));
        mask_color.copyTo(combined_frame(Rect(frame_width, 0, frame_width, frame_height)));
        result_frame.copyTo(combined_frame(Rect(frame_width*2, 0, frame_width, frame_height)));
        putText(combined_frame, "Original", Point(20, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
        putText(combined_frame, "Mask", Point(frame_width+20, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
        putText(combined_frame, "Result", Point(frame_width*2+20, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
        imshow(window_name, combined_frame);
        output_video.write(combined_frame);
        if (waitKey(30) == 27) break;
    }
    cap.release();
    output_video.release();
    destroyAllWindows();
    cout << "三屏效果视频生成完成！路径：/home/aurora/task5/assets/result_video.avi" << endl;
    return 0;
}

