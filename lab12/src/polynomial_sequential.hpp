#pragma once

#include <cstddef>
#include <vector>

namespace lab12 {

// coeffs holds s blocks of r coefficients each, block-major
// (block i occupies coeffs[i*r, i*r+r)); coeffs.size() must be a multiple
// of r. Evaluates a0 + sum_i x^(i*r) * q_i(x) where
// q_i(x) = coeffs[i*r] + coeffs[i*r+1]*x + ... + coeffs[i*r+r-1]*x^(r-1).
double evaluate_polynomial_sequential(double a0,
                                      const std::vector<double> &coeffs,
                                      std::size_t r, double x);

} // namespace lab12
