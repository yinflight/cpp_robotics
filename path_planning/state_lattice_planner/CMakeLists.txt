cmake_minimum_required(VERSION 3.5.1)
project(state_lattice_planner)

set(CMAKE_CXX_STANDARD 11)

find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PythonLibs 2.7)

include_directories(${OpenCV_LIBS} ${PYTHON_INCLUDE_DIRS})

add_executable(lattice_planner src/state_lattice_planner.cpp)
target_link_libraries(lattice_planner ${OpenCV_LIBS})

