#include <cstddef>
#include <gtest/gtest.h>

#include "block_distribution.hpp"

TEST(BlockRange, EvenlyDivisibleGivesEqualChunks) {
  constexpr std::size_t num_blocks = 8;
  constexpr int size = 4;
  for (int rank = 0; rank < size; ++rank) {
    const lab12::BlockRange range = lab12::block_range(rank, size, num_blocks);
    EXPECT_EQ(range.end - range.begin, 2U);
  }
}

TEST(BlockRange, PartitionsAllBlocksContiguouslyWithoutGaps) {
  constexpr std::size_t num_blocks = 10;
  constexpr int size = 3;
  std::size_t expected_begin = 0;
  for (int rank = 0; rank < size; ++rank) {
    const lab12::BlockRange range = lab12::block_range(rank, size, num_blocks);
    EXPECT_EQ(range.begin, expected_begin);
    EXPECT_LE(range.begin, range.end);
    expected_begin = range.end;
  }
  EXPECT_EQ(expected_begin, num_blocks);
}

TEST(BlockRange, RemainderGoesToLowestRanksOneEach) {
  // 10 blocks over 3 ranks -> sizes 4,3,3
  EXPECT_EQ(lab12::block_range(0, 3, 10).end -
                lab12::block_range(0, 3, 10).begin,
            4U);
  EXPECT_EQ(lab12::block_range(1, 3, 10).end -
                lab12::block_range(1, 3, 10).begin,
            3U);
  EXPECT_EQ(lab12::block_range(2, 3, 10).end -
                lab12::block_range(2, 3, 10).begin,
            3U);
}

TEST(BlockRange, MoreRanksThanBlocksLeavesSomeEmpty) {
  const lab12::BlockRange owned = lab12::block_range(0, 5, 2);
  const lab12::BlockRange empty = lab12::block_range(2, 5, 2);
  EXPECT_EQ(owned.end - owned.begin, 1U);
  EXPECT_EQ(empty.begin, empty.end);
}

TEST(BlockRange, SingleRankOwnsEverything) {
  const lab12::BlockRange range = lab12::block_range(0, 1, 7);
  EXPECT_EQ(range.begin, 0U);
  EXPECT_EQ(range.end, 7U);
}

TEST(ScatterPlan, CountsMatchBlockRangeSizesTimesR) {
  constexpr std::size_t num_blocks = 10;
  constexpr std::size_t r = 4;
  constexpr int size = 3;
  const lab12::ScatterPlan plan = lab12::scatter_plan(size, num_blocks, r);

  for (int rank = 0; rank < size; ++rank) {
    const lab12::BlockRange range = lab12::block_range(rank, size, num_blocks);
    EXPECT_EQ(plan.counts[static_cast<std::size_t>(rank)],
              static_cast<int>((range.end - range.begin) * r));
  }
}

TEST(ScatterPlan, DisplacementsMatchBlockRangeBeginTimesR) {
  constexpr std::size_t num_blocks = 10;
  constexpr std::size_t r = 4;
  constexpr int size = 3;
  const lab12::ScatterPlan plan = lab12::scatter_plan(size, num_blocks, r);

  for (int rank = 0; rank < size; ++rank) {
    const lab12::BlockRange range = lab12::block_range(rank, size, num_blocks);
    EXPECT_EQ(plan.displacements[static_cast<std::size_t>(rank)],
              static_cast<int>(range.begin * r));
  }
}

TEST(ScatterPlan, CountsSumToTotalCoefficientCount) {
  constexpr std::size_t num_blocks = 17;
  constexpr std::size_t r = 3;
  constexpr int size = 5;
  const lab12::ScatterPlan plan = lab12::scatter_plan(size, num_blocks, r);

  int total = 0;
  for (int const count : plan.counts) {
    total += count;
  }
  EXPECT_EQ(total, static_cast<int>(num_blocks * r));
}
