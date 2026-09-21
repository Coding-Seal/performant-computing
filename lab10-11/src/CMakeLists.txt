find_package(MPI REQUIRED COMPONENTS CXX)

add_library(lab10_11 STATIC
  barrier_dichotomy.cpp
  barrier_builtin.cpp
)

target_include_directories(lab10_11 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lab10_11 PUBLIC MPI::MPI_CXX)

add_executable(barrier_benchmark
  main.cpp
)
target_link_libraries(barrier_benchmark PRIVATE lab10_11)

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
  foreach(nproc 2 3 5 8)
    add_test(
      NAME barrier_benchmark_np${nproc}
      COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} ${nproc}
              ${MPIEXEC_RUN_FLAGS}
              $<TARGET_FILE:barrier_benchmark> 2000
              ${MPIEXEC_POSTFLAGS}
    )
  endforeach()
endif()
