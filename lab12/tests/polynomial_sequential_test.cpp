#include <cmath>
#include <cstddef>
#include <gtest/gtest.h>
#include <vector>

#include "polynomial_sequential.hpp"

namespace {

// Independent brute-force reference: total = a0 + sum_i x^(i*r) * q_i(x),
// q_i(x) = sum_j coeffs[i*r+j] * x^j.
double reference_evaluate(double a0, const std::vector<double> &coeffs,
                          std::size_t r, double x) {
  const std::size_t s = coeffs.size() / r;
  double total = a0;
  for (std::size_t i = 0; i < s; ++i) {
    double qi = 0.0;
    for (std::size_t j = 0; j < r; ++j) {
      qi += coeffs[(i * r) + j] * std::pow(x, static_cast<double>(j));
    }
    total += std::pow(x, static_cast<double>(i * r)) * qi;
  }
  return total;
}

} // namespace

TEST(EvaluatePolynomialSequential, MatchesHandComputedExample) {
  // a0=1, q1(x)=2+3x, q2(x)=4+5x, x=2:
  // q1(2)=8, q2(2)=14, x^2*q2(2)=56, total=1+8+56=65
  const std::vector<double> coeffs = {2.0, 3.0, 4.0, 5.0};
  EXPECT_DOUBLE_EQ(lab12::evaluate_polynomial_sequential(1.0, coeffs, 2, 2.0),
                   65.0);
}

TEST(EvaluatePolynomialSequential, MatchesBruteForceReferenceAcrossShapes) {
  const struct {
    double a0;
    std::vector<double> coeffs;
    std::size_t r;
    double x;
  } cases[] = {
      {.a0 = 0.0,
       .coeffs = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0},
       .r = 1,
       .x = 1.5},
      {.a0 = -3.0,
       .coeffs = {1.0, -2.0, 3.0, -4.0, 5.0, -6.0, 7.0, -8.0},
       .r = 2,
       .x = 0.75},
      {.a0 = 2.5,
       .coeffs = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0},
       .r = 4,
       .x = -1.2},
      {.a0 = 1.0,
       .coeffs = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0},
       .r = 8,
       .x = 1.0001},
  };

  for (const auto &c : cases) {
    EXPECT_NEAR(lab12::evaluate_polynomial_sequential(c.a0, c.coeffs, c.r, c.x),
                reference_evaluate(c.a0, c.coeffs, c.r, c.x), 1e-9);
  }
}

TEST(EvaluatePolynomialSequential, AtZeroOnlyFirstCoefficientSurvives) {
  const std::vector<double> coeffs = {7.0, 9.0, 11.0, 13.0};
  EXPECT_DOUBLE_EQ(lab12::evaluate_polynomial_sequential(3.0, coeffs, 2, 0.0),
                   3.0 + 7.0);
}

TEST(EvaluatePolynomialSequential, AtOneSumsAllCoefficientsPlusA0) {
  const std::vector<double> coeffs = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  double sum = 4.0;
  for (double const c : coeffs) {
    sum += c;
  }
  EXPECT_DOUBLE_EQ(lab12::evaluate_polynomial_sequential(4.0, coeffs, 3, 1.0),
                   sum);
}
