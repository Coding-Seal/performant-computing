#include <gtest/gtest.h>

#include "island.hpp"

TEST(MakeIsland, TreasureIsWithinBounds) {
  const lab9::Island island = lab9::make_island(37, 41, 7);

  EXPECT_LT(island.treasure_x, island.width);
  EXPECT_LT(island.treasure_y, island.height);
}

TEST(MakeIsland, SameSeedIsReproducible) {
  const lab9::Island a = lab9::make_island(100, 100, 123);
  const lab9::Island b = lab9::make_island(100, 100, 123);

  EXPECT_EQ(a.treasure_x, b.treasure_x);
  EXPECT_EQ(a.treasure_y, b.treasure_y);
}

TEST(MakeIsland, SingleCellIslandPlacesTreasureAtOrigin) {
  const lab9::Island island = lab9::make_island(1, 1, 99);

  EXPECT_EQ(island.treasure_x, 0U);
  EXPECT_EQ(island.treasure_y, 0U);
}
