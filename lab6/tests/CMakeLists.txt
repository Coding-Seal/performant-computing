find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(lab6_tests
  string_compare_test.cpp
)

target_link_libraries(lab6_tests PRIVATE lab6 GTest::gtest GTest::gtest_main)

gtest_discover_tests(lab6_tests)
