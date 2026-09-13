#include <cstdio>
#include <cstdlib>
#include <string>

#include "string_compare_parallel.hpp"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::fprintf(stderr, "usage: %s <string_a> <string_b> <num_threads>\n",
                 argv[0]);
    return 1;
  }

  const std::string a = argv[1];
  const std::string b = argv[2];
  const int num_threads = static_cast<int>(std::strtol(argv[3], nullptr, 10));

  const bool sequential_result = a == b;
  const bool parallel_result = lab6::strings_equal_parallel(a, b, num_threads);

  std::printf("sequential: %s\n", sequential_result ? "EQUAL" : "NOT EQUAL");
  std::printf("parallel (%d threads): %s\n", num_threads,
              parallel_result ? "EQUAL" : "NOT EQUAL");

  return 0;
}
