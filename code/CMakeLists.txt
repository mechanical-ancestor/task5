cmake_minimum_required(VERSION 3.22)
project(OpenCVProject)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 寻找OpenCV包
find_package(OpenCV REQUIRED)

# 输出OpenCV信息
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")
message(STATUS "OpenCV include path: ${OpenCV_INCLUDE_DIRS}")

# 包含头文件目录
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

# 添加可执行文件
add_executable(code code.cpp)
add_executable(main code1.cpp)

# 链接OpenCV库
target_link_libraries(code ${OpenCV_LIBS})
target_link_libraries(main ${OpenCV_LIBS})

# 设置编译属性
target_compile_options(code PRIVATE -Wall -Wextra -O2)
target_compile_options(main PRIVATE -Wall -Wextra -O2)