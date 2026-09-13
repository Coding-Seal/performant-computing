#include <cmath>
#include <cstddef>
#include <cstdio>

constexpr std::size_t N = 100;

int main() {
  double b[N];
  for (std::size_t i = 0; i < N; ++i) {
    const auto x = static_cast<double>(i);
    b[i] = x * std::sqrt(x);
  }

  for (double const i : b) {
    std::printf("%f ", i);
  }
  std::printf("\n");

  return 0;
}
