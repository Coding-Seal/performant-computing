#pragma once

#include <cstddef>

#include <mpi.h>

namespace lab10_11 {

std::size_t largest_power_of_two_leq(std::size_t n);

bool is_extra_rank(int rank, int size);

int extra_phase_partner(int rank, int size);

int round_partner(int rank, std::size_t round_mask);

void barrier_dichotomy(MPI_Comm comm);

void barrier_builtin(MPI_Comm comm);

} // namespace lab10_11
