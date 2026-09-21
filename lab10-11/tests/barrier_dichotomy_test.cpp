#include <cstddef>
#include <gtest/gtest.h>

#include "barrier.hpp"

TEST(LargestPowerOfTwoLeq, PowersOfTwoReturnThemselves) {
  EXPECT_EQ(lab10_11::largest_power_of_two_leq(1), 1U);
  EXPECT_EQ(lab10_11::largest_power_of_two_leq(2), 2U);
  EXPECT_EQ(lab10_11::largest_power_of_two_leq(8), 8U);
}

TEST(LargestPowerOfTwoLeq, NonPowersRoundDown) {
  EXPECT_EQ(lab10_11::largest_power_of_two_leq(3), 2U);
  EXPECT_EQ(lab10_11::largest_power_of_two_leq(5), 4U);
  EXPECT_EQ(lab10_11::largest_power_of_two_leq(9), 8U);
  EXPECT_EQ(lab10_11::largest_power_of_two_leq(1023), 512U);
}

TEST(IsExtraRank, PowerOfTwoSizeHasNoExtraRanks) {
  for (int rank = 0; rank < 8; ++rank) {
    EXPECT_FALSE(lab10_11::is_extra_rank(rank, 8));
  }
}

TEST(IsExtraRank, NonPowerOfTwoSizeMarksTailRanksAsExtra) {
  EXPECT_FALSE(lab10_11::is_extra_rank(0, 5));
  EXPECT_FALSE(lab10_11::is_extra_rank(3, 5));
  EXPECT_TRUE(lab10_11::is_extra_rank(4, 5));
}

TEST(ExtraPhasePartner, IsSymmetric) {
  constexpr int size = 5;
  for (int rank = 0; rank < size; ++rank) {
    const int partner = lab10_11::extra_phase_partner(rank, size);
    EXPECT_EQ(lab10_11::extra_phase_partner(partner, size), rank);
  }
}

TEST(ExtraPhasePartner, MatchesExpectedPow2Offset) {
  EXPECT_EQ(lab10_11::extra_phase_partner(0, 5), 4);
  EXPECT_EQ(lab10_11::extra_phase_partner(4, 5), 0);
  EXPECT_EQ(lab10_11::extra_phase_partner(1, 3), 3);
}

TEST(RoundPartner, XorsWithMask) {
  EXPECT_EQ(lab10_11::round_partner(0, 1), 1);
  EXPECT_EQ(lab10_11::round_partner(1, 1), 0);
  EXPECT_EQ(lab10_11::round_partner(2, 4), 6);
  EXPECT_EQ(lab10_11::round_partner(6, 4), 2);
}

TEST(RoundPartner, IsInvolution) {
  for (int rank = 0; rank < 16; ++rank) {
    for (std::size_t mask = 1; mask < 16; mask <<= 1) {
      const int partner = lab10_11::round_partner(rank, mask);
      EXPECT_EQ(lab10_11::round_partner(partner, mask), rank);
    }
  }
}
