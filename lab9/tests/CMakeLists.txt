find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(lab9_tests
  island_test.cpp
  zone_test.cpp
  search_test.cpp
)

target_link_libraries(lab9_tests PRIVATE lab9 GTest::gtest GTest::gtest_main)

gtest_discover_tests(lab9_tests)
