#include <cstdio>
#include <cstdlib>
#include <random>

#include <omp.h>

#include "matrix_multiply.hpp"

namespace {

lab7_2::Matrix random_matrix(std::size_t n, std::mt19937 &rng) {
  std::uniform_real_distribution<double> dist(-10.0, 10.0);
  lab7_2::Matrix m(n * n);
  for (double &v : m) {
    v = dist(rng);
  }
  return m;
}

} // namespace

int main(int argc, char **argv) {
  if (argc != 3) {
    std::fprintf(stderr, "usage: %s <n> <num_threads>\n", argv[0]);
    return 1;
  }

  const auto n = static_cast<std::size_t>(std::strtoul(argv[1], nullptr, 10));
  const int num_threads = static_cast<int>(std::strtol(argv[2], nullptr, 10));

  std::mt19937 rng(42);
  const lab7_2::Matrix a = random_matrix(n, rng);
  const lab7_2::Matrix b = random_matrix(n, rng);

  lab7_2::Matrix c_sequential(n * n);
  lab7_2::Matrix c_fixed(n * n);
  lab7_2::Matrix c_auto(n * n);

  double start = omp_get_wtime();
  lab7_2::multiply_sequential(a, b, c_sequential, n);
  const double sequential_seconds = omp_get_wtime() - start;

  start = omp_get_wtime();
  lab7_2::multiply_parallel_fixed_threads(a, b, c_fixed, n, num_threads);
  const double fixed_seconds = omp_get_wtime() - start;

  start = omp_get_wtime();
  lab7_2::multiply_parallel_auto(a, b, c_auto, n);
  const double auto_seconds = omp_get_wtime() - start;

  std::printf("n=%zu num_threads=%d\n", n, num_threads);
  std::printf("sequential:             %.6fs\n", sequential_seconds);
  std::printf("parallel fixed threads: %.6fs (matches sequential: %s)\n",
              fixed_seconds, c_fixed == c_sequential ? "yes" : "no");
  std::printf("parallel auto threads:  %.6fs (matches sequential: %s)\n",
              auto_seconds, c_auto == c_sequential ? "yes" : "no");

  return 0;
}
