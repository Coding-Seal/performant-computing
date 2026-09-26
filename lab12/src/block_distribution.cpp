#include "block_distribution.hpp"

#include <algorithm>
#include <cstddef>

namespace lab12 {

BlockRange block_range(int rank, int size, std::size_t num_blocks) {
  const std::size_t base = num_blocks / static_cast<std::size_t>(size);
  const std::size_t remainder = num_blocks % static_cast<std::size_t>(size);
  const auto r = static_cast<std::size_t>(rank);

  const std::size_t begin = (r * base) + std::min(r, remainder);
  const std::size_t end = begin + base + (r < remainder ? 1 : 0);
  return {.begin = begin, .end = end};
}

ScatterPlan scatter_plan(int size, std::size_t num_blocks, std::size_t r) {
  ScatterPlan plan;
  plan.counts.resize(static_cast<std::size_t>(size));
  plan.displacements.resize(static_cast<std::size_t>(size));

  for (int rank = 0; rank < size; ++rank) {
    const BlockRange range = block_range(rank, size, num_blocks);
    plan.counts[static_cast<std::size_t>(rank)] =
        static_cast<int>((range.end - range.begin) * r);
    plan.displacements[static_cast<std::size_t>(rank)] =
        static_cast<int>(range.begin * r);
  }
  return plan;
}

} // namespace lab12
