
cmake_minimum_required(VERSION 3.10)

project(BasketballDetection)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 查找 OpenCV 库
find_package(OpenCV REQUIRED)

# 输出找到的 OpenCV 版本信息（可选，用于调试）
message(STATUS "Found OpenCV ${OpenCV_VERSION}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

add_executable(basketball_app gg-2.cpp)

# 链接 OpenCV 库到可执行文件
target_link_libraries(basketball_app ${OpenCV_LIBS})

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(basketball_app PRIVATE -Wall -Wextra -O3)
endif()
