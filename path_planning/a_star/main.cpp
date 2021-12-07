#include <iostream>

#include "a_star.hpp"

using namespace cpp_robotics::path_planning;

int main() {
  std::cout << "Hello, World!" << std::endl;
  //! load map data
  cv::Mat map_data = cv::imread("../maps/map2.png", CV_8UC1);
  if (map_data.empty()) {
    std::cerr << "load map image fail." << std::endl;
    return -1;
  }
  //! a_star
  std::cout << map_data.size() << std::endl;
  a_star::Astar astar(a_star::Heuristic::euclidean, true);
  astar.init(map_data);
  auto path = astar.findPath({25,25}, {480, 480});
  return 0;
}