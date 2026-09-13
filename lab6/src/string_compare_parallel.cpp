#include "string_compare_parallel.hpp"

#include <cstddef>
#include <string>

namespace lab6 {

bool strings_equal_parallel(const std::string &a, const std::string &b,
                            int num_threads) {
  if (a.size() != b.size()) {
    return false;
  }

  const std::size_t n = a.size();
  bool equal = true;

#pragma omp parallel for num_threads(num_threads) schedule(static)             \
    reduction(&& : equal)
  for (std::size_t i = 0; i < n; ++i) {
    if (a[i] != b[i]) {
      equal = false;
    }
  }

  return equal;
}

} // namespace lab6
