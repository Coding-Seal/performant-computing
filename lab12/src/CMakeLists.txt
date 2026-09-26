find_package(MPI REQUIRED COMPONENTS CXX)

add_library(lab12 STATIC
  block_distribution.cpp
  polynomial_sequential.cpp
  polynomial_parallel.cpp
)

target_include_directories(lab12 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lab12 PUBLIC MPI::MPI_CXX)

add_executable(polynomial_benchmark
  main.cpp
)
target_link_libraries(polynomial_benchmark PRIVATE lab12)

set(MPIEXEC_RUN_FLAGS ${MPIEXEC_PREFLAGS})
if(MPIEXEC_EXECUTABLE)
  execute_process(
    COMMAND ${MPIEXEC_EXECUTABLE} --version
    OUTPUT_VARIABLE MPIEXEC_VERSION_OUTPUT
    ERROR_VARIABLE MPIEXEC_VERSION_OUTPUT
  )
  if(MPIEXEC_VERSION_OUTPUT MATCHES "Open MPI")
    list(APPEND MPIEXEC_RUN_FLAGS --oversubscribe)
  endif()
endif()

if(MPIEXEC_EXECUTABLE)
  foreach(nproc 1 2 4 8)
    add_test(
      NAME polynomial_benchmark_np${nproc}
      COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} ${nproc}
              ${MPIEXEC_RUN_FLAGS}
              $<TARGET_FILE:polynomial_benchmark> 1024 8 200
              ${MPIEXEC_POSTFLAGS}
    )
  endforeach()
endif()
