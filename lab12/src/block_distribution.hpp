#pragma once

#include <cstddef>
#include <vector>

namespace lab12 {

struct BlockRange {
  std::size_t begin;
  std::size_t end;
};

BlockRange block_range(int rank, int size, std::size_t num_blocks);

struct ScatterPlan {
  std::vector<int> counts;        // element count per rank, in coefficients
  std::vector<int> displacements; // offset per rank, in coefficients
};

// Per-rank MPI_Scatterv counts/displacements matching block_range(), with
// num_blocks blocks of r coefficients each distributed over size ranks.
ScatterPlan scatter_plan(int size, std::size_t num_blocks, std::size_t r);

} // namespace lab12
