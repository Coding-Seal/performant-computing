#pragma once

#include <cstddef>

#include "island.hpp"
#include "zone.hpp"

namespace lab9 {

struct SearchResult {
  bool found;
  std::size_t x;
  std::size_t y;
  std::size_t cells_searched;
};

SearchResult search_zone(const Island &island, const Zone &zone);

} // namespace lab9
