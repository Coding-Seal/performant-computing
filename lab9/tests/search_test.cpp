#include <gtest/gtest.h>

#include "island.hpp"
#include "search.hpp"
#include "zone.hpp"

TEST(SearchZone, FindsTreasureInsideZone) {
  const lab9::Island island{
      .width = 10, .height = 10, .treasure_x = 4, .treasure_y = 6};
  const lab9::Zone zone{.x_begin = 0, .x_end = 10, .y_begin = 0, .y_end = 10};

  const lab9::SearchResult result = lab9::search_zone(island, zone);

  EXPECT_TRUE(result.found);
  EXPECT_EQ(result.x, 4U);
  EXPECT_EQ(result.y, 6U);
  EXPECT_GT(result.cells_searched, 0U);
}

TEST(SearchZone, DoesNotFindTreasureOutsideZone) {
  const lab9::Island island{
      .width = 10, .height = 10, .treasure_x = 4, .treasure_y = 6};
  const lab9::Zone zone{.x_begin = 0, .x_end = 10, .y_begin = 0, .y_end = 3};

  const lab9::SearchResult result = lab9::search_zone(island, zone);

  EXPECT_FALSE(result.found);
  EXPECT_EQ(result.cells_searched, 30U);
}

TEST(SearchZone, StopsSearchingAsSoonAsTreasureIsFound) {
  const lab9::Island island{
      .width = 10, .height = 1, .treasure_x = 3, .treasure_y = 0};
  const lab9::Zone zone{.x_begin = 0, .x_end = 10, .y_begin = 0, .y_end = 1};

  const lab9::SearchResult result = lab9::search_zone(island, zone);

  EXPECT_TRUE(result.found);
  EXPECT_EQ(result.cells_searched, 4U);
}

TEST(SearchZone, EmptyZoneFindsNothing) {
  const lab9::Island island{
      .width = 10, .height = 10, .treasure_x = 4, .treasure_y = 6};
  const lab9::Zone zone{.x_begin = 0, .x_end = 10, .y_begin = 5, .y_end = 5};

  const lab9::SearchResult result = lab9::search_zone(island, zone);

  EXPECT_FALSE(result.found);
  EXPECT_EQ(result.cells_searched, 0U);
}
