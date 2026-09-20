#include <omp.h>

#include <cstddef>
#include <cstdio>

namespace {
constexpr std::size_t N = 100;
int A[N];
} // namespace

int main() {
  for (std::size_t i = 0; i < N; ++i) {
    A[i] = static_cast<int>(i);
  }

  if (omp_get_max_threads() < 4) {
    std::printf("mало нитей\n");
    return 0;
  }
  omp_set_num_threads(4);

  double sum = 0.0;
  std::size_t i;
  double a2;

#pragma omp parallel for schedule(static, 25) private(i, a2) shared(A)         \
    reduction(+ : sum)
  for (i = 0; i < N; ++i) {
    a2 = A[i] * A[i];
    sum = sum + a2;
  }

  std::printf("sum = %f\n", sum);
  return 0;
}
