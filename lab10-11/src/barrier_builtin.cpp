#include "barrier.hpp"
#include <mpi.h>

namespace lab10_11 {

void barrier_builtin(MPI_Comm comm) { MPI_Barrier(comm); }

} // namespace lab10_11
