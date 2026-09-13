#include <cstddef>
#include <gtest/gtest.h>

#include <random>

#include "matrix_multiply.hpp"

namespace {

lab7_2::Matrix random_matrix(std::size_t n, unsigned seed) {
  std::mt19937 rng(seed);
  std::uniform_real_distribution<double> dist(-5.0, 5.0);
  lab7_2::Matrix m(n * n);
  for (double &v : m) {
    v = dist(rng);
  }
  return m;
}

} // namespace

TEST(MatrixMultiplyParallelFixedThreads,
     MatchesSequentialWhenSizeDividesThreads) {
  constexpr std::size_t n = 8;
  const lab7_2::Matrix a = random_matrix(n, 1);
  const lab7_2::Matrix b = random_matrix(n, 2);

  lab7_2::Matrix expected(n * n);
  lab7_2::multiply_sequential(a, b, expected, n);

  lab7_2::Matrix actual(n * n);
  lab7_2::multiply_parallel_fixed_threads(a, b, actual, n, 4);

  EXPECT_EQ(actual, expected);
}

TEST(MatrixMultiplyParallelFixedThreads,
     MatchesSequentialWhenSizeDoesNotDivideThreads) {
  constexpr std::size_t n = 7;
  const lab7_2::Matrix a = random_matrix(n, 3);
  const lab7_2::Matrix b = random_matrix(n, 4);

  lab7_2::Matrix expected(n * n);
  lab7_2::multiply_sequential(a, b, expected, n);

  lab7_2::Matrix actual(n * n);
  lab7_2::multiply_parallel_fixed_threads(a, b, actual, n, 4);

  EXPECT_EQ(actual, expected);
}

TEST(MatrixMultiplyParallelFixedThreads,
     MatchesSequentialWhenThreadsExceedSize) {
  constexpr std::size_t n = 3;
  const lab7_2::Matrix a = random_matrix(n, 5);
  const lab7_2::Matrix b = random_matrix(n, 6);

  lab7_2::Matrix expected(n * n);
  lab7_2::multiply_sequential(a, b, expected, n);

  lab7_2::Matrix actual(n * n);
  lab7_2::multiply_parallel_fixed_threads(a, b, actual, n, 8);

  EXPECT_EQ(actual, expected);
}

TEST(MatrixMultiplyParallelAuto, MatchesSequential) {
  constexpr std::size_t n = 10;
  const lab7_2::Matrix a = random_matrix(n, 7);
  const lab7_2::Matrix b = random_matrix(n, 8);

  lab7_2::Matrix expected(n * n);
  lab7_2::multiply_sequential(a, b, expected, n);

  lab7_2::Matrix actual(n * n);
  lab7_2::multiply_parallel_auto(a, b, actual, n);

  EXPECT_EQ(actual, expected);
}

TEST(MatrixMultiplySequential, IdentityMatrixIsUnchanged) {
  constexpr std::size_t n = 4;
  lab7_2::Matrix identity(n * n, 0.0);
  for (std::size_t i = 0; i < n; ++i) {
    identity[lab7_2::index(i, i, n)] = 1.0;
  }
  const lab7_2::Matrix a = random_matrix(n, 9);

  lab7_2::Matrix result(n * n);
  lab7_2::multiply_sequential(a, identity, result, n);

  EXPECT_EQ(result, a);
}
