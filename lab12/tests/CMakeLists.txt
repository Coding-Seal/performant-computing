find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(lab12_tests
  block_distribution_test.cpp
  polynomial_sequential_test.cpp
)

target_link_libraries(lab12_tests PRIVATE lab12 GTest::gtest GTest::gtest_main)

gtest_discover_tests(lab12_tests)
