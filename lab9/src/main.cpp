#include <cstdio>
#include <cstdlib>

#include <mpi.h>

#include "island.hpp"
#include "search.hpp"
#include "treasure_hunt_parallel.hpp"
#include "treasure_hunt_parallel_packed.hpp"
#include "treasure_hunt_sequential.hpp"

namespace {

bool matches_treasure(const lab9::SearchResult &result,
                      const lab9::Island &island) {
  return result.found && result.x == island.treasure_x &&
         result.y == island.treasure_y;
}

} // namespace

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  const std::size_t width = argc > 1 ? std::strtoul(argv[1], nullptr, 10) : 500;
  const std::size_t height =
      argc > 2 ? std::strtoul(argv[2], nullptr, 10) : 500;
  const unsigned seed =
      argc > 3 ? static_cast<unsigned>(std::strtoul(argv[3], nullptr, 10)) : 42;

  const lab9::Island island = lab9::make_island(width, height, seed);

  MPI_Barrier(MPI_COMM_WORLD);
  const double typed_start = MPI_Wtime();
  const lab9::SearchResult typed_result =
      lab9::treasure_hunt_parallel(island, MPI_COMM_WORLD);
  const double typed_seconds = MPI_Wtime() - typed_start;

  MPI_Barrier(MPI_COMM_WORLD);
  const double packed_start = MPI_Wtime();
  const lab9::SearchResult packed_result =
      lab9::treasure_hunt_parallel_packed(island, MPI_COMM_WORLD);
  const double packed_seconds = MPI_Wtime() - packed_start;

  int exit_code = 0;
  if (rank == 0) {
    const double sequential_start = MPI_Wtime();
    const lab9::SearchResult sequential_result =
        lab9::treasure_hunt_sequential(island);
    const double sequential_seconds = MPI_Wtime() - sequential_start;

    const bool ok = matches_treasure(typed_result, island) &&
                    matches_treasure(packed_result, island) &&
                    matches_treasure(sequential_result, island);

    std::printf("island=%zux%zu processes=%d treasure=(%zu,%zu)\n", width,
                height, size, island.treasure_x, island.treasure_y);
    std::printf("sequential:     %.6fs\n", sequential_seconds);
    std::printf("parallel typed: %.6fs (speedup=%.2f, efficiency=%.2f)\n",
                typed_seconds, sequential_seconds / typed_seconds,
                sequential_seconds / typed_seconds /
                    static_cast<double>(size - 1 > 0 ? size - 1 : 1));
    std::printf("parallel packed:%.6fs (speedup=%.2f, efficiency=%.2f)\n",
                packed_seconds, sequential_seconds / packed_seconds,
                sequential_seconds / packed_seconds /
                    static_cast<double>(size - 1 > 0 ? size - 1 : 1));
    std::printf("result correct: %s\n", ok ? "yes" : "no");

    exit_code = ok ? 0 : 1;
  }

  MPI_Finalize();
  return exit_code;
}
