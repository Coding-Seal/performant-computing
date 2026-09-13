find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(lab7_2_tests
  matrix_multiply_test.cpp
)

target_link_libraries(lab7_2_tests PRIVATE lab7_2 GTest::gtest GTest::gtest_main)

gtest_discover_tests(lab7_2_tests)
