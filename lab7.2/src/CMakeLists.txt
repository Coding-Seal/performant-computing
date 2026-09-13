find_package(OpenMP REQUIRED)

add_library(lab7_2 STATIC
  matrix_multiply.cpp
)

target_include_directories(lab7_2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lab7_2 PUBLIC OpenMP::OpenMP_CXX)

add_executable(matrix_multiply_demo
  main.cpp
)
target_link_libraries(matrix_multiply_demo PRIVATE lab7_2)
