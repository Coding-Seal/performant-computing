#pragma once

#include <cstddef>
#include <vector>

namespace lab9 {

struct Zone {
  std::size_t x_begin;
  std::size_t x_end;
  std::size_t y_begin;
  std::size_t y_end;
};

std::vector<Zone> partition_into_zones(std::size_t width, std::size_t height,
                                       std::size_t num_zones);

} // namespace lab9
