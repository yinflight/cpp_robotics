#ifndef A_STAR_A_STAR_HPP
#define A_STAR_A_STAR_HPP

#include <iostream>
#include <array>
#include <vector>
#include <queue>
#include <functional>
#include <opencv2/opencv.hpp>

/**
 * 坐标系: ------> x
 *        |
 *        v
 *        y
 * 角度为:x轴0度,顺时针(-180~180)
 */
namespace cpp_robotics {
namespace path_planning {
namespace a_star {

using Coord = cv::Point;
using Score = std::pair<int64_t, Coord>;
using HeuristicFunction = std::function<int64_t(Coord, Coord)>;

enum State {
  FREE = 0,
  OBSTACLE
};
// cell struct
struct Cell {
  State state = State::FREE;
  bool visited = false;
  // The numeric_limits class template provides a standardized way to query
  // various properties of arithmetic types (e.g. the largest possible value
  // for type int is std::numeric_limits<int>::max()).
  int64_t cost_g = std::numeric_limits<int64_t>::max();
  Coord parent;
};

struct CompareScore {
  //! we want the priority_queue to be ordered from smaller to larger
  bool operator()(const Score &a, const Score &b) {
    return a.first > b.first;
  }
};

class Heuristic {
 public:
  static int64_t manhattan(Coord source, Coord target) {
    auto delta = Coord((source.x - target.x), (source.y - target.y));
    return static_cast<int64_t>(10 * (abs(delta.x) + abs(delta.y)));
  }

  static int64_t euclidean(Coord source, Coord target) {
    auto delta = Coord((source.x - target.x), (source.y - target.y));
    return static_cast<int64_t>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
  }

  static int64_t octagonal(Coord source, Coord target) {
    auto delta = Coord(abs(source.x - target.x), abs(source.y - target.y));
    return 10 * (delta.x + delta.y) + (-6) * std::min(delta.x, delta.y);
  }
};

class Astar {
 public:
  Astar(HeuristicFunction heuristic, bool display_flag = false) {
    // When used as an argument in a std::bind expression, the placeholder objects are stored in
    // the generated function object, and when that function object is invoked with unbound arguments,
    // each placeholder _N is replaced by the corresponding Nth unbound argument.
    heuristic_ = std::bind(heuristic, std::placeholders::_1, std::placeholders::_2);
    display_flag_ = display_flag;
    directions_ = {
      Coord(-1, -1), Coord(0, -1), Coord(1, -1),
      Coord(-1, 0),                Coord(1, 0),
      Coord(-1, 1),  Coord(0, 1),  Coord(1, 1)
    };
    directions_cost_ = {
      14, 10, 14,
      10,     10,
      14, 10, 14
    };
  }

  void init(const cv::Mat &map) {
    display_img_ = map.clone();
    map_.resize(map.rows, std::vector<Cell>(map.cols));
    for (auto y = 0; y < map.rows; ++y) {
      for (auto x = 0; x < map.cols; ++x) {
        if (map.at<uchar>(y, x) > 250) {
          map_[y][x].state = State::FREE;
        } else {
          map_[y][x].state = State::OBSTACLE;
        }
      }
    }
  }

  std::vector<Coord> findPath(Coord start, Coord end) {
    //! reset
    reset();
    //! check start and end point legal
    if (map_[start.y][start.x].state == State::OBSTACLE) {
      std::cerr << "start point in obstacle." << std::endl;
      return std::vector<Coord>();
    }
    if (map_[end.y][end.x].state == State::OBSTACLE) {
      std::cerr << "end point in obstacle." << std::endl;
      return std::vector<Coord>();
    }
    //! find path
    bool found_path = false;
    // A priority queue is a container adaptor that provides constant time lookup of the largest
    // (by default) element, at the expense of logarithmic insertion and extraction.
    std::priority_queue<Score, std::vector<Score>, CompareScore> open_list;
    open_list.push({0, start});
    map_[start.y][start.x].cost_g = 0;
    while (!open_list.empty()) {
      auto current_corrd = open_list.top().second;
      open_list.pop();
      //! finish check
      if (current_corrd == end) {
        found_path = true;
        break;
      }
      map_[current_corrd.y][current_corrd.x].visited = true;
      //! search
      for (auto i = 0; i < directions_.size(); ++i) {
        auto next_coord = current_corrd + directions_[i];
        if (isOutOfMap(next_coord) ||
          map_[next_coord.y][next_coord.x].visited ||
          map_[next_coord.y][next_coord.x].state == State::OBSTACLE) {
          continue;
        }
        auto new_cost_g = map_[current_corrd.y][current_corrd.x].cost_g + directions_cost_[i];
        if (new_cost_g < map_[next_coord.y][next_coord.x].cost_g) {
          auto cost_h = heuristic_(next_coord, end);
          open_list.push({new_cost_g + cost_h, next_coord});
          map_[next_coord.y][next_coord.x].cost_g = new_cost_g;
          map_[next_coord.y][next_coord.x].parent = current_corrd;
        }
      }
      //! draw open_list
      if (display_flag_) {
        drawOpenList(open_list);
      }
    }
    //! get path
    std::vector<Coord> path;
    if (!found_path) {
      std::cerr << "find path fail." << std::endl;
    } else {
      auto coord = end;
      while (map_[coord.y][coord.x].parent != Coord(0, 0)) {
        path.emplace_back(coord);
        coord = map_[coord.y][coord.x].parent;
      }
      path.emplace_back(start);
      std::reverse(path.begin(), path.end());
      std::cout << "found path size:" << path.size() << std::endl;
    }
    //! draw path
    if (display_flag_) {
      drawPath(path);
    }
    return path;
  }

 private:
  void reset() {
    for (auto y = 0; y < map_.size(); ++y) {
      for (auto x = 0; x < map_.front().size(); ++x) {
        map_[y][x].visited = false;
      }
    }
  }

  bool isOutOfMap(Coord coord) {
    if (coord.x < 0 || coord.x >= map_.front().size() ||
      coord.y < 0 || coord.y >= map_.size()) {
      return true;
    }
    return false;
  }

  void drawOpenList(const std::priority_queue<Score, std::vector<Score>, CompareScore> &open_list) {
    auto op = open_list;
    while (!op.empty()) {
      auto coord = op.top().second;
      op.pop();
      display_img_.at<uchar>(coord.y, coord.x) = 192;
    }
    cv::namedWindow("a_star", 0);
    cv::imshow("a_star", display_img_);
    cv::waitKey(1);
  }

  void drawPath(const std::vector<Coord> &path) {
    for (auto coord : path) {
      display_img_.at<uchar>(coord.y, coord.x) = 64;
      cv::namedWindow("a_star", 0);
      cv::imshow("a_star", display_img_);
      cv::waitKey(10);
    }
    cv::namedWindow("a_star", 0);
    cv::imshow("a_star", display_img_);
    cv::waitKey(0);
  }

 private:
  std::array<Coord, 8> directions_;
  std::array<int, 8> directions_cost_;
  std::vector<std::vector<Cell>> map_;
  HeuristicFunction heuristic_;

  //! for display
  cv::Mat display_img_;
  bool display_flag_ = false;
};

} // namespace a_star
} // namespace path_planning
} // namespace cpp_robotics

#endif //A_STAR_A_STAR_HPP
