#include "zone.hpp"
#include <cstddef>
#include <vector>

namespace lab9 {

std::vector<Zone> partition_into_zones(std::size_t width, std::size_t height,
                                       std::size_t num_zones) {
  std::vector<Zone> zones;
  if (num_zones == 0) {
    return zones;
  }
  zones.reserve(num_zones);

  const std::size_t base_rows = height / num_zones;
  const std::size_t remainder = height % num_zones;

  std::size_t y = 0;
  for (std::size_t i = 0; i < num_zones; ++i) {
    const std::size_t rows = base_rows + (i < remainder ? 1 : 0);
    const std::size_t y_end = y + rows;
    zones.push_back(
        Zone{.x_begin = 0, .x_end = width, .y_begin = y, .y_end = y_end});
    y = y_end;
  }
  return zones;
}

} // namespace lab9
