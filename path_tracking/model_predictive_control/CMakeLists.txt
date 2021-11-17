cmake_minimum_required(VERSION 3.5.1)
project(model_predictive_control)

set(CMAKE_CXX_STANDARD 11)

find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PythonLibs 2.7)

include_directories(${OpenCV_LIBS} ${PYTHON_INCLUDE_DIRS})

add_executable(mptg src/model_predictive_trajectory_generator.cpp)
target_link_libraries(mptg ${OpenCV_LIBS})
