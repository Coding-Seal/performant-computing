find_package(OpenMP REQUIRED)

add_executable(primes_openmp
  primes_openmp.cpp
)
target_link_libraries(primes_openmp PRIVATE OpenMP::OpenMP_CXX)
