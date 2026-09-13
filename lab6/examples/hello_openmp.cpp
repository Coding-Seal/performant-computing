#include <omp.h>

#include <cstdio>

int main() {
  const int max_threads = omp_get_max_threads();
  const int threads_to_use = max_threads < 4 ? max_threads : 4;

#pragma omp parallel num_threads(threads_to_use)
  {
    const int id = omp_get_thread_num();
    const int total = omp_get_num_threads();

#pragma omp critical
    {
      std::printf("Hello, OpenMP! I am %d of %d\n", id, total);
    }
  }

  return 0;
}
