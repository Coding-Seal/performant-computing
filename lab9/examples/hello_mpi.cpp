#include <cstdio>

#include <mpi.h>

namespace {
constexpr int kLength = 20;
}

int main(int argc, char **argv) {
  int rank = 0;
  int size = 0;
  char wr[kLength];
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::snprintf(wr, kLength, "Hello from %d", rank);
  const int partner = size - (rank + 1);
  const double t = MPI_Wtime();
  MPI_Send(wr, kLength, MPI_CHAR, partner, rank, MPI_COMM_WORLD);
  MPI_Recv(wr, kLength, MPI_CHAR, partner, partner, MPI_COMM_WORLD, &status);
  const double t2 = MPI_Wtime();

  std::printf("\n From processor %d\n WR=%s\n", rank, wr);
  std::printf("\n From processor %d\n Time=%le\n", rank, (t2 - t) / 100);

  MPI_Finalize();
  return 0;
}
