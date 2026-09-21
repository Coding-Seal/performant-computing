#include "treasure_hunt_sequential.hpp"
#include "island.hpp"
#include "search.hpp"
#include "zone.hpp"

namespace lab9 {

SearchResult treasure_hunt_sequential(const Island &island) {
  return search_zone(island, Zone{.x_begin = 0,
                                  .x_end = island.width,
                                  .y_begin = 0,
                                  .y_end = island.height});
}

} // namespace lab9
