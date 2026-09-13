#include "geometric_sequence.hpp"

#include <cmath>
#include <cstddef>
#include <vector>

namespace lab7_1 {

void compute_geometric_sequence_racy(std::vector<double> &a, double f,
                                     int num_threads) {
#pragma omp parallel for num_threads(num_threads)
  for (std::size_t i = 1; i < a.size(); ++i) {
    a[i] = f * a[i - 1];
  }
}

void compute_geometric_sequence_parallel(std::vector<double> &a, double f,
                                         int num_threads) {
  const double a0 = a[0];
  const std::size_t n = a.size();

#pragma omp parallel for num_threads(num_threads) default(none) shared(a, n)   \
    firstprivate(a0, f)
  for (std::size_t i = 1; i < n; ++i) {
    a[i] = a0 * std::pow(f, static_cast<double>(i));
  }
}

} // namespace lab7_1
