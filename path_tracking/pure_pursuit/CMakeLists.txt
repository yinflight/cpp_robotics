cmake_minimum_required(VERSION 3.5.1)
project(pure_pursuit)

set(CMAKE_CXX_STANDARD 11)

find_package(PythonLibs 2.7)

include_directories(${PYTHON_INCLUDE_DIRS})

add_executable(pure_pursuit main.cpp)
target_link_libraries(pure_pursuit ${PYTHON_LIBRARIES})
