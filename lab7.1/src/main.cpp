#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "geometric_sequence.hpp"

namespace {

bool matches_closed_form(const std::vector<double> &a, double f) {
  for (std::size_t i = 0; i < a.size(); ++i) {
    if (a[i] != std::pow(f, static_cast<double>(i))) {
      return false;
    }
  }
  return true;
}

} // namespace

int main(int argc, char **argv) {
  if (argc != 4) {
    std::fprintf(stderr, "usage: %s <n> <f> <num_threads>\n", argv[0]);
    return 1;
  }

  const auto n = static_cast<std::size_t>(std::strtoul(argv[1], nullptr, 10));
  const double f = std::strtod(argv[2], nullptr);
  const int num_threads = static_cast<int>(std::strtol(argv[3], nullptr, 10));

  std::vector<double> racy(n, 0.0);
  racy[0] = 1.0;
  lab7_1::compute_geometric_sequence_racy(racy, f, num_threads);

  std::vector<double> fixed(n);
  fixed[0] = 1.0;
  lab7_1::compute_geometric_sequence_parallel(fixed, f, num_threads);

  std::printf("n=%zu f=%g threads=%d\n", n, f, num_threads);
  std::printf("racy parallel matches a[i]=f^i:  %s\n",
              matches_closed_form(racy, f) ? "yes" : "no");
  std::printf("fixed parallel matches a[i]=f^i: %s\n",
              matches_closed_form(fixed, f) ? "yes" : "no");

  return 0;
}
