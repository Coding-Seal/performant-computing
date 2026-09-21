find_package(MPI REQUIRED)

add_executable(comm_split_broadcast_example
  comm_split_broadcast_example.cpp
)
target_link_libraries(comm_split_broadcast_example PRIVATE MPI::MPI_CXX)
