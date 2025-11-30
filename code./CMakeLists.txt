cmake_minimum_required(VERSION 3.10)
project(OpenCVProject)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置运行时库路径
set(CMAKE_BUILD_RPATH "/lib/x86_64-linux-gnu")
set(CMAKE_INSTALL_RPATH "/lib/x86_64-linux-gnu")

# 查找OpenCV
find_package(OpenCV REQUIRED)

# 包含目录
include_directories(${OpenCV_INCLUDE_DIRS})

# 创建可执行文件
add_executable(test first.cpp)

# 链接库 - 明确指定库顺序
target_link_libraries(test 
    ${OpenCV_LIBS}
    pthread
    dl
)

# 设置链接器标志
set_target_properties(test PROPERTIES
    LINK_FLAGS "-Wl,--as-needed"
    INSTALL_RPATH "/lib/x86_64-linux-gnu;/usr/lib/x86_64-linux-gnu"
)