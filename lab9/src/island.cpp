#include "island.hpp"

#include <cstddef>
#include <random>

namespace lab9 {

Island make_island(std::size_t width, std::size_t height, unsigned seed) {
  std::mt19937 rng(seed);
  std::uniform_int_distribution<std::size_t> x_dist(0, width - 1);
  std::uniform_int_distribution<std::size_t> y_dist(0, height - 1);
  return Island{.width = width,
                .height = height,
                .treasure_x = x_dist(rng),
                .treasure_y = y_dist(rng)};
}

} // namespace lab9
