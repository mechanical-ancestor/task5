#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main() {
    // 替换为你的项目根路径，例如：/home/sirius/temp/task5
    string project_root = "/home/sirius/temp/task5";
    string video_path = project_root + "/assets/origin.mp4";
    string output_path = project_root + "/assets/temp_out.mp4";

    // 1. 读取视频
    VideoCapture cap(video_path);
    if (!cap.isOpened()) {
        cerr << "错误：无法打开视频文件 " << video_path << endl;
        cerr << "请检查：1.路径是否正确 2.文件是否存在 3.文件是否损坏" << endl;
        return -1;
    }

    // 2. 获取视频参数
    int fps = cap.get(CAP_PROP_FPS);
    int width = cap.get(CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CAP_PROP_FRAME_HEIGHT);

    // 3. 定义视频写入器（输出处理后的视频）
    VideoWriter out(output_path, VideoWriter::fourcc('m','p','4','v'), fps, Size(width, height));
    if (!out.isOpened()) {
        cerr << "错误：无法创建输出视频文件 " << output_path << endl;
        cap.release();
        return -1;
    }

    // 创建调试窗口
    namedWindow("Basketball Detect", WINDOW_NORMAL);
    namedWindow("Mask", WINDOW_NORMAL);

    Mat frame, hsv, mask;
    while (cap.read(frame)) {
        // 4. 转换为HSV色彩空间（适合颜色分割）
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        // 5. 优化后的篮球橙色HSV阈值（适配视频中的篮球颜色）
        Scalar lower_orange = Scalar(5, 150, 150);
        Scalar upper_orange = Scalar(15, 255, 255);
        inRange(hsv, lower_orange, upper_orange, mask);

        // 6. 增强形态学操作去噪（更大的核尺寸）
        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
        dilate(mask, mask, kernel);
        erode(mask, mask, kernel);

        // 7. 查找轮廓
        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        // 8. 优化轮廓筛选（降低面积阈值+放宽圆形判定）
        for (auto& cnt : contours) {
            double area = contourArea(cnt);
            // 面积阈值从500降至200，适配视频中篮球的大小
            if (area > 200) {
                Rect rect = boundingRect(cnt);
                // 宽高比容忍度从0.2提至0.3，适配非正圆的篮球轮廓
                if (abs(1.0 - (double)rect.width/rect.height) < 0.3) {
                    rectangle(frame, rect, Scalar(0, 255, 0), 2); // 绿色识别框
                    // 在框上方标注"Basketball"
                    putText(frame, "Basketball", Point(rect.x, rect.y-5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
                }
            }
        }

        // 9. 显示调试窗口并写入视频
        imshow("Mask", mask); // 掩码窗口，查看颜色分割效果
        imshow("Basketball Detect", frame);
        out.write(frame);

        // 按ESC退出，按空格暂停
        int key = waitKey(1);
        if (key == 27) break;
        if (key == 32) waitKey(0);
    }

    // 释放资源
    cap.release();
    out.release();
    destroyAllWindows();
    cout << "视频处理完成，输出文件：" << output_path << endl;
    return 0;
}

