find_package(OpenMP REQUIRED)

add_library(lab7_1 STATIC
  geometric_sequence.cpp
)

target_include_directories(lab7_1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lab7_1 PUBLIC OpenMP::OpenMP_CXX)

add_executable(geometric_sequence_demo
  main.cpp
)
target_link_libraries(geometric_sequence_demo PRIVATE lab7_1)
