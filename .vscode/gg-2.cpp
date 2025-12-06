#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap("/home/violet/task5/assets/origin.mp4"); 
    if (!cap.isOpened()) {
        std::cerr << "无法打开视频文件！" << std::endl;
        return -1;
    }

    // 强制读取为彩色
    cap.set(cv::CAP_PROP_CONVERT_RGB, 1);

    cv::Mat frame, hsv, mask, kernel;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    // 定义篮球的 HSV 颜色范围
    cv::Scalar lowerOrange(5, 100, 100);
    cv::Scalar upperOrange(20, 255, 255);

    kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // 确保 frame 是三通道
        if (frame.channels() == 1) {
            cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
        }

        // 转换到 HSV 空间
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        // 创建掩码
        cv::inRange(hsv, lowerOrange, upperOrange, mask);

        // 形态学操作
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
        cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

        // 查找轮廓
        cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // 遍历轮廓，在原图上画框
        for (const auto& contour : contours) {
            double area = cv::contourArea(contour);
            if (area < 500) continue;

            cv::Rect boundingBox = cv::boundingRect(contour);
            cv::rectangle(frame, boundingBox, cv::Scalar(0, 255, 0), 2); // 绿色框
            cv::Point center(boundingBox.x + boundingBox.width / 2,
                             boundingBox.y + boundingBox.height / 2);
            cv::circle(frame, center, 3, cv::Scalar(0, 0, 255), -1); // 红点
            cv::putText(frame, "Basketball", cv::Point(boundingBox.x, boundingBox.y - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2); // 标签
        }
        cv::namedWindow("原图 + 框", cv::WINDOW_NORMAL);
        cv::namedWindow("黑白掩码", cv::WINDOW_NORMAL);
        cv::resizeWindow("原图 + 框", 800, 600);
        cv::resizeWindow("黑白掩码", 800, 600);
        cv::imshow("原图 + 框", frame);
        cv::imshow("黑白掩码", mask);

        if (cv::waitKey(30) == 27) break; // 按 ESC 退出
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}