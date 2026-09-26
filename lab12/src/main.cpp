#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <vector>

#include <mpi.h>

#include "polynomial_parallel.hpp"
#include "polynomial_sequential.hpp"

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank = 0;
  int size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (argc != 4) {
    if (rank == 0) {
      std::fprintf(stderr, "usage: %s <s> <r> <iterations>\n", argv[0]);
    }
    MPI_Finalize();
    return 1;
  }

  const auto s = static_cast<std::size_t>(std::strtoul(argv[1], nullptr, 10));
  const auto r = static_cast<std::size_t>(std::strtoul(argv[2], nullptr, 10));
  const auto iterations =
      static_cast<std::size_t>(std::strtoul(argv[3], nullptr, 10));
  constexpr double x = 1.0001;

  // Only rank 0 ever holds the full coefficient array; every other rank
  // gets just its own blocks below, via scatter_coefficients().
  double a0 = 0.0;
  std::vector<double> coeffs;
  if (rank == 0) {
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(-5.0, 5.0);
    a0 = dist(rng);
    coeffs.resize(s * r);
    for (double &c : coeffs) {
      c = dist(rng);
    }
  }

  const std::vector<double> local_coeffs =
      lab12::scatter_coefficients(coeffs, s, r, MPI_COMM_WORLD);

  // Untimed warm-up so neither phase below is penalized by page faults /
  // cold cache just for running first.
  if (rank == 0) {
    lab12::evaluate_polynomial_sequential(a0, coeffs, r, x);
  }
  lab12::evaluate_polynomial_parallel(a0, local_coeffs, s, r, x,
                                      MPI_COMM_WORLD);

  // Timed on rank 0 alone: with every rank running the same reference loop
  // at once, the other ranks would contend for memory bandwidth and inflate
  // this baseline, especially at higher process counts.
  double sequential_result = 0.0;
  double sequential_seconds = 0.0;
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) {
    const double sequential_start = MPI_Wtime();
    for (std::size_t it = 0; it < iterations; ++it) {
      sequential_result =
          lab12::evaluate_polynomial_sequential(a0, coeffs, r, x);
    }
    sequential_seconds = MPI_Wtime() - sequential_start;
  }

  double parallel_result = 0.0;
  MPI_Barrier(MPI_COMM_WORLD);
  const double parallel_start = MPI_Wtime();
  for (std::size_t it = 0; it < iterations; ++it) {
    parallel_result = lab12::evaluate_polynomial_parallel(a0, local_coeffs, s,
                                                          r, x, MPI_COMM_WORLD);
  }
  const double parallel_seconds = MPI_Wtime() - parallel_start;

  if (rank == 0) {
    const double speedup = sequential_seconds / parallel_seconds;
    const double efficiency = speedup / static_cast<double>(size);
    const double tolerance = 1e-9 * std::max(1.0, std::abs(sequential_result));
    const bool matches =
        std::abs(parallel_result - sequential_result) < tolerance;

    std::printf("processes=%d s=%zu r=%zu iterations=%zu\n", size, s, r,
                iterations);
    std::printf("sequential: %.6fs total, %.3f us/call, result=%.10f\n",
                sequential_seconds,
                sequential_seconds / static_cast<double>(iterations) * 1e6,
                sequential_result);
    std::printf(
        "parallel:   %.6fs total, %.3f us/call, result=%.10f (matches: %s)\n",
        parallel_seconds,
        parallel_seconds / static_cast<double>(iterations) * 1e6,
        parallel_result, matches ? "yes" : "no");
    std::printf("speedup=%.3f efficiency=%.3f\n", speedup, efficiency);
  }

  MPI_Finalize();
  return 0;
}
