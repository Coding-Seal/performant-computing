#pragma once

#include <cstddef>
#include <vector>

#include <mpi.h>

namespace lab12 {

// coeffs (size s*r) is only read on rank 0; every other rank may pass an
// empty vector. Distributes it with MPI_Scatterv, sized per block_range(),
// and returns this rank's own slice (block-major, same layout as coeffs).
std::vector<double> scatter_coefficients(const std::vector<double> &coeffs,
                                         std::size_t s, std::size_t r,
                                         MPI_Comm comm);

// local_coeffs must be this rank's slice as produced by
// scatter_coefficients() for the same s, r, comm. Returns a0 + q(x) on
// rank 0; the value returned on other ranks is unspecified.
double evaluate_polynomial_parallel(double a0,
                                    const std::vector<double> &local_coeffs,
                                    std::size_t s, std::size_t r, double x,
                                    MPI_Comm comm);

} // namespace lab12
