find_package(OpenMP REQUIRED)

add_executable(dependency_example
  dependency_example.cpp
)
target_link_libraries(dependency_example PRIVATE OpenMP::OpenMP_CXX)
