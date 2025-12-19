cmake_minimum_required(VERSION 3.10)
project(final_task5)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找OpenCV库
find_package(OpenCV REQUIRED)

# 添加头文件目录
include_directories(${OpenCV_INCLUDE_DIRS})

# 添加可执行文件
add_executable(final_task5 src/main.cpp)

# 链接OpenCV库
target_link_libraries(final_task5 ${OpenCV_LIBS})
