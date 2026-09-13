#include <cmath>
#include <cstdio>

namespace {
constexpr int kMaxNumbers = 10000;
}

static int primes[kMaxNumbers];
static int prime_count;

int main() {
  prime_count = 0;
  primes[prime_count++] = 2;

  std::printf("Determining primes from 1 - %d\n", kMaxNumbers);

#pragma omp parallel for
  for (int i = 3; i < kMaxNumbers; i += 2) {
    int factor = 3;
    const int max_factor = static_cast<int>(std::sqrt(static_cast<double>(i)));
    while ((i % factor) != 0 && factor <= max_factor) {
      factor += 2;
    }
    if (factor > max_factor) {
#pragma omp critical
      {
        primes[prime_count] = i;
        prime_count++;
      }
    }
  }

  std::printf("Found %d primes\n", prime_count);
  return 0;
}
