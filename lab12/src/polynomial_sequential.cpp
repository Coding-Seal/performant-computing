#include "polynomial_sequential.hpp"
#include <cstddef>
#include <vector>

namespace lab12 {

double evaluate_polynomial_sequential(double a0,
                                      const std::vector<double> &coeffs,
                                      std::size_t r, double x) {
  const std::size_t s = coeffs.size() / r;

  std::vector<double> powers(r);
  powers[0] = 1.0;
  for (std::size_t j = 1; j < r; ++j) {
    powers[j] = powers[j - 1] * x;
  }
  const double x_r = powers[r - 1] * x;

  double total = a0;
  double block_multiplier = 1.0;
  for (std::size_t i = 0; i < s; ++i) {
    double qi = 0.0;
    for (std::size_t j = 0; j < r; ++j) {
      qi += coeffs[(i * r) + j] * powers[j];
    }
    total += block_multiplier * qi;
    block_multiplier *= x_r;
  }
  return total;
}

} // namespace lab12
