#pragma once

#include <cstddef>
#include <vector>

namespace lab7_2 {

using Matrix = std::vector<double>;

inline std::size_t index(std::size_t row, std::size_t col, std::size_t n) {
  return (row * n) + col;
}

void multiply_sequential(const Matrix &a, const Matrix &b, Matrix &c,
                         std::size_t n);

void multiply_parallel_fixed_threads(const Matrix &a, const Matrix &b,
                                     Matrix &c, std::size_t n, int num_threads);

void multiply_parallel_auto(const Matrix &a, const Matrix &b, Matrix &c,
                            std::size_t n);

} // namespace lab7_2
