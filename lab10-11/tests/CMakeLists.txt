find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(lab10_11_tests
  barrier_dichotomy_test.cpp
)

target_link_libraries(lab10_11_tests PRIVATE lab10_11 GTest::gtest GTest::gtest_main)

gtest_discover_tests(lab10_11_tests)
