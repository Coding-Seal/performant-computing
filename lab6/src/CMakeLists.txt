find_package(OpenMP REQUIRED)

add_library(lab6 STATIC
  string_compare_parallel.cpp
)

target_include_directories(lab6 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lab6 PUBLIC OpenMP::OpenMP_CXX)

add_executable(string_compare_demo
  main.cpp
)
target_link_libraries(string_compare_demo PRIVATE lab6)
