#include <gtest/gtest.h>

#include <string>

#include "string_compare_parallel.hpp"

namespace {

using lab6::strings_equal_parallel;

TEST(StringCompare, EqualStringsMatch) {
  EXPECT_TRUE(strings_equal_parallel("hello", "hello", 4));
}

TEST(StringCompare, DifferentLengthsDoNotMatch) {
  EXPECT_FALSE(strings_equal_parallel("hello", "hell", 4));
}

TEST(StringCompare, MismatchAtStartDetected) {
  EXPECT_FALSE(strings_equal_parallel("xello", "hello", 4));
}

TEST(StringCompare, MismatchAtEndDetected) {
  EXPECT_FALSE(strings_equal_parallel("hellx", "hello", 4));
}

TEST(StringCompare, EmptyStringsMatch) {
  EXPECT_TRUE(strings_equal_parallel("", "", 4));
}

TEST(StringCompare, ThreadCountNotDividingLengthEvenly) {
  const std::string a(101, 'a');
  const std::string matching(101, 'a');
  EXPECT_TRUE(strings_equal_parallel(a, matching, 4));

  std::string mismatching = matching;
  mismatching[100] = 'b';
  EXPECT_FALSE(strings_equal_parallel(a, mismatching, 4));
}

TEST(StringCompare, SingleThreadMatchesExpected) {
  EXPECT_TRUE(strings_equal_parallel("performance", "performance", 1));
}

} // namespace
