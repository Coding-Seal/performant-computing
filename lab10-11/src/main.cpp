#include <cstdio>
#include <cstdlib>

#include <mpi.h>

#include "barrier.hpp"

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  const std::size_t iterations =
      argc > 1 ? std::strtoul(argv[1], nullptr, 10) : 20000;

  MPI_Barrier(MPI_COMM_WORLD);
  const double dichotomy_start = MPI_Wtime();
  for (std::size_t i = 0; i < iterations; ++i) {
    lab10_11::barrier_dichotomy(MPI_COMM_WORLD);
  }
  const double dichotomy_end = MPI_Wtime();

  MPI_Barrier(MPI_COMM_WORLD);
  const double builtin_start = MPI_Wtime();
  for (std::size_t i = 0; i < iterations; ++i) {
    lab10_11::barrier_builtin(MPI_COMM_WORLD);
  }
  const double builtin_end = MPI_Wtime();

  if (rank == 0) {
    const double dichotomy_us = (dichotomy_end - dichotomy_start) /
                                static_cast<double>(iterations) * 1e6;
    const double builtin_us =
        (builtin_end - builtin_start) / static_cast<double>(iterations) * 1e6;

    std::printf("processes=%d iterations=%zu\n", size, iterations);
    std::printf("barrier_dichotomy: %.3f us/call\n", dichotomy_us);
    std::printf("MPI_Barrier:       %.3f us/call\n", builtin_us);
    std::printf("efficiency (MPI_Barrier / dichotomy): %.3f\n",
                builtin_us / dichotomy_us);
  }

  MPI_Finalize();
  return 0;
}
