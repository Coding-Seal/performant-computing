#include <cstddef>
#include <cstdio>

namespace {
constexpr std::size_t kNMax = 11;
}

static double a[kNMax];
static double b_racy[kNMax];
static double b_fixed[kNMax];

int main() {
  a[0] = 0.0;

#pragma omp parallel
  {
#pragma omp for
    for (std::size_t i = 1; i < kNMax; ++i) {
      a[i] = 1.0 / static_cast<double>(i);
      b_racy[i] = a[i] + a[i - 1];
      b_fixed[i] = a[i] + (1.0 / static_cast<double>(i - 1));
    }
  }

  for (std::size_t i = 1; i < kNMax; ++i) {
    std::printf("racy=%g fixed=%g\n", b_racy[i], b_fixed[i]);
  }

  return 0;
}
