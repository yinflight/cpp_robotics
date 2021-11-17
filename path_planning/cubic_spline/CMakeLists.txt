cmake_minimum_required(VERSION 3.5.1)
project(cubic_spline)

set(CMAKE_CXX_STANDARD 11)

find_package(PythonLibs 2.7)

include_directories(include lib ${PYTHON_INCLUDE_DIRS})

add_executable(cubic_spline test/main.cpp)
target_link_libraries(cubic_spline ${PYTHON_LIBRARIES})
