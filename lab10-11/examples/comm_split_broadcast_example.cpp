#include <cstdio>

#include <mpi.h>

int main(int argc, char **argv) {
  int rank1 = 0;
  int size1 = 0;
  int rank2 = 0;
  int size2 = 0;
  int value = 0;
  MPI_Comm comm1;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank1);
  MPI_Comm_size(MPI_COMM_WORLD, &size1);
  std::printf("Мой ранг в MPI_COMM_WORLD %d\n", rank1);
  std::printf("Всего в MPI_COMM_WORLD %d процессов\n", size1);

  MPI_Comm_split(MPI_COMM_WORLD, (rank1 % 2 == 0) ? 0 : 1, 0, &comm1);
  MPI_Comm_rank(comm1, &rank2);
  MPI_Comm_size(comm1, &size2);

  if (rank2 == 0 && ((rank1 % 2) != 0)) {
    value = 777;
  }
  if (rank2 == 0 && ((rank1 % 2) == 0)) {
    value = 666;
  }

  MPI_Bcast(&value, 1, MPI_INT, 0, comm1);
  std::printf("Мой ранг в MPI_COMM_WORLD %d, мой ранг в новом коммуникаторе "
              "%d, broadcasted message %d\n",
              rank1, rank2, value);

  MPI_Comm_free(&comm1);
  MPI_Finalize();
  return 0;
}
