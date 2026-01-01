#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main() {
    VideoCapture cap("/home/aurora/task5/assets/origin.mp4", CAP_FFMPEG);
    if (!cap.isOpened()) {
        cout << "无法打开视频文件！" << endl;
        return -1;
    }
    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(CAP_PROP_FPS);
    VideoWriter output_video("result_video.avi", VideoWriter::fourcc('M','J','P','G'), fps, Size(frame_width, frame_height));
    Mat frame, hsv, mask;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            cap.set(CAP_PROP_POS_FRAMES, 0);
            continue;
        }
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        Scalar lower_orange = Scalar(5, 80, 80);
        Scalar upper_orange = Scalar(35, 255, 255);
        inRange(hsv, lower_orange, upper_orange, mask);
        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
        erode(mask, mask, kernel);
        dilate(mask, mask, kernel);
        dilate(mask, mask, kernel);
        erode(mask, mask, kernel);
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        for (size_t i = 0; i < contours.size(); i++) {
            Rect rect = boundingRect(contours[i]);
            double area = contourArea(contours[i]);
            if (area > 50 && (double)rect.width/rect.height > 0.7 && (double)rect.width/rect.height < 1.3) {
                rectangle(frame, rect, Scalar(0, 0, 255), 2);
            }
        }
        imshow("Basketball Detection", frame);
        imshow("Mask", mask);
        output_video.write(frame);
        if (waitKey(30) == 27) break;
    }
    cap.release();
    output_video.release();
    waitKey(0);
    destroyAllWindows();
    cout << "视频处理完成！" << endl;
    return 0;
}
