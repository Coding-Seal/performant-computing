#include "matrix_multiply.hpp"
#include <cstddef>

namespace lab7_2 {

void multiply_sequential(const Matrix &a, const Matrix &b, Matrix &c,
                         std::size_t n) {
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      double sum = 0.0;
      for (std::size_t k = 0; k < n; ++k) {
        sum += a[index(i, k, n)] * b[index(k, j, n)];
      }
      c[index(i, j, n)] = sum;
    }
  }
}

void multiply_parallel_fixed_threads(const Matrix &a, const Matrix &b,
                                     Matrix &c, std::size_t n,
                                     int num_threads) {
#pragma omp parallel for num_threads(num_threads) default(none)                \
    shared(a, b, c, n)
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      double sum = 0.0;
      for (std::size_t k = 0; k < n; ++k) {
        sum += a[index(i, k, n)] * b[index(k, j, n)];
      }
      c[index(i, j, n)] = sum;
    }
  }
}

void multiply_parallel_auto(const Matrix &a, const Matrix &b, Matrix &c,
                            std::size_t n) {
#pragma omp parallel for default(none) shared(a, b, c, n)
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      double sum = 0.0;
      for (std::size_t k = 0; k < n; ++k) {
        sum += a[index(i, k, n)] * b[index(k, j, n)];
      }
      c[index(i, j, n)] = sum;
    }
  }
}

} // namespace lab7_2
