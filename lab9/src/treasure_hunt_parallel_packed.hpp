#pragma once

#include <mpi.h>

#include "island.hpp"
#include "search.hpp"

namespace lab9 {

SearchResult treasure_hunt_parallel_packed(const Island &island, MPI_Comm comm);

} // namespace lab9
