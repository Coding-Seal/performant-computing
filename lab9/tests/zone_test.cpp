#include <cstddef>
#include <gtest/gtest.h>

#include "zone.hpp"

TEST(PartitionIntoZones, EvenSplitCoversWholeIsland) {
  const auto zones = lab9::partition_into_zones(20, 12, 4);

  ASSERT_EQ(zones.size(), 4U);
  EXPECT_EQ(zones.front().y_begin, 0U);
  EXPECT_EQ(zones.back().y_end, 12U);
  for (const auto &zone : zones) {
    EXPECT_EQ(zone.x_begin, 0U);
    EXPECT_EQ(zone.x_end, 20U);
    EXPECT_EQ(zone.y_end - zone.y_begin, 3U);
  }
}

TEST(PartitionIntoZones, UnevenSplitDistributesRemainderToFirstZones) {
  const auto zones = lab9::partition_into_zones(10, 10, 3);

  ASSERT_EQ(zones.size(), 3U);
  EXPECT_EQ(zones[0].y_end - zones[0].y_begin, 4U);
  EXPECT_EQ(zones[1].y_end - zones[1].y_begin, 3U);
  EXPECT_EQ(zones[2].y_end - zones[2].y_begin, 3U);
  EXPECT_EQ(zones.back().y_end, 10U);
}

TEST(PartitionIntoZones, ZonesAreContiguousAndNonOverlapping) {
  const auto zones = lab9::partition_into_zones(5, 17, 5);

  ASSERT_EQ(zones.size(), 5U);
  for (std::size_t i = 1; i < zones.size(); ++i) {
    EXPECT_EQ(zones[i].y_begin, zones[i - 1].y_end);
  }
}

TEST(PartitionIntoZones, MoreZonesThanRowsYieldsEmptyZones) {
  const auto zones = lab9::partition_into_zones(4, 2, 5);

  ASSERT_EQ(zones.size(), 5U);
  std::size_t empty_zones = 0;
  for (const auto &zone : zones) {
    if (zone.y_begin == zone.y_end) {
      ++empty_zones;
    }
  }
  EXPECT_EQ(empty_zones, 3U);
}

TEST(PartitionIntoZones, ZeroZonesReturnsEmptyVector) {
  const auto zones = lab9::partition_into_zones(10, 10, 0);

  EXPECT_TRUE(zones.empty());
}
