cmake_minimum_required(VERSION 3.10)
project(src)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(src src.cpp)

target_link_libraries(src ${OpenCV_LIBS})
