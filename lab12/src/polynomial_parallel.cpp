#include "polynomial_parallel.hpp"

#include <cmath>
#include <cstddef>
#include <mpi.h>
#include <vector>

#include "block_distribution.hpp"

namespace lab12 {

std::vector<double> scatter_coefficients(const std::vector<double> &coeffs,
                                         std::size_t s, std::size_t r,
                                         MPI_Comm comm) {
  int rank = 0;
  int size = 0;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  const BlockRange range = block_range(rank, size, s);
  std::vector<double> local_coeffs((range.end - range.begin) * r);

  const ScatterPlan plan = scatter_plan(size, s, r);
  MPI_Scatterv(rank == 0 ? coeffs.data() : nullptr,
               rank == 0 ? plan.counts.data() : nullptr,
               rank == 0 ? plan.displacements.data() : nullptr, MPI_DOUBLE,
               local_coeffs.data(), static_cast<int>(local_coeffs.size()),
               MPI_DOUBLE, 0, comm);

  return local_coeffs;
}

double evaluate_polynomial_parallel(double a0,
                                    const std::vector<double> &local_coeffs,
                                    std::size_t s, std::size_t r, double x,
                                    MPI_Comm comm) {
  int rank = 0;
  int size = 0;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  const BlockRange range = block_range(rank, size, s);
  const std::size_t local_blocks = range.end - range.begin;

  std::vector<double> powers(r);
  powers[0] = 1.0;
  for (std::size_t j = 1; j < r; ++j) {
    powers[j] = powers[j - 1] * x;
  }
  const double x_r = powers[r - 1] * x;

  double multiplier = std::pow(x_r, static_cast<double>(range.begin));

  double partial_sum = 0.0;
  for (std::size_t i = 0; i < local_blocks; ++i) {
    double qi = 0.0;
    for (std::size_t j = 0; j < r; ++j) {
      qi += local_coeffs[(i * r) + j] * powers[j];
    }
    partial_sum += multiplier * qi;
    multiplier *= x_r;
  }

  double total = 0.0;
  MPI_Reduce(&partial_sum, &total, 1, MPI_DOUBLE, MPI_SUM, 0, comm);

  return a0 + total;
}

} // namespace lab12
