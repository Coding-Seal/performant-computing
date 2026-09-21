#pragma once

#include <cstddef>

namespace lab9 {

struct Island {
  std::size_t width;
  std::size_t height;
  std::size_t treasure_x;
  std::size_t treasure_y;
};

Island make_island(std::size_t width, std::size_t height, unsigned seed);

} // namespace lab9
