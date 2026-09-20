find_package(OpenMP REQUIRED)

add_executable(hello_openmp
  hello_openmp.cpp
)
target_link_libraries(hello_openmp PRIVATE OpenMP::OpenMP_CXX)

add_executable(power_table_sequential
  power_table_sequential.cpp
)

add_executable(power_table_parallel
  power_table_parallel.cpp
)
target_link_libraries(power_table_parallel PRIVATE OpenMP::OpenMP_CXX)

add_executable(sum_of_squares
  sum_of_squares.cpp
)
target_link_libraries(sum_of_squares PRIVATE OpenMP::OpenMP_CXX)
