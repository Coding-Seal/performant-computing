#include <utility>

#include "barrier.hpp"
#include <cstddef>
#include <mpi.h>

namespace lab10_11 {

namespace {
constexpr int kTagFold = 1;
constexpr int kTagRelease = 2;
constexpr int kTagRoundBase = 10;
} // namespace

std::size_t largest_power_of_two_leq(std::size_t n) {
  std::size_t pow2 = 1;
  while (pow2 * 2 <= n) {
    pow2 *= 2;
  }
  return pow2;
}

bool is_extra_rank(int rank, int size) {
  return std::cmp_greater_equal(
      rank, largest_power_of_two_leq(static_cast<std::size_t>(size)));
}

int extra_phase_partner(int rank, int size) {
  const int pow2 = static_cast<int>(
      largest_power_of_two_leq(static_cast<std::size_t>(size)));
  return is_extra_rank(rank, size) ? rank - pow2 : rank + pow2;
}

int round_partner(int rank, std::size_t round_mask) {
  return rank ^ static_cast<int>(round_mask);
}

void barrier_dichotomy(MPI_Comm comm) {
  int rank = 0;
  int size = 0;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  const std::size_t pow2 =
      largest_power_of_two_leq(static_cast<std::size_t>(size));
  const int extra = size - static_cast<int>(pow2);

  if (is_extra_rank(rank, size)) {
    const int partner = extra_phase_partner(rank, size);
    MPI_Send(nullptr, 0, MPI_BYTE, partner, kTagFold, comm);
    MPI_Recv(nullptr, 0, MPI_BYTE, partner, kTagRelease, comm,
             MPI_STATUS_IGNORE);
    return;
  }

  if (rank < extra) {
    const int partner = extra_phase_partner(rank, size);
    MPI_Recv(nullptr, 0, MPI_BYTE, partner, kTagFold, comm, MPI_STATUS_IGNORE);
  }

  std::size_t round = 0;
  for (std::size_t mask = 1; mask < pow2; mask <<= 1, ++round) {
    const int partner = round_partner(rank, mask);
    const int tag = kTagRoundBase + static_cast<int>(round);
    MPI_Sendrecv(nullptr, 0, MPI_BYTE, partner, tag, nullptr, 0, MPI_BYTE,
                 partner, tag, comm, MPI_STATUS_IGNORE);
  }

  if (rank < extra) {
    const int partner = extra_phase_partner(rank, size);
    MPI_Send(nullptr, 0, MPI_BYTE, partner, kTagRelease, comm);
  }
}

} // namespace lab10_11
