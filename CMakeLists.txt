cmake_minimum_required(VERSION 3.10)
project(basketball_detect)
set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(basketball_detect basketball_detect.cpp)
target_link_libraries(basketball_detect ${OpenCV_LIBS})

