cmake_minimum_required(VERSION 3.5.1)
project(linear_quadratic_regulator)

set(CMAKE_CXX_STANDARD 11)

find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PythonLibs 2.7)

include_directories(${OpenCV_LIBS} ${PYTHON_INCLUDE_DIRS})

add_executable(lqr src/lqr_steer_control.cpp)
target_link_libraries(lqr ${OpenCV_LIBS})

add_executable(lqr_full src/lqr_speed_steer_control.cpp)
target_link_libraries(lqr_full ${OpenCV_LIBS})
