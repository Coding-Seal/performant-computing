#include "search.hpp"
#include "island.hpp"
#include "zone.hpp"
#include <cstddef>

namespace lab9 {

SearchResult search_zone(const Island &island, const Zone &zone) {
  std::size_t cells_searched = 0;
  for (std::size_t y = zone.y_begin; y < zone.y_end; ++y) {
    for (std::size_t x = zone.x_begin; x < zone.x_end; ++x) {
      ++cells_searched;
      if (x == island.treasure_x && y == island.treasure_y) {
        return SearchResult{
            .found = true, .x = x, .y = y, .cells_searched = cells_searched};
      }
    }
  }
  return SearchResult{
      .found = false, .x = 0, .y = 0, .cells_searched = cells_searched};
}

} // namespace lab9
